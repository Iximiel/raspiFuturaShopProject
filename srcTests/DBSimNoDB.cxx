/**Partially copied from the examples of libgpiod
 */
#include <iostream>
#include <string>
#include <gpiod.hpp>
#include <chrono>
#include <thread>
#include <atomic>

#include "PCF8591onFT1060M.hpp"
#include "ledControllerForFT1060M.hpp"
#include "FT1060Mbase.hpp"

struct temp {
  std::atomic_bool changed;
  std::atomic_bool p1;
  std::atomic_bool p2;
  std::atomic_bool run;
} buttons;

void eventRoutine(const gpiod::line_event& event) {

  if (!(event.event_type == gpiod::line_event::RISING_EDGE
	|| event.event_type == gpiod::line_event::FALLING_EDGE)) {
    throw std::logic_error("invalid event type");
  }

  
  if(event.source.offset()==FT1060M::pinMap::gpioButton1) {
    buttons.p1=!buttons.p1;
  } else {
    buttons.p2=!buttons.p2;
  }

}

void ledServer(const gpiod::chip & GPIO,
	       const FT1060M::PCF8591& i2c){
  FT1060M::LedController LEDS(GPIO,i2c);
  while(buttons.run){
    LEDS.toggleLED1(buttons.p1);
    LEDS.toggleLED2(buttons.p2);
    std::this_thread::sleep_for (std::chrono::milliseconds(500));
  }
}

void buttonServer(const gpiod::chip & GPIO){
  gpiod::line Switch = GPIO.get_line(FT1060M::pinMap::gpioSwitch);
  bool wait = true;
  if (!Switch.is_requested()){
    Switch.request({
		    "DBSim",
		    ::gpiod::line_request::DIRECTION_INPUT,
		    0
      });
  } else {
    throw "Cannot request the switch";
  }
  
  const int initialSwitchPosition = Switch.get_value();
  gpiod::line_bulk Buttons = GPIO.get_lines({FT1060M::pinMap::gpioButton1,FT1060M::pinMap::gpioButton2});
 Buttons.request({
		   "DBSim",
		   //gpiod::line_request::EVENT_BOTH_EDGES,
		   gpiod::line_request::EVENT_RISING_EDGE,
		   0
    });

  while (Switch.get_value()==initialSwitchPosition) {
    auto events = Buttons.event_wait(std::chrono::milliseconds(500));
    if (events) {      
      for (auto& lineWithEvent: events) {
	eventRoutine(lineWithEvent.event_read());
      }
    }
  }
  buttons.run=false;
}

int main (int, char**argv) {
  int retval =0;
  try{
    buttons.run=true;
    gpiod::chip GPIO("pinctrl-bcm2835");
    FT1060M::PCF8591 i2c;
    i2c.setAnalogOutputEnabled(true);
    std::thread tbt(buttonServer,GPIO);
    std::thread tld(ledServer,GPIO,i2c);
    tbt.join();
    tld.join();

  } catch (const char *problem) {
    std::cerr << "\033[1;31mERROR: \033[0m\033[1m" << problem << "\033[0m" << std::endl;
    retval = 1;
  } catch (const std::string &problem) {
    std::cerr << "\033[1;31mERROR: \033[0m\033[1m" << problem << "\033[0m" << std::endl;
    retval = 1;
  }
  return retval;
}

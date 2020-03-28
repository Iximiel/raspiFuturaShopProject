/**Partially copied from the examples of libgpiod
 */
#include <iostream>
#include <string>
#include <gpiod.hpp>
#include <chrono>
#include <thread>

#include "PCF8591onFT1060M.hpp"
#include "ledControllerForFT1060M.hpp"

constexpr unsigned int switchPIN = 17;
constexpr unsigned int plusPIN   = 27;
constexpr unsigned int minusPIN  = 22;

int  eventRoutine(const gpiod::line_event& event) {
  int toRet=0;
  if (!(event.event_type == gpiod::line_event::RISING_EDGE
	|| event.event_type == gpiod::line_event::FALLING_EDGE)) {
    throw std::logic_error("invalid event type");
  }

  
  if(event.source.offset()==plusPIN) {
    toRet=10;
  } else {
    toRet=-10;
  }
  return toRet;
}

int main (int, char**argv) {
  int retval =0;
  try{
    gpiod::chip GPIO("pinctrl-bcm2835");
    FT1060M::PCF8591 i2c;
    i2c.setAnalogOutputEnabled(true);
    FT1060M::LedController LEDS(GPIO,i2c);
    gpiod::line Switch = GPIO.get_line(switchPIN);
    bool wait = true;
    if (!Switch.is_requested()){      
      Switch.request({
		      argv[0],
		      ::gpiod::line_request::DIRECTION_INPUT,
		      0
	});
    } else {
      throw "Cannot request the switch";
    }
    const int initialSwitchPosition = Switch.get_value();
    gpiod::line_bulk buttons = GPIO.get_lines({plusPIN,minusPIN});
    buttons.request({
		     argv[0],
		     //gpiod::line_request::EVENT_BOTH_EDGES,
		     gpiod::line_request::EVENT_RISING_EDGE,
		     0
      });
    int DACVAL  = 0;
    int counter = 0;
    while (Switch.get_value()==initialSwitchPosition) {
      auto events = buttons.event_wait(::std::chrono::seconds(1));
      if (events) {      
	for (auto& lineWithEvent: events) {
	  DACVAL+=eventRoutine(lineWithEvent.event_read());
	  if (++counter > 15){
	    counter = 0;
	  }
	}
	if (DACVAL>255) {
	  DACVAL=255;
	} else if (DACVAL<0) {
	  DACVAL=0;
	}

	LEDS.toggleLED1((1&counter)==1);
	LEDS.toggleLED2((2&counter)==2);
	LEDS.toggleLED3((4&counter)==4);
	LEDS.toggleLED4((8&counter)==8);
	
	LEDS.setAUXLedValue(DACVAL);
	/*for(auto test : {1,2,4,8}){
	  std::cout <<((counter & test)!=0);
	}
	std::cout 		  << std::endl;
	*/
      }   
    }
    LEDS.setAUXLedValue(0);
    std::this_thread::sleep_for (std::chrono::seconds(1));
  } catch (const char *problem) {
    std::cerr << "\033[1;31mERROR: \033[0m\033[1m" << problem << "\033[0m" << std::endl;
    retval = 1;
  } catch (const std::string &problem) {
    std::cerr << "\033[1;31mERROR: \033[0m\033[1m" << problem << "\033[0m" << std::endl;
    retval = 1;
  }
  return retval;
}

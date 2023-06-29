/**Partially copied from the examples of libgpiod
 */
#include <iostream>
#include <string>
#include <gpiod.hpp>
#include <chrono>
#include <thread>

#include "PCF8591onFT1060M.hpp"
#include "ledControllerForFT1060M.hpp"
#include "FT1060Mbase.hpp"

int  eventRoutine(const gpiod::line_event& event) {
  int toRet=0;
  if (!(event.event_type == gpiod::line_event::RISING_EDGE
	|| event.event_type == gpiod::line_event::FALLING_EDGE)) {
    throw std::logic_error("invalid event type");
  }

  
  if(event.source.offset()==FT1060M::pinMap::gpioButton1) {
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
    {
      bool enableAnalogInput=true;
      FT1060M::PCF8591::getPCF8591().setAnalogOutputEnabled(enableAnalogInput);
    }
    FT1060M::LedController LEDS(GPIO);
    gpiod::line Switch = GPIO.get_line(FT1060M::pinMap::gpioSwitch);
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
    gpiod::line_bulk buttons = GPIO.get_lines({FT1060M::pinMap::gpioButton1,FT1060M::pinMap::gpioButton2});
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
	  if (++counter > 31){
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
	LEDS.toggleLED5((16&counter)==16);
	
	LEDS.setAUXLedValue(DACVAL);
	std::cout << DACVAL << ", ";
	for(auto test : {1,2,4,8}){
	  std::cout <<((counter & test)!=0);
	}
	std::cout 		  << std::endl;
      }   
    }
    LEDS.toggleLED1(false);
    LEDS.toggleLED2(false);
    LEDS.toggleLED3(false);
    LEDS.toggleLED4(false);
    LEDS.toggleLED5(false);
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

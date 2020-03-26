#include <iostream>
#include <string>
#include <gpiod.hpp>
#include <chrono>
#include <thread>

#include "PCF8591onFT1060M.hpp"

constexpr unsigned int switchPIN = 17;
constexpr unsigned int plusPIN   = 27;
constexpr unsigned int minusPIN  = 22;

int  eventRoutine(const ::gpiod::line_event& event) {
  int toRet=0;
  if (event.event_type == ::gpiod::line_event::RISING_EDGE) {
    std::cout << "\nRISING EDGE";
  } else if (event.event_type == ::gpiod::line_event::FALLING_EDGE) {
    std::cout << "\nFALLING EDGE";
  } else {
    throw ::std::logic_error("invalid event type");
  }

  
  if(event.source.offset()==plusPIN) {
    toRet=10;
  } else {
    toRet=-10;
  }

  std::cout << ::std::endl;
  return toRet;
}

int main (int, char**argv) {
  int retval =0;
  try{
    gpiod::chip GPIO("pinctrl-bcm2835");
    FT1060M::PCF8591 i2c();
    i2c.setAnalogOutputEnabled(true);
    auto switch = GPIO.get_line(swithcPIN);
    bool wait = true;
    if (!switch.is_requested()){      
      switch.request({
		      argv[0],
		      ::gpiod::line_request::DIRECTION_INPUT,
		      0
	});
    } else {
      //security
      wait = false;
    }

    auto buttons = GPIO.get_lines({plusPIN,swithcPIN});
    buttons.request({
		     argv[0],
		     //gpiod::line_request::EVENT_BOTH_EDGES,
		     gpiod::line_request::EVENT_RISING_EDGE,
		     0
      });
    int DACVAL=0;
    while (wait) {
      auto event = button.event_wait(::std::chrono::seconds(1));
      if (event) {
	DACVAL+=eventRoutine();
	if (DACVAL>255) {
	  DACVAL=255;
	} else if (DACVAL<0) {
	  DACVAL=0;
	}
	i2c.writeToAnalogOut(DACVAL);
      }
      if(switch.get_value()==0) {
	wait = false;
      }
    
    }
  
  } catch (const char *problem) {
    std::cerr << "\033[1;31mERROR: \033[0m\033[1m" << problem << "\033[0m" << std::endl;
    retval = 1;
  } catch (const std::string &problem) {
    std::cerr << "\033[1;31mERROR: \033[0m\033[1m" << problem << "\033[0m" << std::endl;
    retval = 1;
  }
  return retval;
}

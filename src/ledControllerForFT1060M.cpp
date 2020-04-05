#include "ledControllerForFT1060M.hpp"
#include "FT1060Mbase.hpp"
#include "PCF8591onFT1060M.hpp"
#include <iostream>

namespace FT1060M {

  LedController::LedController(const ::gpiod::chip& gpioChip)
    : GPIOleds_(gpioChip.get_lines({pinMap::gpioLED1,pinMap::gpioLED2,pinMap::gpioLED3,pinMap::gpioLED4,pinMap::gpioLED5})) {
    GPIOleds_.request({
		       "LedController",
		       ::gpiod::line_request::DIRECTION_OUTPUT,
		       0
      });
  }
  
  LedController::~LedController(){
    GPIOleds_.release();
  }
  
  void LedController::setAUXLedValue(const int &value){
    //value is 0to255ed within the function
      PCF8591::getPCF8591().writeToAnalogOut(value);
  }
  void LedController::toggleLED1(const bool light){
    toggleLED(0,light);
  }
  void LedController::toggleLED2(const bool light){
    toggleLED(1,light);
  }
  void LedController::toggleLED3(const bool light){
    toggleLED(2,light);
  }
  void LedController::toggleLED4(const bool light){
    toggleLED(3,light);
  }
    void LedController::toggleLED5(const bool light){
    toggleLED(4,light);
  }
  void LedController::toggleLED(const int &pinNumber, const bool& light){
    auto values = GPIOleds_.get_values();
    values[pinNumber] = ((light)?1:0);
    GPIOleds_.set_values(values);
  }
  
}

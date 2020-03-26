#include "ledControllerForFT1060M.hpp"

namespace FT1060M {
  //redundant redeclaration for compatibility
  /*  constexpr int LedController::gpioLED1;
  constexpr int LedController::gpioLED2;
  constexpr int LedController::gpioLED3;
  constexpr int LedController::gpioLED4;*/

  LedController::LedController(const ::gpiod::chip& gpioChip, const PCF8591& i2c)
    : GPIOleds_(gpioChip.get_lines({gpioLED1,gpioLED2,gpioLED3,gpioLED4})),
      DAC_(i2c) {
    GPIOleds_.request({
		       "LedController",
		       ::gpiod::line_request::DIRECTION_OUTPUT,
		       0
      });
  }
  void LedController::setAUXLedValue(const int &value){
    //value is 0to255ed within the function
    DAC_.writeToAnalogOut(value);
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
  void LedController::toggleLED(const int &pinNumber, const bool& light){
    GPIOleds_.get(pinNumber).set_value((light)?1:0);
  }
  
}

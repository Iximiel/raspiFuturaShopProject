#include "ledControllerForFT1060M.hpp"

namespace FT1060M {

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
    toggleLED(gpioLED1,light);
  }
  void LedController::toggleLED2(const bool light){
    toggleLED(gpioLED2,light);
  }
  void LedController::toggleLED3(const bool light){
    toggleLED(gpioLED3,light);
  }
  void LedController::toggleLED4(const bool light){
    toggleLED(gpioLED4,light);
  }
  void LedController::toggleLED(const int &pinNumber, const bool& light){
    GPIOleds_.get(pinNumber).set_value((light)?1:0);
  }
  
}

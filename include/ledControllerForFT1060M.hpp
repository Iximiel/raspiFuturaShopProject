#ifndef LEDCONTROLLER_HPP
#define LEDCONTROLLER_HPP
#include <gpiod.hpp>

namespace FT1060M {
    
  class LedController {
  public:
    LedController(const ::gpiod::chip&);
    ~LedController();
    void setAUXLedValue(const int &);
    void toggleLED1(const bool);
    void toggleLED2(const bool);
    void toggleLED3(const bool);
    void toggleLED4(const bool);
    void toggleLED5(const bool);
  private:
    void toggleLED(const int &, const bool&);
    ::gpiod::line_bulk GPIOleds_;
  };
}
#endif // LEDCONTROLLER_HPP

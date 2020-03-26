#ifndef LEDCONTROLLER_HPP
#define LEDCONTROLLER_HPP
#include <gpiod.hpp>
namespace FT1060M {

  class LedController {
  public:
    LedController(const ::gpiod::chip&, const PCF8591&);
    static constexpr int gpioLED1 = 18;
    static constexpr int gpioLED2 = 23;
    static constexpr int gpioLED3 = 24;
    static constexpr int gpioLED4 = 25;
    void setAUXLedValue(const int &);
    void toggleLED1(const bool);
    void toggleLED2(const bool);
    void toggleLED3(const bool);
    void toggleLED4(const bool);
  private:
    void toggleLED(const int &, const bool);
    ::gpiod::line_bulk GPIOleds_;
    PCF8591 DAC_;
  };
}
#endif // LEDCONTROLLER_HPP

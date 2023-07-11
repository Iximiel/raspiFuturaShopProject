#ifndef FT1060MPARAMETERS
#define FT1060MPARAMETERS
namespace FT1060Mparameters {
  constexpr float Vin = 3.3f;
  constexpr float Vref = 3.3f;
  constexpr int adapterNumber = 1;
  constexpr int i2CAddress = 0x48;
  constexpr float K = 6.0f; // 6mV C
  constexpr float zeroKelvin = 273.15f;
  constexpr float R1 = 15000.0f;
  constexpr float Rth0 = 10000.0f;
  // parameters for Steinhart-Hart
  constexpr float sA = 0.00335402f;
  constexpr float sB = 0.000256985f;
  constexpr float sC = 2.62013e-6f;
  constexpr float sD = 6.38309e-8;
  // formula B
  constexpr float parB = 4100.0f;
  // lux sensor
  constexpr float Rlux0 = 70000.0f;
  constexpr float Pend = 0.7f;
} // namespace FT1060Mparameters
#endif // FT1060MPARAMETERS

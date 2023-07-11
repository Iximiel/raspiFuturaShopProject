#include "FT1060Mdata.hpp"
#include <cmath>

namespace FT1060Mdata {
  float VoltageFromADCVoltage (const long res) {
    return (res / 256.0f) * FT1060Mparameters::Vref;
  }

  float ResinstanceFromADCVoltage (const long res) {
    float V = VoltageFromADCVoltage (res);
    return (FT1060Mparameters::R1 * V) / (FT1060Mparameters::Vin - V);
  }
  float LuxFromFromADCVoltage (const long res) {
    float Rlux = ResinstanceFromADCVoltage (res);
    float Lux = std::pow (
      Rlux / FT1060Mparameters::Rlux0, 1.0 / -FT1060Mparameters::Pend);
    return Lux;
  }

  inline float Steinhart_HartT (float V, float Rth) {
    using FT1060Mparameters::K;
    using FT1060Mparameters::Rth0;
    using FT1060Mparameters::sA;
    using FT1060Mparameters::sB;
    using FT1060Mparameters::sC;
    using FT1060Mparameters::sD;
    using FT1060Mparameters::zeroKelvin;

    float lR = std::log (Rth / Rth0);
    float lRsq = lR * lR;
    float lRcb = lRsq * lR;
    // Steinhart-Hart
    float T = 1.0f / (sA + sB * lR + sC * lRsq + sD * lRcb);
    T = T - zeroKelvin - V * V / (K * Rth);
    return T;
  }

  float TempSHFromFromADCVoltage (const long res) {
    float V = VoltageFromADCVoltage (res);
    float Rth = ResinstanceFromADCVoltage (res);
    float T = Steinhart_HartT (V, Rth);
    return T;
  }

  inline float parameterBT (float V, float Rth) {
    // parameter B
    using FT1060Mparameters::K;
    using FT1060Mparameters::parB;
    using FT1060Mparameters::zeroKelvin;
    float lR = std::log (Rth / 10000.0f);
    float T2 = 1.0f / ((1.0f / 298.15f) + lR / parB);
    T2 = T2 - zeroKelvin - V * V / (K * Rth);
    return T2;
  }

  float TempBPFromFromADCVoltage (const long res) {
    float V = VoltageFromADCVoltage (res);
    float Rth = ResinstanceFromADCVoltage (res);
    float T = parameterBT (V, Rth);
    return T;
  }

  bothTemp TempFromFromADCVoltage (const long res) {
    float V = VoltageFromADCVoltage (res);
    float Rth = ResinstanceFromADCVoltage (res);
    float Steinhart_Hart = Steinhart_HartT (V, Rth);
    float parameterB = parameterBT (V, Rth);
    return {Steinhart_Hart, parameterB};
  }

} // namespace FT1060Mdata

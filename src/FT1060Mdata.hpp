#ifndef FT1060MDATA
#define FT1060MDATA
#include "parameters.hpp"

namespace FT1060Mdata {
  float VoltageFromADCVoltage (const long res);
  float LuxFromFromADCVoltage (const long res);

  float ResinstanceFromADCVoltage (const long res);
  float TempSHFromFromADCVoltage (const long res);

  float TempBPFromFromADCVoltage (const long res);

  struct bothTemp {
    float Steinhart_Hart;
    float parameterB;
  };
  bothTemp TempFromFromADCVoltage (const long res);

} // namespace FT1060Mdata

#endif // FT1060MDATA
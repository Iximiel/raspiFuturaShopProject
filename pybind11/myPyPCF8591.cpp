#include "FT1060Mdata.hpp"
#include "FT1060Mexcept.hpp"
#include "PCF8591onFT1060M.hpp"

#include <pybind11/pybind11.h>

namespace py = pybind11;

float getLux () {
  try {
    long res = FT1060M::PCF8591::getPCF8591 ().readFromAnalogChannel1 ();
    float Lux = FT1060Mdata::LuxFromFromADCVoltage (res);
    return Lux;
  } catch (FT1060MException &) {
    return -1.0;
  }
}

float getTempSH () {
  try {
    long res = FT1060M::PCF8591::getPCF8591 ().readFromAnalogChannel0 ();
    float Temp = FT1060Mdata::TempSHFromFromADCVoltage (res);
    return Temp;
  } catch (FT1060MException &) {
    return -1.0;
  }
}

float getTempBP () {
  try {
    long res = FT1060M::PCF8591::getPCF8591 ().readFromAnalogChannel0 ();
    float Temp = FT1060Mdata::TempBPFromFromADCVoltage (res);
    return Temp;
  } catch (FT1060MException &) {
    return -1.0;
  }
}

PYBIND11_MODULE (myPyPCF8591, m) {
  m.doc () = "pybind11 example plugin"; // optional module docstring

  m.def ("getLux", &getLux, "get a Lux reading");
  m.def (
    "getTempBP", &getTempBP, "get a Temperature reading using Steinhart Hart");
  m.def (
    "getTempSH", &getTempSH, "get a Temperature reading using Beta Parameter");
}
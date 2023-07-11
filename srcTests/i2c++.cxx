#include "FT1060Mdata.hpp"
#include "FT1060Mexcept.hpp"
#include "PCF8591onFT1060M.hpp"
#include "parameters.hpp"
#include <iostream>

#include <math.h>
#include <string>

int main (int, char **) {
  int retval = 0;
  try {
    // to initialize the device
    FT1060M::PCF8591::getPCF8591 ();

    {
      long res = FT1060M::PCF8591::getPCF8591 ().readFromAnalogChannel1 ();
      float Lux = FT1060Mdata::LuxFromFromADCVoltage (res);
      if (res < 0) {
        std::cerr << "Failed to read lux\n";
      } else {
        std::cout << "Lux: " << Lux << std::endl;
      }
    }
    {
      long res = FT1060M::PCF8591::getPCF8591 ().readFromAnalogChannel0 ();
      auto T = FT1060Mdata::TempFromFromADCVoltage (res);
      if (res < 0) {
        std::cerr << "Failed to read temperature\n";
      } else {
        std::cout << "Temperature (SH): " << T.Steinhart_Hart << std::endl;
        std::cout << "Temperature (Beta): " << T.parameterB << std::endl;
      }
    }
  } catch (const char *problem) {
    std::cerr << "\033[1;31mERROR: \033[0m\033[1m" << problem << "\033[0m"
              << std::endl;
    retval = 1;
  } catch (const std::string &problem) {
    std::cerr << "\033[1;31mERROR: \033[0m\033[1m" << problem << "\033[0m"
              << std::endl;
    retval = 1;
  } catch (FT1060MException &e) {
    std::cerr << "\033[1;31mERROR: \033[0m\033[1m" << e.what () << "\033[0m"
              << std::endl;
    retval = 1;
  }
  return retval;
}

#include <iostream>
#include "PCF8591onFT1060M.hpp"

#include <math.h>
#include <string>

constexpr float Vin = 3.3f;
constexpr float Vref = 3.3f;
constexpr int adapterNumber = 1;
constexpr int i2CAddress = 0x48;
constexpr float K = 6.0f;//6mV C
constexpr float zeroKelvin = 273.15f;
constexpr float R1 = 15000.0f;
constexpr float Rth0 =10000.0f;
//parameters for Steinhart-Hart
constexpr float sA = 0.00335402f;
constexpr float sB = 0.000256985f;
constexpr float sC = 2.62013e-6f;
constexpr float sD = 6.38309e-8;
//formula B
constexpr float parB = 4100.0f;
//lux sensor
constexpr float Rlux0 = 70000.0f;
constexpr float Pend = 0.7f;

float VoltageFromADCVoltage(const long& res) {
  return (res/256.0f) * Vref;
}

float ResinstanceFromADCVoltage(const long& res) {
  float V = VoltageFromADCVoltage(res);
  return  (R1*V) / (Vin - V);
}
// this is the c style thing
int main(int, char**) {
  int retval =0;
  try{
    //to initialize the device
    FT1060M::PCF8591::getPCF8591();
  
    {
      long res =     FT1060M::PCF8591::getPCF8591().readFromAnalogChannel1();
      float Rlux = ResinstanceFromADCVoltage(res);
      float Lux = std::pow(Rlux/Rlux0,1.0/-Pend);
      if(res <0 ){
	std::cerr << "Failed to read lux\n";
      } else {
	std::cout << "Lux: " << Lux <<std::endl;
      }
    }
    {

      long res =     FT1060M::PCF8591::getPCF8591().readFromAnalogChannel0();
      float V = VoltageFromADCVoltage(res);
      float Rth = ResinstanceFromADCVoltage(res);
      float lR = std::log(Rth/Rth0);
      float lRsq = lR*lR;
      float lRcb = lRsq*lR;
      float T = 1.0f/(sA + sB*lR + sC*lRsq + sD*lRcb);
      T = T-zeroKelvin - V*V / ( K*Rth);
      lR = std::log(Rth/10000.0f);
      float T2 = 1.0f/((1.0f/298.15f) + lR/parB);
      T2 = T2-zeroKelvin - V*V / ( K*Rth);
      if(res <0 ){
	std::cerr << "Failed to read temperature\n";
      } else {
	std::cout << "Temperature: " << T <<std::endl;
	std::cout << "Temperature: " << T2 <<std::endl;
      }
    }
  } catch (const char *problem) {
    std::cerr << "\033[1;31mERROR: \033[0m\033[1m" << problem << "\033[0m" << std::endl;
    retval = 1;
  } catch (const std::string &problem) {
    std::cerr << "\033[1;31mERROR: \033[0m\033[1m" << problem << "\033[0m" << std::endl;
    retval = 1;
  }
  return retval;
}

#include <iostream>

//C things
#include <linux/i2c-dev.h>
extern "C" {//remember ti include -li2c
#include <i2c/smbus.h>
}
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <cmath>

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
  std::string device = "/dev/i2c-";
  device+=std::to_string(adapterNumber);
  int file = open(device.c_str(), O_RDWR);
  if (file < 0) {
    /* ERROR HANDLING; you can check errno to see what went wrong */
    std::cerr << "failed to open file\n";
    exit(1);
  }
  
  if (ioctl(file, I2C_SLAVE, i2CAddress) < 0) {
    /* ERROR HANDLING; you can check errno to see what went wrong */
    std::cerr << "not i2c slave\n";
    exit(1);
  }
  //set registry

  //unsigned channelLux = 0x41;
  //unsigned channelTemp = 0x40;
  unsigned channelLux = 0x1;
  unsigned channelTemp = 0x0;
  {
    i2c_smbus_write_byte(file,channelLux);
    long res = i2c_smbus_read_byte(file);
    res = i2c_smbus_read_byte(file);
    res = i2c_smbus_read_byte(file);
    float Rlux = ResinstanceFromADCVoltage(res);
    float Lux = pow(Rlux/Rlux0,1.0/-Pend);
    if(res <0 ){
      std::cerr << "Failed to read lux\n";
    } else {
      std::cout << "Lux: " << Lux <<std::endl;
    }
  }
  {
    i2c_smbus_write_byte(file,channelTemp);
    long res = i2c_smbus_read_byte(file);
    res = i2c_smbus_read_byte(file);
    res = i2c_smbus_read_byte(file);
    float V = VoltageFromADCVoltage(res);
    float Rth = ResinstanceFromADCVoltage(res);
    float lR = log(Rth/Rth0);
    float lRsq = lR*lR;
    float lRcb = lRsq*lR;
    float T = 1.0f/(sA + sB*lR + sC*lRsq + sD*lRcb);
    T = T-zeroKelvin - V*V / ( K*Rth);
    lR = log(Rth/10000.0f);
    float T2 = 1.0f/((1.0f/298.15f) + lR/parB);
    T2 = T2-zeroKelvin - V*V / ( K*Rth);
    if(res <0 ){
      std::cerr << "Failed to read temperature\n";
    } else {
      std::cout << "Temperature: " << T <<std::endl;
      std::cout << "Temperature: " << T2 <<std::endl;
    }
  }
  return 0;
}

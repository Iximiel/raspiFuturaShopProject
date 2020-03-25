#include "myPCF8591.hpp"


//C things
#include <linux/i2c-dev.h>
extern "C" {//remember ti include -li2c
#include <i2c/smbus.h> }
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

myPCF8591::myPCF8591(){
   deviceFile_ = open(device.c_str(), O_RDWR);
  if (deviceFile_ < 0) {
    /* ERROR HANDLING; you can check errno to see what went wrong */
    throw string("failed to open device, code") + to_string(deviceFile_);
  }
  
  if (ioctl(deviceFile_, I2C_SLAVE, i2CAddress) < 0) {
    /* ERROR HANDLING; you can check errno to see what went wrong */
    throw "not i2c slave\n";
  } ;
}
int myPCF8591::readFromAnalogChannel0(const int& times=3){
  unsigned channel=0;
  if (AnalogOutputEnabled) {
    channel |= AnalogOutputEnableMask;
      }
    i2c_smbus_write_byte(deviceFile_,channel);
    i2c_smbus_read_byte(deviceFile_);
    i2c_smbus_read_byte(deviceFile_);
    int res = i2c_smbus_read_byte(deviceFile_);
    if (res < 0) {
      throw "error at reading channel 0";
    }
    return res;
}
int myPCF8591::readFromAnalogChannel1(const int& times=3){}
int myPCF8591::readFromAnalogChannel2(const int& times=3){}
int myPCF8591::readFromAnalogChannel3(const int& times=3){}
void myPCF8591::setAnalogOutputEnabled(const bool& flag) {
  AnalogOutputEnabled = flag;
}
bool myPCF8591::getAnalogOutputEnabled() const { return AnalogOutputEnabled;}

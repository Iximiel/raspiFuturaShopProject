#include "PCF8591onFT1060M.hpp"


//C things
#include <linux/i2c-dev.h>
extern "C" {//remember ti include -li2c
#include <i2c/smbus.h>
}
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <string>
namespace FT1060M {
  PCF8591::PCF8591(){
    std::string device = "/dev/i2c-";
    device+=std::to_string(adapterNumber);
    deviceFile_ = open(device.c_str(), O_RDWR);
    if (deviceFile_ < 0) {
      /* ERROR HANDLING; you can check errno to see what went wrong */
      throw std::string("failed to open device, code") + std::to_string(deviceFile_);
    }
  
    if (ioctl(deviceFile_, I2C_SLAVE, i2CAddress) < 0) {
      /* ERROR HANDLING; you can check errno to see what went wrong */
      throw "not i2c slave\n";
    } ;
  }
  int PCF8591::readFromAnalogChannel0(const int& times){
    return readFromAnalogChannel(0,times);
  }
  int PCF8591::readFromAnalogChannel1(const int& times){
    return readFromAnalogChannel(1,times);
  }
  int PCF8591::readFromAnalogChannel2(const int& times){
    return readFromAnalogChannel(2,times);
  }
  int PCF8591::readFromAnalogChannel3(const int& times){
    return readFromAnalogChannel(3,times);
  }

  int PCF8591::readFromAnalogChannel(const int& ch,const int& times){
    unsigned channel=unsigned(ch);
    if (AnalogOutputEnabled_) {
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
  void PCF8591::setAnalogOutputEnabled(const bool& flag) {
    AnalogOutputEnabled_ = flag;
  }
  bool PCF8591::getAnalogOutputEnabled() const { return AnalogOutputEnabled_;}

}

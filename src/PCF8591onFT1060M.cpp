#include "PCF8591onFT1060M.hpp"

// C things
extern "C" { // remember to include -li2c
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
}
#include <errno.h>
#include <fcntl.h>
#include <memory>
#include <string.h>
#include <string>
#include <sys/ioctl.h>

#include <iostream>

namespace FT1060M {
  PCF8591 &PCF8591::getPCF8591 () {
    static std::unique_ptr<PCF8591> instance (new PCF8591 ());
    return *instance;
  }

  PCF8591::PCF8591 () {
    std::string device_{"/dev/i2c-"};
    device_ += std::to_string (adapterNumber);
    deviceFile_ = open (device_.c_str (), O_RDWR);
    if (deviceFile_ < 0) {
      /* ERROR HANDLING; you can check errno to see what went wrong */
      throw std::string ("failed to open device, code") +
        std::to_string (deviceFile_);
    }

    if (ioctl (deviceFile_, I2C_SLAVE, i2CAddress) < 0) {
      /* ERROR HANDLING; you can check errno to see what went wrong */
      throw "not i2c slave\n";
    };
  }

  // PCF8591::~PCF8591()=default;

  int PCF8591::readFromAnalogChannel0 (const int times) {
    return readFromAnalogChannel (0, times);
  }
  int PCF8591::readFromAnalogChannel1 (const int times) {
    return readFromAnalogChannel (1, times);
  }
  int PCF8591::readFromAnalogChannel2 (const int times) {
    return readFromAnalogChannel (2, times);
  }
  int PCF8591::readFromAnalogChannel3 (const int times) {
    return readFromAnalogChannel (3, times);
  }

  int PCF8591::writeToAnalogOut (const int value) {
    // default set channel to 0, then writes
    unsigned t = value;
    if (t > 255) {
      t = 255;
    } else if (value < 0) {
      t = 0;
    }
    return i2c_smbus_write_byte_data (deviceFile_, AnalogOutputEnableMask, t);
  }

  int PCF8591::readFromAnalogChannel (const int ch, int times) {
    unsigned channel = unsigned (ch);
    // the channel are the lowest 2 bits
    if (AnalogOutputEnabled_) {
      channel |= AnalogOutputEnableMask;
    }
    int write_check = -1;
    do {
      write_check = i2c_smbus_write_byte (deviceFile_, channel);
      std::cout << "reding " << ch << " : " << write_check << " " << times
                << "\n";
      --times;
    } while (!write_check && times > 0);
    i2c_smbus_read_byte (deviceFile_);
    i2c_smbus_read_byte (deviceFile_);
    int res = i2c_smbus_read_byte (deviceFile_);
    if (res < 0) {
      throw "error at reading channel " + std::to_string (channel);
    }
    return res;
  }

  void PCF8591::setAnalogOutputEnabled (const bool flag) {
    AnalogOutputEnabled_ = flag;
  }
  bool PCF8591::getAnalogOutputEnabled () const { return AnalogOutputEnabled_; }

} // namespace FT1060M

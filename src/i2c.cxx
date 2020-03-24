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

constexpr int adapterNumber = 1;
constexpr int i2CAddress = 0x48;
/// thi is the c style thing
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

  unsigned channelLux = 0x41;
  unsigned channelTemp = 0x40;
  {
    i2c_smbus_write_byte(file,channelLux);
    long res = i2c_smbus_read_byte(file);
    res = i2c_smbus_read_byte(file);
    res = i2c_smbus_read_byte(file);
    if(res <0 ){
      std::cerr << "Failed to read lux\n";
    } else {
      std::cout << "Lux: " << res <<std::endl;
    }
  }
  {
    i2c_smbus_write_byte(file,channelTemp);
    long res = i2c_smbus_read_byte(file);
    res = i2c_smbus_read_byte(file);
    res = i2c_smbus_read_byte(file);
    if(res <0 ){
      std::cerr << "Failed to read temperature\n";
    } else {
      std::cout << "Temperature: " << res <<std::endl;
    }
  }
  return 0;
}

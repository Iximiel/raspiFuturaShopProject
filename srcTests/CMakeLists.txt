add_executable(i2cTest i2c.cxx)
TARGET_LINK_LIBRARIES(i2cTest i2c)

add_executable(i2cTest++ i2c++.cxx)
TARGET_LINK_LIBRARIES(i2cTest++ i2c FT1060M)

add_executable(gpioTest gpio.cxx)
TARGET_LINK_LIBRARIES(gpioTest gpiod gpiodcxx)

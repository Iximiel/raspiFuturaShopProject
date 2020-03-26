add_executable(i2cTest i2c.cxx)
TARGET_LINK_LIBRARIES(i2cTest i2c)

add_executable(i2cTest++ i2c++.cxx)
TARGET_LINK_LIBRARIES(i2cTest++ i2c FT1060M)

add_executable(gpioTest gpio.cxx)
TARGET_LINK_LIBRARIES(gpioTest gpiod gpiodcxx)

add_executable(gpioi2cTest gpio_i2c.cxx)
TARGET_LINK_LIBRARIES(gpioi2cTest i2c gpiod gpiodcxx FT1060M)

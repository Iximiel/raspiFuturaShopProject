#NB: the library i2c does not give a pkgconfig
add_executable(i2cTest i2c.cxx)
TARGET_LINK_LIBRARIES(i2cTest i2c)

add_executable(i2cTest++ i2c++.cxx)
TARGET_LINK_LIBRARIES(i2cTest++ i2c FT1060M FT1060Mdata)

add_executable(gpioTest gpio.cxx)
TARGET_LINK_LIBRARIES(gpioTest gpiod gpiodcxx)

add_executable(gpioi2cTest gpio_i2c.cxx)
TARGET_LINK_LIBRARIES(gpioi2cTest i2c gpiod gpiodcxx FT1060M)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(DBSimulationWithNoDB DBSimNoDB.cxx)
TARGET_LINK_LIBRARIES(DBSimulationWithNoDB i2c gpiod gpiodcxx FT1060M Threads::Threads)

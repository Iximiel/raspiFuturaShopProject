add_executable(i2cTest i2c.cxx)
TARGET_LINK_LIBRARIES(i2cTest i2c)

add_executable(i2cTest++ i2c++.cxx myPCF8591.cpp)
TARGET_LINK_LIBRARIES(i2cTest i2c)

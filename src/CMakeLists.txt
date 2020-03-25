add_library(FT1060M myPCF8591.cpp)

add_executable(i2cTest i2c.cxx)
TARGET_LINK_LIBRARIES(i2cTest i2c)

add_executable(i2cTest++ i2c++.cxx)
TARGET_LINK_LIBRARIES(i2cTest++ i2c FT1060M)

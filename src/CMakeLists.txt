add_library(FT1060M
    PCF8591onFT1060M.cpp
    ledControllerForFT1060M.cpp)

set_target_properties(FT1060M PROPERTIES PUBLIC_HEADER 
    FT1060Mbase.hpp
    ledControllerForFT1060M.hpp
    PCF8591onFT1060M.hpp)
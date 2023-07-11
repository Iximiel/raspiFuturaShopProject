find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG QUIET)
if (NOT pybind11_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        pybind11
  GIT_SHALLOW    TRUE
  GIT_REPOSITORY https://github.com/pybind/pybind11
  GIT_TAG        v2.10.4
  )
    FetchContent_MakeAvailable(pybind11)
 #   find_package(pybind11 CONFIG )
endif(NOT pybind11_FOUND)


pybind11_add_module(myPyPCF8591 myPyPCF8591.cpp)

target_link_libraries(myPyPCF8591 PUBLIC i2c FT1060M FT1060Mdata pybind11::module)

install(TARGETS myPyPCF8591 LIBRARY DESTINATION .)
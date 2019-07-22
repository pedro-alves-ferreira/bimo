cmake_minimum_required(VERSION 3.9)

project(bimo)

option(BIMO_BUILD_TESTS "If true, build the unit tests" OFF)

# -------------------------------------------------------------------------
# conan

# download automatically
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                 "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(CONANFILE conanfile.txt
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

# -------------------------------------------------------------------------
# GSL

add_library(GSL INTERFACE)
target_include_directories(GSL INTERFACE "third-party/GSL/include")

# -------------------------------------------------------------------------
# library

include(config/cmake/cpp-requirements.cmake)

bimo_set_cpp_properties()

add_subdirectory(cpp/lib)

# -------------------------------------------------------------------------
# tests
if(BIMO_BUILD_TESTS)
    enable_testing ()
	add_subdirectory(cpp/unit_tests)
    ParseAndAddCatchTests(bimo_unit_tests)
endif()

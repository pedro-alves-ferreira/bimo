cmake_minimum_required(VERSION 3.9)

include(../../config/cmake/cpp-requirements.cmake)

project(bimo)

file(GLOB_RECURSE bimo_source_files *.cpp *.h)
add_library(bimo STATIC ${bimo_source_files})
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${bimo_source_files})

target_include_directories(bimo
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        src
        )


if (BIMO_BUILD_PIC AND NOT WIN32)
    target_compile_options(bimo PRIVATE -fPIC)
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options(bimo PRIVATE
            /W4 /WX /std:c++17 /permissive-
            )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(bimo PRIVATE
            -Werror
            )
endif ()


target_link_libraries(bimo GSL
        CONAN_PKG::nlohmann_json
        SimpleAmqpClient
        )

bimo_set_target_cpp_properties()

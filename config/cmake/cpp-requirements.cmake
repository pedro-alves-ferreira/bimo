function(bimo_clear_default_cpp_properties)
    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") # Clang and AppleClang
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        bimo_clear_default_msvc_cpp_properties()
    endif()

    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} PARENT_SCOPE)
endfunction()

macro(bimo_set_target_cpp_properties)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )

    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") # Clang and AppleClang
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        bimo_set_target_gcc_cpp_properties()
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        bimo_set_target_msvc_cpp_properties()
    endif()

endmacro()

macro(bimo_clear_default_msvc_cpp_properties)
    # Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4] " "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    endif()
    if(CMAKE_CXX_FLAGS MATCHES "/WX- ")
        string(REGEX REPLACE "/WX-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    endif()
endmacro()

function(bimo_set_target_gcc_cpp_properties)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wshadow -Wnon-virtual-dtor
            -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wrestrict -Wnull-dereference
            -Wold-style-cast -Wuseless-cast -Wdouble-promotion
            -Wformat=2 -pedantic)
endfunction()

function(bimo_set_target_msvc_cpp_properties)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /permissive-)
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /IGNORE:4221" PARENT_SCOPE)
endfunction()

function(bimo_set_cpp_properties)
    bimo_clear_default_cpp_properties()
    if (MSVC_VERSION GREATER_EQUAL "1900")
        include(CheckCXXCompilerFlag)
        CHECK_CXX_COMPILER_FLAG("/std:c++17" _cpp_latest_flag_supported)
        if (_cpp_latest_flag_supported)
            add_compile_options("/std:c++17")
        endif()
    endif()
endfunction(bimo_set_cpp_properties)
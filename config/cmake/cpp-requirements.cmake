function(bimo_set_cpp_properties)
    bimo_clear_default_cpp_properties()
    if (MSVC_VERSION GREATER_EQUAL "1900")
        include(CheckCXXCompilerFlag)
        CHECK_CXX_COMPILER_FLAG("/std:c++17" _cpp_latest_flag_supported)
        if (_cpp_latest_flag_supported)
            add_compile_options("/std:c++17")
        endif ()
    endif ()
endfunction(bimo_set_cpp_properties)

macro(bimo_clear_default_cpp_properties)
    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") # Clang and AppleClang
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        bimo_clear_default_msvc_cpp_properties()
    endif ()

    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} PARENT_SCOPE)
endmacro()

macro(bimo_clear_default_msvc_cpp_properties)
    # Force to always compile with W4
    set(LOCAL_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    if (LOCAL_CMAKE_CXX_FLAGS MATCHES "/WX- ")
        string(REGEX REPLACE "/WX-" LOCAL_CMAKE_CXX_FLAGS "${LOCAL_CMAKE_CXX_FLAGS}")
    endif ()
    if (LOCAL_CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4] " "" LOCAL_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    endif ()

    set(LOCAL_CMAKE_CXX_FLAGS "${LOCAL_CMAKE_CXX_FLAGS}" PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS "${LOCAL_CMAKE_CXX_FLAGS}" PARENT_SCOPE)
endmacro()

macro(bimo_set_target_c_properties_no_warnings)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        bimo_set_target_gcc_c_properties_no_warnings()
    endif ()
endmacro()

macro(bimo_set_target_cpp_properties)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO
            )

    target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") # Clang and AppleClang
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        bimo_set_target_gcc_cpp_properties()
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        bimo_set_target_msvc_cpp_properties()
    endif ()

endmacro()

function(bimo_set_target_gcc_c_properties_no_warnings)
    set(MY_DEBUG_OPTIONS -O0 -g)
    target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
endfunction()

function(bimo_set_target_gcc_cpp_properties)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wshadow
            -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wrestrict -Wnull-dereference
            -Wdouble-promotion -Wno-unknown-pragmas
            -Wformat=2 -pedantic)

    if (NOT ${NO_WARNINGS_AS_ERRORS})
        target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
    endif ()

    set(MY_CXX_OPTIONS -Wnon-virtual-dtor -Wold-style-cast -Wno-useless-cast)
    target_compile_options(${PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${MY_CXX_OPTIONS}>")

    set(MY_DEBUG_OPTIONS -O0 -g)
    target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
endfunction()

function(bimo_set_target_msvc_cpp_properties)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /permissive-)
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /IGNORE:4221" PARENT_SCOPE)
endfunction()

project(bimo_unit_tests)

# -------------------------------------------------------------------------
# catch

# download automatically
if(NOT EXISTS "${CMAKE_BINARY_DIR}/ParseAndAddCatchTests.cmake")
    message(STATUS "Downloading ParseAndAddCatchTests.cmake from https://github.com/philsquared/Catch")
    file(DOWNLOAD "https://raw.githubusercontent.com/philsquared/Catch/master/contrib/ParseAndAddCatchTests.cmake"
                    "${CMAKE_BINARY_DIR}/ParseAndAddCatchTests.cmake")
endif()

include(${CMAKE_BINARY_DIR}/ParseAndAddCatchTests.cmake)

file(GLOB_RECURSE bimo_unit_tests_source_files *.cpp *.h)
add_executable(bimo_unit_tests ${bimo_unit_tests_source_files})
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${bimo_unit_tests_source_files})

target_link_libraries(bimo_unit_tests
    bimo
	CONAN_PKG::Catch
)

set_target_properties(bimo_unit_tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    )

target_compile_options(bimo_unit_tests PRIVATE 
    /W4 /WX /std:c++17 /permissive-
)

enable_testing()
ParseAndAddCatchTests(${PROJECT_NAME})

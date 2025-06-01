# Create a list of all test files ending with _Test.cpp
file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*Test.cpp)

# Create the test executable
add_executable(MazeGeneratorTests ${TEST_SOURCES})

# Include the src directory for header files
target_include_directories(MazeGeneratorTests PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link the MazeCore library and Google Test libraries
target_link_libraries(MazeGeneratorTests PRIVATE MazeCore gtest gtest_main)

# Add compiler flags for Debug and Release builds
target_compile_options(MazeGeneratorTests PRIVATE
    $<$<CONFIG:Debug>:-g -O0 -Wall -Wextra -Werror -Wshadow -Wsign-conversion -Wunreachable-code -Wuninitialized -Winit-self>
    $<$<CONFIG:Release>:-O3 -Wall -Wextra -Werror -Wshadow -Wsign-conversion -Wunreachable-code -Wuninitialized -Winit-self>
)

# Register tests with CTest
add_test(NAME MazeGeneratorUnitTests COMMAND MazeGeneratorTests)
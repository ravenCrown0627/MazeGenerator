# Collect all source files except main.cpp
file(GLOB SRC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
list(REMOVE_ITEM SRC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp) # Exclude main.cpp

# Create a library for the MazeGenerator core logic
add_library(MazeCore ${SRC_SOURCES})

# Include directories for the library
target_include_directories(MazeCore PUBLIC ${CMAKE_SOURCE_DIR}/src)

# Add the MazeGenerator executable
add_executable(MazeGenerator main.cpp)

# Link the MazeCore library to the MazeGenerator executable
target_link_libraries(MazeGenerator PRIVATE MazeCore)
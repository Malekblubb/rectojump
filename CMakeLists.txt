project(rectojump)
cmake_minimum_required(VERSION 2.8)

# collect files
file(GLOB_RECURSE SRC_LIST "./include/*" "./src/*")
include_directories("./include" "./external/mlk/include")

# set compiler flags
set(CMAKE_CXX_FLAGS "-std=c++11 -O3")
set(CMAKE_CXX_COMPILER "clang++")

add_executable(${PROJECT_NAME} ${SRC_LIST})

# link
# sfml
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
endif()

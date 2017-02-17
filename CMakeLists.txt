project(rectojump)
cmake_minimum_required(VERSION 2.8)

# collect files
file(GLOB_RECURSE SRC_LIST "./include/*" "./src/*")
include_directories("./include" "./external/mlk/include")

add_executable(${PROJECT_NAME} ${SRC_LIST})

# set compiler flags
set(CMAKE_CXX_FLAGS "-std=c++1z -O3 -L/usr/local/lib")
set(CMAKE_CXX_COMPILER "clang++")


# link
# zlib
find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES})
endif()

# sfml
#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.4.0 REQUIRED system window graphics network audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
endif()

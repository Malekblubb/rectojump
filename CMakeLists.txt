project(rectojump)
cmake_minimum_required(VERSION 2.8)

# collect files
file(GLOB_RECURSE SRC_LIST "./include/*" "./src/*")
include_directories("./include" "./external/mlk/include")

add_executable(${PROJECT_NAME} ${SRC_LIST})

# set compiler flags
set(CMAKE_CXX_FLAGS "-std=c++0x -stdlib=libc++ -O3")
set(CMAKE_CXX_COMPILER "clang++")


# link
#zlib
find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES})
endif()

# sfml
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
endif()

project(rectojump)
cmake_minimum_required(VERSION 3.9)

# collect files
file(GLOB_RECURSE SRC_LIST "./include/*" "./src/*")
include_directories("./include" "./external/mlk/include")

add_executable(${PROJECT_NAME} ${SRC_LIST})

#set(CMAKE_CXX_STANDARD "20")
#set(CMAKE_CXX_STANDARD_REQUIRED on)

# set compiler flags
set(CMAKE_CXX_FLAGS "-std=c++20 -stdlib=libc++ -O3  -Wl,--copy-dt-needed-entries")
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
find_package(SFML 2.5 REQUIRED system window graphics network audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system sfml-audio)
endif()

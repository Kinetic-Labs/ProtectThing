cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0167 NEW)
cmake_policy(SET CMP0144 NEW)
project(ProtectThing)

set(CMAKE_CXX_STANDARD 26)

find_package(Boost 1.89.0 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)

add_library(cJSON third_party/cJSON/cJSON.c)
target_include_directories(cJSON PUBLIC third_party/cJSON)

file(GLOB_RECURSE SOURCES "src/**/*.cpp")

add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE
        cJSON
        Boost::boost
        OpenSSL::SSL
        OpenSSL::Crypto
        CURL::libcurl
)

cmake_minimum_required(VERSION 3.10)
project(monumental-backend)

# Enable C++17 standard (Websocket++ does not support cpp23)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")

# Find (required by WebSocket++)
find_package(Boost REQUIRED system)
include_directories(${Boost_INCLUDE_DIRS})

# Find WebSocket++ (installed via package manager)
find_path(WEBSOCKETPP_INCLUDE_DIR websocketpp/config/asio_no_tls_client.hpp)
if (NOT WEBSOCKETPP_INCLUDE_DIR)
    message(FATAL_ERROR "WebSocket++ not found. Install it via your package manager.")
endif()

# Include libs
include_directories(external/jsoncpp/include)
include_directories(${WEBSOCKETPP_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link Boost libraries
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
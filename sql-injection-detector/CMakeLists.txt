cmake_minimum_required(VERSION 3.16)
project(SQLI_DETECTOR LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt6 + SQL + NETWORK
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql Network)

qt6_add_executable(detector_real 
    src/main_real.cpp
)

set_target_properties(detector_real PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    WIN32_EXECUTABLE ON
)

# LINK PostgreSQL + HTTP
target_link_libraries(detector_real PRIVATE 
    Qt6::Core 
    Qt6::Widgets 
    Qt6::Sql 
    Qt6::Network
)

install(TARGETS detector_real DESTINATION bin)
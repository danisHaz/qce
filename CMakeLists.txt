cmake_minimum_required(VERSION 3.10)
project(Platform)

set(SOURCES
    src/main/Qbit.cpp
    src/main/QbitSystem.cpp
    src/main/Matr.cpp
    src/main/Utils.cpp
    src/main/QubitEnv.cpp
    src/main/OperationGraph.cpp
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined")

add_executable(Platform ${SOURCES})

target_include_directories(Platform
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/include
    ${PROJECT_SOURCE_DIR}/src/libs
)
cmake_minimum_required(VERSION 3.10)
project(Platform)
include(CTest)

set(SOURCES
    src/main/Qbit.cpp
    src/main/QbitSystem.cpp
    src/main/Matr.cpp
    src/main/Utils.cpp
    src/main/QubitEnv.cpp
    src/main/OperationGraph.cpp
)

set(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS "-std=c++17")

add_executable(Platform ${SOURCES})
target_include_directories(Platform
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/include
    ${PROJECT_SOURCE_DIR}/src/libs
)

# ----------- Test Section -----------
add_executable(UtilsTest
    ${PROJECT_SOURCE_DIR}/src/tests/utils_test.cpp
)
add_executable(GatesTest
    ${PROJECT_SOURCE_DIR}/src/tests/gates_test.cpp
)
target_include_directories(UtilsTest 
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/include
    ${PROJECT_SOURCE_DIR}/src/libs
    ${PROJECT_SOURCE_DIR}/src/main
)
target_include_directories(GatesTest
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/include
    ${PROJECT_SOURCE_DIR}/src/libs
    ${PROJECT_SOURCE_DIR}/src/main
)
add_test(NAME utils_test COMMAND UtilsTest)
add_test(NAME gates_test COMMAND GatesTest)

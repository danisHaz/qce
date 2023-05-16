cmake_minimum_required(VERSION 3.10)
project(Platform)
include(CTest)

set(COMMON_SOURCES
    src/main/Qubit.cpp
    src/main/Matr.cpp
    src/main/Utils.cpp
    src/main/QubitEnv.cpp
    src/main/OperationGraph.cpp
    src/main/OperationArgs.cpp
)

set(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS "-std=c++17")

add_executable(Platform 
    ${COMMON_SOURCES}
    src/main/QbitSystem.cpp
)
target_include_directories(Platform
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/include
    ${PROJECT_SOURCE_DIR}/src/libs
)

# ----------- Test Section -----------
add_executable(UtilsTest)
target_sources(UtilsTest
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/tests/utils_test.cpp
    ${COMMON_SOURCES}
)

add_executable(GatesTest)
target_sources(GatesTest
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/tests/gates_test.cpp
    ${COMMON_SOURCES}
)

add_executable(QubitEnvTest)
target_sources(QubitEnvTest
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/tests/qubitenv_test.cpp
    ${COMMON_SOURCES}
)

target_include_directories(UtilsTest 
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/include
    ${PROJECT_SOURCE_DIR}/src/libs
)
target_include_directories(GatesTest
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/include
    ${PROJECT_SOURCE_DIR}/src/libs
)
target_include_directories(QubitEnvTest
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/include
    ${PROJECT_SOURCE_DIR}/src/libs
)
add_test(NAME utils_test COMMAND UtilsTest)
add_test(NAME gates_test COMMAND GatesTest)
add_test(NAME qubitenv_test COMMAND QubitEnvTest)
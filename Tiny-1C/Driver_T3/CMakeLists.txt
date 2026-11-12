# Tầng 3: Cấu hình tích hợp CMake cho Driver Camera nhiệt Tiny1C (ASIC-2121W) trên FreeRTOS SG2002.
cmake_minimum_required(VERSION 3.16.3)

project(thermal_tiny1c C)

set(CMAKE_C_STANDARD 99)

include_directories(include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(DEFINED TOP_DIR)
    include_directories(${TOP_DIR}/driver/spi/include)
    include_directories(${TOP_DIR}/driver/i2c/include)
    include_directories(${TOP_DIR}/hal/cv181x/spi/include)
    include_directories(${TOP_DIR}/hal/cv181x/i2c/include)
    include_directories(${TOP_DIR}/driver/rtos_cmdqu/include)
    include_directories(${TOP_DIR}/driver/common/include)
    include_directories(${TOP_DIR}/common/include)
    include_directories(${TOP_DIR}/common/include/riscv64)
    include_directories(${TOP_DIR}/hal/cv181x/config)
    include_directories(${TOP_DIR}/arch/riscv64/include)
    include_directories(${TOP_DIR}/kernel/include/riscv64)
    include_directories(${TOP_DIR}/../Source/include)
    include_directories(${TOP_DIR}/../Source/portable/GCC/RISC-V)
    include_directories(${TOP_DIR}/../Source/portable/GCC/RISC-V/chip_specific_extensions/thead_c906_rv64imafdc)
    include_directories(${TOP_DIR}/../Tracealyzer/include)
    include_directories(${TOP_DIR}/../Tracealyzer/config)
    include_directories(${CMAKE_INSTALL_INC_PREFIX}/arch)
    include_directories(${CMAKE_INSTALL_INC_PREFIX}/common)
    include_directories(${CMAKE_INSTALL_INC_PREFIX}/kernel)
endif()

file(GLOB _SOURCES "src/*.c")
file(GLOB _HEADERS "include/*.h")

add_library(${PROJECT_NAME} STATIC ${_SOURCES})

if(DEFINED TOP_DIR)
    install(TARGETS ${PROJECT_NAME} DESTINATION lib)
    install(FILES ${_HEADERS} DESTINATION include/task/thermal_tiny1c)
else()
    # Standalone build on Host
    add_executable(thermal_test ${_SOURCES})
    target_link_libraries(thermal_test pthread)
endif()

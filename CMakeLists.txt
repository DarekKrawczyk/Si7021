cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(si7021)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(example src/si7021.h src/si7021.cpp
        src/example.cpp
    )
# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(example pico_stdlib hardware_i2c)

pico_enable_stdio_usb(example 1)
pico_enable_stdio_uart(example 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(example)
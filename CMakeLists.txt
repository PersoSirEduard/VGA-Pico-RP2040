cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(vga_pico)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(vga_pico src/vga_pico.c)

include_directories(src)

target_link_libraries(vga_pico pico_stdlib hardware_pio hardware_dma)

pico_add_extra_outputs(vga_pico)
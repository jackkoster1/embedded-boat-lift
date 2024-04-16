cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(test_project C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD "pico_w")

pico_sdk_init()

message("Running makefsdata python script")
exec_program(COMMAND py makefsdata.py WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

add_executable(blc
  main.c
)
pico_enable_stdio_usb(blc 1)
pico_enable_stdio_uart(blc 1)
pico_add_extra_outputs(blc)

target_include_directories(blc PRIVATE ${CMAKE_CURRENT_LIST_DIR} )

target_link_libraries(blc 
  pico_cyw43_arch_lwip_threadsafe_background 
  pico_lwip_http
  pico_stdlib
  pico_cyw43_arch_none
  hardware_adc

  pico_btstack_ble
  pico_btstack_cyw43
  )
pico_btstack_make_gatt_header(blc PRIVATE "${CMAKE_CURRENT_LIST_DIR}/blc.gatt")

pico_add_extra_outputs(blc)

add_compile_options(blc PRIVATE -Wall)
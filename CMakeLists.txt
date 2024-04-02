cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(test_project C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD "pico_w")

pico_sdk_init()

add_executable(test
  main.c
)
pico_enable_stdio_usb(test 1)
pico_enable_stdio_uart(test 1)
pico_add_extra_outputs(test)

target_include_directories(test PRIVATE ${CMAKE_CURRENT_LIST_DIR} )

target_link_libraries(test 
  pico_cyw43_arch_lwip_threadsafe_background 
  pico_stdlib
  pico_cyw43_arch_none
  )
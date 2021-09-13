cmake_minimum_required(VERSION 3.1)


#Force arm-none-eabi-gcc compiler for cmake

#include(CMakeForceCompiler)
include(./arm-gcc.cmake)


#set(CMAKE_SYSTEM_NAME Generic)

#CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)
#CMAKE_FORCE_CXX_COMPILER(arm-none-eabi-g++ GNU)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(TARGET example-aes)
project(${TARGET} C CXX ASM)

if(NOT DEVICE)
	set(DEVICE stm32f051)
	set(FLASH_START 0x08000000)
endif()

set(USE_STLINK ON)



#include libopencm3

set(LIBOPENCM3_DIR ${CMAKE_SOURCE_DIR}/libopencm3)
add_custom_target(libopencm3 make -j WORKING_DIRECTORY ${LIBOPENCM3_DIR})
add_custom_target(libopencm3-clean make clean WORKING_DIRECTORY ${LIBOPENCM3_DIR})
include_directories(${LIBOPENCM3_DIR}/include ${CMAKE_SOURCE_DIR}/src /usr/lib/arm-none-eabi/include)
link_directories(${LIBOPENCM3_DIR}/lib /usr/lib/arm-none-eabi/newlib/armv7e-m/fpu)
add_definitions(-DSTM32F0)
set(LIBOPENCM3_LIB opencm3_stm32f0)


set(STM32F0_FLAGS "-mfloat-abi=soft -mthumb -mcpu=cortex-m0 -Wextra -Wimplicit-function-declaration -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes -Wundef -Wshadow -fno-common")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall ${STM32F0_FLAGS} -std=c11 -g -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall --std=c++14 ${STM32F0_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${PROJECT_SOURCE_DIR}/linker/libopencm3_stm32f0.ld -nostartfiles --specs=rdimon.specs -g")

function(add_bin_from_elf bin elf)
	add_custom_target(${bin} ALL ${CMAKE_OBJCOPY} -Obinary ${elf} ${bin} DEPENDS ${elf})
endfunction(add_bin_from_elf)

function(add_lst_from_elf lst elf)
	add_custom_target(${lst} ALL ${CMAKE_OBJDUMP} -S ${elf} > ${lst} DEPENDS ${elf})
endfunction(add_lst_from_elf)

#file(GLOB Source RELATIVE "src" "*.c")



add_executable(${TARGET}.elf "")

target_sources(${TARGET}.elf
  PUBLIC
  "src/main.c"
  "src/aesTest.c"
  "src/aesTest.h"
  "src/usart1.c"
  "src/usart1.h"
)

target_link_libraries(${TARGET}.elf ${LIBOPENCM3_LIB} tiny-aes c m nosys)
add_bin_from_elf(${TARGET}.bin ${TARGET}.elf)
add_lst_from_elf(${TARGET}.lst ${TARGET}.elf)

set(BINARY ${TARGET}.bin)

add_subdirectory(src/tiny-AES-c)

add_custom_target(flash 
	COMMAND st-flash write ${BINARY} ${FLASH_START}
	DEPENDS ${TARGET}.bin
	)


add_custom_target(debug-server 
	COMMAND st-util --listen_port 4242
	DEPENDS ${TARGET}.elf
	)

add_custom_target(debug 
	COMMAND ${DEBUGGER} -command ${CMAKE_CURRENT_LIST_DIR}/remote.gdbconf ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.elf 
	DEPENDS ${TARGET}.elf
	)
add_dependencies(${TARGET}.elf tiny-aes libopencm3)

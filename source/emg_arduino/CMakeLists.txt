cmake_minimum_required(VERSION 2.8.9)

# Use AVR GCC toolchain
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_ASM_COMPILER avr-gcc)
set(OBJCOPY avr-objcopy)
set(STRIP avr-strip)

set(APP_NAME avr)
project("Avr Project")

# The upload to the target is currently not included in the make process
#set(PORT /dev/ttyACM0)
#set(PROG_TYPE arduino)

set(MCU atmega328p)

set(CMAKE_EXE_LINKER_FLAGS -mmcu=${MCU})

add_compile_options(
    -mmcu=${MCU} # MCU
    -std=gnu99 # C99 standard
    -Os
    -Wall
    -Wno-main
    -Wundef
    -Wstrict-prototypes
    -Werror
    -Wfatal-errors
    -Wl,--relax,--gc-sections
    -g
    -gdwarf-2
    -funsigned-char
    -funsigned-bitfields
    -fpack-struct
    -fshort-enums
    -ffunction-sections
    -fdata-sections
    -fno-split-wide-types
    -fno-tree-scev-cprop
)

include_directories("src")
file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(${APP_NAME} ${SOURCES})
set_target_properties(${APP_NAME} PROPERTIES OUTPUT_NAME ${APP_NAME}.elf)

add_custom_target(strip ALL ${STRIP} ${APP_NAME}.elf DEPENDS ${APP_NAME})
add_custom_target(hex ALL  ${OBJCOPY} -j .text -j .data -O ihex ${APP_NAME}.elf ${APP_NAME}.hex DEPENDS strip)
#add_custom_target(upload_hex ALL avrdude -P${PORT} -c${PROG_TYPE} -p${MCU} -Uflash:w:${APP_NAME}.hex:i  -V -v)

target_link_libraries(${APP_NAME} m)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${APP_NAME}.hex")

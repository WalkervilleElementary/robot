cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

# Sanity check our source directory to make sure that we are not trying to
# generate an in-tree build. I borrowed this from LLVM.
if( CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR )
  message(FATAL_ERROR "In-source builds are not allowed.
Please create a directory and run cmake from there, passing the path
to this source directory as the last argument.
This process created the file `CMakeCache.txt' and the directory `CMakeFiles'.
Please delete them.")
endif()

set(PROJECT_NAME MagicSchoolBot)

set(MCU   atmega128)
set(F_CPU 16000000)

set(PROG_CHIP m128)
set(PROG_BAUD 115200)
set(PROG_TYPE wiring)
include(programmer_config.cmake)

set(AVRCPP   avr-g++)
set(AVRC     avr-gcc)
set(AVRSTRIP avr-strip)
set(OBJCOPY  avr-objcopy)
set(OBJDUMP  avr-objdump)
set(AVRSIZE  avr-size)
set(AVRDUDE  avrdude)

add_definitions(-DF_CPU=${F_CPU})

set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_CXX_COMPILER ${AVRCPP})
set(CMAKE_C_COMPILER   ${AVRC})
set(CMAKE_ASM_COMPILER   ${AVRC})

project(MagicSchoolBot C CXX ASM)

# Compiler flags
set(CSTANDARD "-std=c99")
set(CXXSTANDARD "-std=c++11")
set(CDEBUG    "-gstabs -g -ggdb")
set(CWARN     "-Wall -Wstrict-prototypes -Wl,--gc-sections -Wl,--relax")
set(CXXWARN     "-Wall -Wl,--gc-sections -Wl,--relax")
set(CTUNING   "-funsigned-char -funsigned-bitfields -fshort-enums -ffunction-sections -fdata-sections")
set(COPT      "-O1 -lm -lprintf_flt")
set(CMCU      "-mmcu=${MCU}")
set(CDEFS     "-DF_CPU=${F_CPU}")

set(CFLAGS   "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CWARN} ${CSTANDARD} ${CTUNING}")
set(CXXFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CXXWARN} ${CXXSTANDARD} ${CTUNING}")

set(CMAKE_C_FLAGS          "${CFLAGS}")
set(CMAKE_CXX_FLAGS        "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS   "${CFLAGS}")

set(CORE_PATH "cores/arduino")
set(VARIANT_PATH "variants/TINAH")
file(GLOB CORE_SRC "${CORE_PATH}/*.cpp" "${CORE_PATH}/*.c" "${CORE_PATH}/*.s")
file(GLOB CORE_INC "${CORE_PATH}/*.h")
file(GLOB VARIANT_INC "${VARIANT_PATH}/*.h")

include_directories(${CORE_PATH} ${VARIANT_PATH})
add_library(ArduinoCore ${CORE_SRC} ${CORE_INC} ${VARIANT_INC})
set_target_properties(ArduinoCore PROPERTIES OUTPUT_NAME ArduinoCore)

set(LIB_SRC)
set(LIB_INC)
set(LIB_INC_PATH)
file(GLOB LIBRARIES "libraries/*")
foreach(subdir ${LIBRARIES})
    if(IS_DIRECTORY ${subdir})
        file(GLOB lib_src_files "${subdir}/*.cpp" "${subdir}/*.c" "${subdir}/src/*.cpp" "${subdir}/src/*.c")
        file(GLOB lib_inc_files "${subdir}/*.h" "${subdir}/inc/*.h")
            list(APPEND LIB_INC_PATH  "${subdir}")
            list(APPEND LIB_INC_PATH  "${subdir}/src")
        list(APPEND LIB_SRC "${lib_src_files}")
        list(APPEND LIB_INC "${lib_inc_files}")
    endif()
endforeach()

include_directories(${LIB_INC_PATH})
add_library(ArduinoLibraries ${LIB_SRC} ${LIB_INC})
target_link_libraries(ArduinoLibraries ArduinoCore)
set_target_properties(ArduinoLibraries PROPERTIES OUTPUT_NAME ArduinoLibraries)


set(INC_PATH     "include")
set(SRC_PATH     "src")
file(GLOB_RECURSE MAIN_SRC  "${SRC_PATH}/*.cpp"
                    "${SRC_PATH}/*.c")
file(GLOB_RECURSE MAIN_INC "${INC_PATH}/*.h")
                    
include_directories(${INC_PATH})

add_executable(${PROJECT_NAME} ${MAIN_SRC})
target_link_libraries(${PROJECT_NAME} ArduinoCore ArduinoLibraries)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

add_custom_target(strip ALL     ${AVRSTRIP} "${PROJECT_NAME}.elf" DEPENDS ${PROJECT_NAME})
add_custom_target(hex   ALL     ${OBJCOPY} -R .eeprom -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.hex" DEPENDS strip)
add_custom_target(flash         ${AVRDUDE} -c ${PROG_TYPE} -p ${PROG_CHIP} -P ${PROG_PORT} -b ${PROG_BAUD} -U flash:w:${PROJECT_NAME}.hex DEPENDS hex)
add_custom_target(size  ALL     ${AVRSIZE} -A "${PROJECT_NAME}.elf" DEPENDS strip)

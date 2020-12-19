cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(CMAKE_CXX_STANDARD 98)
set(PROJECT_NAME FlightControl)

set(${PROJECT_NAME}_BOARD megaADK)
# set(ARDUINO_CPU)
project(${PROJECT_NAME})

# Define additional source and header files or default arduino sketch files
include_directories(${PROJECT_SOURCE_DIR})
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB HDR_FILES ${PROJECT_SOURCE_DIR}/src/*.h)

set(PROJ_SRC ${SRC_FILES} FlightControl.cpp)
set(PROJ_HDR ${HDR_FILES})

### Additional static libraries to include in the target.
# set(${PROJECT_NAME}_LIBS)

### Main sketch file
#set(${PROJECT_NAME}_SKETCH FlightControl.ino)

### Add project directories into the build
#add_subdirectory(src)

### Additional settings to add non-standard or your own Arduino libraries.
# For this example (libs will contain additional arduino libraries)
# An Arduino library my_lib will contain files in libs/my_lib/: my_lib.h, my_lib.cpp + any other cpp files
#link_directories(${PROJECT_SOURCE_DIR}/src)
#link_directories(${PROJECT_SOURCE_DIR}/src)
# For nested library sources replace ${LIB_NAME} with library name for each library
#set(_RECURSE true)

#### Additional settings for programmer. From programmers.txt
set(${PROJECT_NAME}_PROGRAMMER avrisp)
# set(${PROJECT_NAME}_PORT /dev/cu.usbserial-00000000)
# set(megaADK.upload.speed 9600)

## Verbose build process
# set(${PROJECT_NAME}_AFLAGS -v)

#generate_arduino_firmware(${PROJECT_NAME})
generate_arduino_firmware(${CMAKE_PROJECT_NAME}
        SRCS ${PROJ_SRC}
        HDRS ${PROJ_HDR}
        )


print_board_list()
print_programmer_list()

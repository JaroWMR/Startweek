# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(blinky)


# Gather source files using wildcards for src directory
file(GLOB SRC_SOURCES 			"src/*.c")
file(GLOB SRC_ACTUATORS_SOURCES "src/actuators/*.c")
file(GLOB SRC_MINIGAMES_SOURCES "src/minigames/*.c")
file(GLOB SRC_SENSORS_SOURCES 	"src/sensors/*.c")
file(GLOB SRC_SENSORS_SOURCES 	"src/hardware/*.c")

# Add your source files
target_sources(app PRIVATE 
    ${SRC_SOURCES}
    ${SRC_ACTUATORS_SOURCES}
	${SRC_MINIGAMES_SOURCES}
	${SRC_SENSORS_SOURCES}
)

# Add include directories
target_include_directories(app PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/actuators
	${CMAKE_CURRENT_SOURCE_DIR}/inc/minigames
	${CMAKE_CURRENT_SOURCE_DIR}/inc/sensors
	${CMAKE_CURRENT_SOURCE_DIR}/inc/hardware
)
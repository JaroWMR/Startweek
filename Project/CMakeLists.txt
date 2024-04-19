# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(Startweek)


# Gather source files using wildcards for src directory
file(GLOB SRC_MAIN_SOURCES 				"main/src/*.c")
file(GLOB SRC_HARDWARE_SOURCES 			"hardware/src/*.c")
file(GLOB SRC_MINIGAMES_SOURCES 		"minigames/src/*.c")
file(GLOB SRC_HARDWARECONTROL_SOURCES 	"hardwareControl/src/*.c")

# Add your source files
target_sources(app PRIVATE 
    ${SRC_MAIN_SOURCES}
	${SRC_HARDWARE_SOURCES}
	${SRC_MINIGAMES_SOURCES}
	${SRC_HARDWARECONTROL_SOURCES}
)

# Add include directories
target_include_directories(app PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main/inc
	${CMAKE_CURRENT_SOURCE_DIR}/hardware/inc
	${CMAKE_CURRENT_SOURCE_DIR}/minigames/inc
	${CMAKE_CURRENT_SOURCE_DIR}/hardwareControl/inc
)
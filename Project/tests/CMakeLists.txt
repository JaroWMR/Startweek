# The following code is stolen from: samples/subsys/testsuite/integration
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(startweek)

FILE(GLOB app_sources src/*.c)
file(GLOB SRC_ACTUATORS_SOURCES "../src/actuators/*.c")
file(GLOB SRC_MINIGAMES_SOURCES "../src/minigames/*.c")
file(GLOB SRC_SENSORS_SOURCES 	"../src/sensors/*.c")
target_sources(app PRIVATE
	${app_sources}
	${SRC_ACTUATORS_SOURCES}
	${SRC_MINIGAMES_SOURCES}
	${SRC_SENSORS_SOURCES}
	../src/walking.c
	../src/statemachine.c
)

target_include_directories(app PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/../inc
)
#!/bin/bash

# Remove the result directory if it exists
rm -rf $STARTWEEK_ROOT/Project/tests/results
#exec $STARTWEEK_ROOT/../zephyr/scripts/twister -O $STARTWEEK_ROOT/Project/tests/results -T $STARTWEEK_ROOT/Project/tests/

# To run test on hardware:
OUTPUT_DIR=$STARTWEEK_ROOT/Project/tests/results
PLATFORM=nucleo_f091rc
west twister --device-testing --device-serial /dev/ttyACM0 --device-serial-baud 115200 -p $PLATFORM -O $OUTPUT_DIR -T $STARTWEEK_ROOT/../zephyr/samples/hello_world --west-flash --west-runner jlink
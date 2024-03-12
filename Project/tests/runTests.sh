#!/bin/bash

# Remove the result directory if it exists
rm -rf $STARTWEEK_ROOT/Project/tests/results
exec $STARTWEEK_ROOT/../zephyr/scripts/twister -O $STARTWEEK_ROOT/Project/tests/results -T $STARTWEEK_ROOT/Project/tests/
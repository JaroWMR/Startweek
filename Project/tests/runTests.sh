#!/bin/bash
PROJECT_ROOT="/home/siem/startweek/zephyrproject/Startweek"
echo "$PROJECT_ROOT/Project/tests/" 
exec $PROJECT_ROOT/../zephyr/scripts/twister -O $PROJECT_ROOT/Project/tests/results -T $PROJECT_ROOT/Project/tests/
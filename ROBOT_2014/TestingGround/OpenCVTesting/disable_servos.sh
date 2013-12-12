#!/bin/bash

echo 0 > /sys/devices/ocp.3/pwm_test_P8_13.15/run
echo 0 > /sys/devices/ocp.3/pwm_test_P9_14.16/run
echo 1285000 > /sys/devices/ocp.3/pwm_test_P9_21.17/duty

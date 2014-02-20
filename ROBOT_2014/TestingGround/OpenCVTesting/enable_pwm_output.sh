#!/bin/bash

echo am33xx_pwm > /sys/devices/bone_capemgr.9/slots
echo bone_pwm_P8_13 > /sys/devices/bone_capemgr.9/slots
# /sys/devices/ocp.3/pwm_test_P8_13.15
echo 20000000 > /sys/devices/ocp.3/pwm_test_P8_13.15/period
echo 10000000 > /sys/devices/ocp.3/pwm_test_P8_13.15/duty
echo 1 > /sys/devices/ocp.3/pwm_test_P8_13.15/run


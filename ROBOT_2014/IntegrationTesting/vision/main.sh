#!/bin/bash

# set up the camera
rmmod uvcvideo
modprobe uvcvideo nodrop=1 timeout=5000 quirks=0x80
v4l2-ctl --set-fmt-video=width=320,height=240,pixelformat=YUYV
v4l2-ctl -d /dev/video0 -p 30 > /dev/null
v4l2-ctl -d /dev/video0 -c white_balance_automatic=1
v4l2-ctl -d /dev/video0 -c gain_automatic=1
v4l2-ctl -d /dev/video0 -c auto_exposure=0
v4l2-ctl -d /dev/video0 -c exposure=100

# set up the aiming and firing pwm hardware
echo am33xx_pwm > /sys/devices/bone_capemgr.9/slots
echo bone_pwm_P8_13 > /sys/devices/bone_capemgr.9/slots
echo bone_pwm_P9_14 > /sys/devices/bone_capemgr.9/slots
echo bone_pwm_P9_21 > /sys/devices/bone_capemgr.9/slots

# execute the main program loop
./main

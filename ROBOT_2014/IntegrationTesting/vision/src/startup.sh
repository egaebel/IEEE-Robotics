#!/bin/bash

#put something in /tmp/ so we can see if this ran
touch /tmp/WINNING
#turn off the heartbeat
echo 0 > /sys/class/leds/beaglebone\:green\:usr0/brightness

#set up the hardware
#rmmod uvcvideo
#modprobe uvcvideo nodrop=1 timeout=5000 quirks=0x80
v4l2-ctl --set-fmt-video=width=640,height=480,pixelformat=MJPEG
v4l2-ctl -d /dev/video0 -p 5 > /dev/null
#v4l2-ctl -d /dev/video0 -c white_balance_automatic=1
#v4l2-ctl -d /dev/video0 -c gain_automatic=1
#v4l2-ctl -d /dev/video0 -c auto_exposure=0
#v4l2-ctl -d /dev/video0 -c exposure=100
echo am33xx_pwm > /sys/devices/bone_capemgr.9/slots
echo bone_pwm_P8_13 > /sys/devices/bone_capemgr.9/slots
echo bone_pwm_P9_14 > /sys/devices/bone_capemgr.9/slots
echo bone_pwm_P9_21 > /sys/devices/bone_capemgr.9/slots

#execute our program
/usr/local/bin/main

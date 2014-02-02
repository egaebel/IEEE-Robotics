#!/bin/bash
rmmod uvcvideo
modprobe uvcvideo nodrop=1 timeout=5000 quirks=0x80
v4l2-ctl --set-fmt-video=width=320,height=240,pixelformat=YUYV
v4l2-ctl -d /dev/video0 -p 30 > /dev/null
./snapshot

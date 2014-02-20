#!/bin/bash
rmmod uvcvideo
#modprobe uvcvideo nodrop=1 timeout=5000 quirks=0x80
modprobe uvcvideo
#v4l2-ctl --set-fmt-video=width=640,height=480,pixelformat=MJPEG
v4l2-ctl -d /dev/video0 -p 5 > /dev/null
./snapshot

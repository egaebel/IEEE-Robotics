#put something in /tmp/ so we can see if this ran
touch /tmp/WINNING
#turn off the heartbeat
echo 0 > /sys/class/leds/beaglebone\:green\:usr0/brightness
#execute our program
./bin/main


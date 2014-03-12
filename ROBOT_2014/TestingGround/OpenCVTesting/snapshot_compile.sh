g++ snapshot.cpp -march=armv7-a -mtune=cortex-a8 -ffast-math -O3 -mfpu=neon -mfloat-abi=hard -Wall -W -o bin/multi_snapshots -lopencv_core -lopencv_imgproc -lopencv_objdetect -lopencv_highgui

echo "creating bin directory"
if [[ ! -e bin ]]; then
    mkdir bin
elif [[ ! -d bin ]]; then
    echo "error: bin already exists but is not a directory" 1>&2
fi

echo "compiling"

g++ -DDEBUG -Wall -march=armv7-a -mtune=cortex-a8 -ffast-math -O3 -mfpu=neon -mfloat-abi=hard -ggdb `pkg-config --cflags opencv` -o bin/main main.cpp gpio.cpp hardware.cpp servo.cpp target.cpp `pkg-config --libs opencv`;

echo "done"

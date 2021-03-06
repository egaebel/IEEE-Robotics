echo "checking bin directory"
if [[ ! -e bin ]]; then
    mkdir bin
elif [[ ! -d bin ]]; then
    echo "bin already exists but is not a directory" 1>&2
fi

echo "compiling $1"
if [[ $1 == *.c ]]
then
    gcc -DDEBUG -Wall -march=armv7-a -mtune=cortex-a8 -ffast-math -O3 -mfpu=neon -mfloat-abi=hard -ggdb `pkg-config --cflags opencv` -o bin/`basename $1 .c` $1 `pkg-config --libs opencv`;
elif [[ $1 == *.cpp ]]
then
    g++ -DDEBUG -Wall -march=armv7-a -mtune=cortex-a8 -ffast-math -O3 -mfpu=neon -mfloat-abi=hard -ggdb `pkg-config --cflags opencv` -o bin/`basename $1 .cpp` $1 `pkg-config --libs opencv`;
else
    echo "Please compile only .c or .cpp files"
fi
echo "Output file => ${1%.*}"

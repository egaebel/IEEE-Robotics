echo "compiling $1"
if [[ $1 == *.c ]]
then
    gcc -DDEBUG -ggdb `pkg-config --cflags opencv -march=armv7-a -mtune=cortex-a8 -ffast-math -O3 -mfpu=neon -mfloat-abi=hard` -o `basename $1 .c` $1 `pkg-config --libs opencv`;
elif [[ $1 == *.cpp ]]
then
    g++ -DDEBUG -Wall -ggdb `pkg-config --cflags opencv -march=armv7-a -mtune=cortex-a8 -ffast-math -O3 -mfpu=neon -mfloat-abi=hard` -o `basename $1 .cpp` $1 `pkg-config --libs opencv`;
else
    echo "Please compile only .c or .cpp files"
fi
echo "Output file => ${1%.*}"


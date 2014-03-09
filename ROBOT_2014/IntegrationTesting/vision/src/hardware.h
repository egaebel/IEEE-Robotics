#ifndef HARDWARE_H
#define HARDWARE_H

bool write_hardware_fs( const char* path, const char* value, const int length, const char* mode = "rb+" );

bool read_hardware_fs( const char* path, char* value, unsigned int length, const char* mode );

#endif

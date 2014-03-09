/**
 * @file hardware
 * @brief embedded hardware control via the filesystem
 * @author D. Butenhoff
 */

#include <cstring>
#include <cstdio>
#include "hardware.h"

bool write_hardware_fs( const char* path, const char* value, const int length, const char* mode )
{
  FILE *myOutputHandle = NULL;
  if( ( myOutputHandle = fopen( path, mode ) ) == NULL ) {
    printf( "Unable to open %s\n", path );
    return false;
  }
  fwrite( value, sizeof(char), length, myOutputHandle );
  fclose( myOutputHandle );
  return true;
}

bool read_hardware_fs( const char* path, char* value, int length, const char* mode )
{
  FILE *myInputHandle = NULL;
  if( ( myInputHandle = fopen( path, mode ) ) == NULL ) {
    printf( "Unable to open %s\n", path );
    return false;
  }
  bool failed = false;
  if ( fread( value, sizeof(char), length, myInputHandle ) != length )
    failed = true;
  fclose( myInputHandle );
  return !failed;
}

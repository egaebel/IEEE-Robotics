#ifndef SERVO_H
#define SERVO_H

bool write_servo_fs( const char* path, const char* value, const int length );
bool enable_servo( const char* servo );
bool disable_servo( const char* servo );
bool set_servo_position( const char* servo, int position );
bool check_servo_range( const char* servo, int min_position, int ctr_position, int max_position );

#endif


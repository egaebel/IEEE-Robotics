#ifndef _COMMON_H_
#define _COMMON_H_

typedef enum CmuColorT {	
	WHITE=0,//LEAVE WHITE AS 0
	YELLOW,
	RED,
	PURPLE,
	BLUE,
	BROWN,
	GREEN,
	BLACK,//here so we can iterate white to black
	EVERYTHING // HERE to evaluate anything in the cmu cam
} CmuColor;

#endif

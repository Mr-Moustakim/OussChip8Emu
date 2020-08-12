/**********************************************************
 * File name: chip8memory.c                               *
 * Version: 1.0                                           *
 * Author: Oussama Moustakim                              *
 * Date: 08/08/2020 <During COVID-19 lockDown>            *
 * Description : In this module we create functions that  *
 *               set values into memory or retrieve them  * 
 **********************************************************/
#include "chip8header.h"
#include <assert.h>
/* This function sets the memory location */
void set_chip8_memory(struct chip8_memory* pointerToMemo_Ptr, int indexToArray, unsigned char value)
{
	/* If the location to be accessed is beyond the size of the memory throw a run-time error and stop the program */
	assert(indexToArray >= 0 && indexToArray < MEMORY_SIZE);
	/* Set the desired memory location to value */
	pointerToMemo_Ptr->memory[indexToArray] = value;
}

/* This function retrieves values from memory */
unsigned char get_chip8_memory(struct chip8_memory* pointerToMemo_Ptr, int indexToArray)
{
	/* If the location to be accessed is beyond the size of the memory throw a run-time error and stop the program */
	assert(indexToArray >= 0 && indexToArray < MEMORY_SIZE);
	/* Return value from the desired location */
	return pointerToMemo_Ptr->memory[indexToArray];
}
/* This function get two succesive values (byte) from the memory and combine them to from the opcode (16bit) */
unsigned short combineBytes_chip8_memory(struct chip8_memory* pointerToMemo_Ptr, int index)
{
	unsigned char firstByte, secondByte;
	firstByte = get_chip8_memory(pointerToMemo_Ptr, index);
	secondByte = get_chip8_memory(pointerToMemo_Ptr, index + 1);
	return (firstByte << 8 | secondByte);
}
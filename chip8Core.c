/**************************************************************
 * File name: chip8stack.c                                    *
 * Version: 1.0                                               *
 * Author: Oussama Moustakim                                  *
 * Date: 08/08/2020 <During COVID-19 lockDown>                *
 * Description : In this module we create all functions       *
 *               That are essential to the functioning of     *
 *               chip8 like initialization, loading, fetching *
 *               the opcode and wait for keyboard input       *
 **************************************************************/
#include "chip8header.h"
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include "SDL2/SDL.h"
/* This is the definition of all built-in sprites from 0 to 9 and from A to F */
const unsigned char  chip8_fontSet[] = 
{    
	0xF0,0x90,0x90,0x90,0xF0,   /* Sprite 0 */
	0x20,0x60,0x20,0x20,0x70,   /* Sprite 1 */ 
	0xF0,0x10,0xF0,0x80,0xF0,   /* Sprite 2 */
	0xF0,0x10,0xF0,0x10,0xF0,   /* Sprite 3 */ 
	0x90,0x90,0xF0,0x10,0x10,   /* Sprite 4 */
	0xF0,0x80,0xF0,0x10,0xF0,   /* Sprite 5 */
	0xF0,0x80,0xF0,0x90,0xF0,   /* Sprite 6 */
	0xF0,0x10,0x20,0x40,0x40,   /* Sprite 7 */
	0xF0,0x90,0xF0,0x90,0xF0,   /* Sprite 8 */
	0xF0,0x90,0xF0,0x10,0xF0,   /* Sprite 9 */
	0xF0,0x90,0xF0,0x90,0x90,   /* Sprite A */
	0xE0,0x90,0xE0,0x90,0xE0,   /* Sprite B */
	0xF0,0x80,0x80,0x80,0xF0,   /* Sprite C */
	0xE0,0x90,0x90,0x90,0xE0,   /* Sprite D */
	0xF0,0x80,0xF0,0x80,0xF0,   /* Sprite E */ 
	0xF0,0x80,0xF0,0x80,0x80};  /* Sprite F */

/* This function initialises all the hardware of chip8 (memory, screeen, registers, keyboard and stack) */
void init_chip8(struct chip8* pointerToStruct_ptr)
{ 
	unsigned short memoIndex = 0;
	unsigned short pix_x = 0;
	unsigned short pix_y = 0;
	unsigned char  keyBr_index = 0;
	unsigned char  vReg_index = 0;
	unsigned char  stack_index = 0;
	
	/* Load the fontSet to the memory from the location 0x0000 to 0x004F (79) (total of 80 char) */
	for(memoIndex = 0; memoIndex < SPRITE_SIZE; memoIndex++)
	{
		pointerToStruct_ptr->mainMemory.memory[memoIndex] = chip8_fontSet[memoIndex];
	}
	/* initialze the remainig location of the  memory */
	for(memoIndex = SPRITE_SIZE; memoIndex < MEMORY_SIZE; memoIndex++)
	{
		pointerToStruct_ptr->mainMemory.memory[memoIndex] = 0;
	}
	/* set the pixels of the screen to off */
	for(pix_x = 0; pix_x < DISPLAY_WIDTH_PX; pix_x++)
	{
		for(pix_y = 0; pix_y < DISPLAY_LENGHT_PX; pix_y++)
		{
			pointerToStruct_ptr->mainScreen.pixels[pix_x][pix_y] = 0;
		}
	}
	/* Initialise the I,SP,PC, delayTimer, the sound timer and general purposes registers Vx*/
	pointerToStruct_ptr->mainRegisters.I = 0;
	pointerToStruct_ptr->mainRegisters.SP = 0;
	pointerToStruct_ptr->mainRegisters.PC = 0;
	pointerToStruct_ptr->mainRegisters.delay_timer = 0;
	pointerToStruct_ptr->mainRegisters.sound_timer = 0;
	for(vReg_index = 0; vReg_index < NBR_REGISTERS; vReg_index++)
	{
		pointerToStruct_ptr->mainRegisters.V[vReg_index] = 0;
	}
	/* Initialise the keyboard array */
	for(keyBr_index = 0; keyBr_index < MAX_KEY; keyBr_index++)
	{
		pointerToStruct_ptr->mainKeyboard.keyboard[keyBr_index] = 0;
	}
	/* Initialise the stack */
	for(stack_index = 0; stack_index < STACK_SIZE; stack_index++)
	{
		pointerToStruct_ptr->mainStack.stack[stack_index] = 0;
	}
}
/* This function copies the content of our file program into memory */
void load_chip8_memory(struct chip8* chip8, const char* buffer, size_t size)
{
	assert(size + CHIP8_PROGRAM_LOAD_ADDRESS < MEMORY_SIZE);
	memcpy(&chip8->mainMemory.memory[CHIP8_PROGRAM_LOAD_ADDRESS],buffer,size);
	chip8->mainRegisters.PC = CHIP8_PROGRAM_LOAD_ADDRESS;
}
/* This function creates a delay until the user press a key from the keyboard */
char wait_chip8_key_press(struct chip8* pointerToStr_ptr)
{
	char presssedKey = 0, mapped_key = 0;
	SDL_Event event;
	while(SDL_WaitEvent(&event))
	{
		if(event.type != SDL_KEYDOWN)
		{
			continue;
		}
		else
		{
			/* Do Nothing */
		}
		presssedKey = event.key.keysym.sym;
		mapped_key = map_chip8_keyboard(&pointerToStr_ptr->mainKeyboard, presssedKey);
		if(mapped_key != -1)
		{
			return mapped_key;
		}
		else
		{
			/* Do nothing */
		}
	}
	return -1;
}
/* This function returns an index (from 0 to 33) to the array function (pointerToFunction) delcared in main.c */
unsigned char fetch_opcode(unsigned short opcode)
{
	unsigned char index = 0;
	unsigned char opref = (opcode & 0xF000) >> 12;
	if(0x0000 == opref)
	{
		if(0x0000 == (opcode & 0x000F))
		{
			index =  0;
		}
		else if(0x000E == (opcode & 0x000F))
		{
			index =  1;
		}
		else
		{
			/* Do nothing */
		}
	}
	else if(opref >= 1 && opref <= 7)
	{
		index =  (opref + 1);
	}
	else if(0x0008 == opref)
	{
		if((opcode & 0x000F) >= 0 && (opcode & 0x000F) <= 7)
		{
			index =  (opref + (opcode & 0x000F) + 1); 
		}
		else if(0x000E == (opcode & 0x000F))
		{
			index =  17;
		}
		else
		{
			/* Do nothing */
		}
	}
	else if(0x0009 == opref)
	{
		index =  18;
	}
	else if(opref >= 0x000A && opref <= 0x000D)
	{
		index =  (opref + 9);
	}
	else if(0x000E == opref)
	{
		if(0x000E == (opcode & 0x000F))
		{
			index =  23;
		}
		else if(0x0001 == (opcode & 0x000F))
		{
			index =  24;
		}
		else
		{
			/* Do nothing */
		}
	}
	else
	{
		if (0x0007 == (opcode & 0x00FF))
		{
			index =  25;
		}
		else if (0x000A == (opcode & 0x00FF))
		{
			index =  26;
		}
		else if (0x0015 == (opcode & 0x00FF))
		{
			index =  27;
		}
		else if (0x0018 == (opcode & 0x00FF))
		{
			index =  28;
		}
		else if (0x001E == (opcode & 0x00FF))
		{
			index =  29;
		}
		else if (0x0029 == (opcode & 0x00FF))
		{
			index =  30;
		}
		else if (0x0033 == (opcode & 0x00FF))
		{
			index =  31;
		}
		else if (0x00055== (opcode & 0x00FF))
		{
			index =  32;
		}
		else if (0x0065 == (opcode & 0x00FF))
		{
			index =  33;
		}
		else
		{
			/* Do nothing */
		}
	}

return index;
}

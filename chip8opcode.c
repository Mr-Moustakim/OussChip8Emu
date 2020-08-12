/**********************************************************
 * File name: chip8opcode.c                               *
 * Version: 1.0                                           *
 * Author: Oussama Moustakim                              *
 * Date: 08/08/2020 <During COVID-19 lockDown>            *
 * Description : In this module we create all functions   *
 *               That emulate the behavior of all opcodes *
 **********************************************************/
#include "chip8header.h"
#include <stdlib.h>

/* Clear the screen */
void op_00E0(struct chip8* pointerTostr_ptr, unsigned short opcode)
{
	unsigned char xAxis = 0;
	unsigned char yAxis = 0;
	/* The screen is clear if all the pixels of the screen are all OFF */
	for(xAxis = 0; xAxis < DISPLAY_WIDTH_PX; xAxis++)
	{
		for(yAxis = 0; yAxis < DISPLAY_LENGHT_PX; yAxis++)
		{
			pointerTostr_ptr->mainScreen.pixels[xAxis][yAxis] = 0;
		}
	}
}

/* Return from a subroutine */
void op_00EE(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	pointerToStr_ptr->mainRegisters.PC = pop_chip8_stack(pointerToStr_ptr);
}

/* Jump to the adress nnn */
void op_1nnn(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	/* Extracting the adress nnn from the given opcode */
	unsigned short nnn = opcode & 0x0FFF; 
	pointerToStr_ptr->mainRegisters.PC = nnn;
}

/* Call a subroutine */
void op_2nnn(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	/* Extracting the adress nnn from the given opcode */
	unsigned short nnn = opcode & 0x0FFF; 
	push_chip8_stack(pointerToStr_ptr,pointerToStr_ptr->mainRegisters.PC);
	pointerToStr_ptr->mainRegisters.PC = nnn;
}

/* Skip the next instruction if vx is equal to kk */
void op_3xkk(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char kk = (opcode & 0x00FF);
	if(pointerToStr_ptr->mainRegisters.V[x] == kk)
	{
		pointerToStr_ptr->mainRegisters.PC += 2;
	}
	else
	{
		/* Do nothing */
	}
}

/* Skip the next instruction if vx is not equal to kk */
void op_4xkk(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char kk = (opcode & 0x00FF);
	if(pointerToStr_ptr->mainRegisters.V[x] != kk)
	{
		pointerToStr_ptr->mainRegisters.PC += 2;
	}
	else
	{
		/* Do nothing */
	}
}

/* Skip the next instruction if vx is equal to vy */
void op_5xy0(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;
	if(pointerToStr_ptr->mainRegisters.V[x] == pointerToStr_ptr->mainRegisters.V[y])
	{
		pointerToStr_ptr->mainRegisters.PC += 2;
	}
	else
	{
		/* Do nothing */
	}
}

/* Put the value kk in the general purpose register Vx */
void op_6xkk(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char kk = (opcode & 0x00FF);
	pointerToStr_ptr->mainRegisters.V[x] = kk;
}

/* Add the value kk to the register Vx and store the result in Vx */
void op_7xkk(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char kk = (opcode & 0x00FF);
	pointerToStr_ptr->mainRegisters.V[x] += kk;
}

/* Store the value of Vy in vx */
void op_8xy0(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;
	pointerToStr_ptr->mainRegisters.V[x] = pointerToStr_ptr->mainRegisters.V[y];
}

/* Perform bitwise OR between Vx and Vy and store in Vx */
void op_8xy1(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;
	pointerToStr_ptr->mainRegisters.V[x] |= pointerToStr_ptr->mainRegisters.V[y];
}

/* Perform bitwise AND between Vx and Vy and store in Vx */
void op_8xy2(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;
	pointerToStr_ptr->mainRegisters.V[x] &= pointerToStr_ptr->mainRegisters.V[y];
}

/* Perform bitwise XOR between Vx and Vy and store in Vx */
void op_8xy3(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;
	pointerToStr_ptr->mainRegisters.V[x] ^= pointerToStr_ptr->mainRegisters.V[y];
}

/* Add Vx to Vy if > 255 set Vf, store only lowest 8bits in Vx */
void op_8xy4(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;
	unsigned short summ = pointerToStr_ptr->mainRegisters.V[x] + pointerToStr_ptr->mainRegisters.V[y];
	if(summ > 255)
	{
		pointerToStr_ptr->mainRegisters.V[0xF] = 1;
	}
	else
	{
		pointerToStr_ptr->mainRegisters.V[0xF] = 0;
	}
	pointerToStr_ptr->mainRegisters.V[x] = summ & 0x00FF;
}

/* If Vx > Vy then set Vf and substract Vy from Vx and store the result in Vx */
void op_8xy5(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;
	if(pointerToStr_ptr->mainRegisters.V[x] > pointerToStr_ptr->mainRegisters.V[y])
	{
		pointerToStr_ptr->mainRegisters.V[0xF] = 1;
	}
	else
	{
		pointerToStr_ptr->mainRegisters.V[0xF] = 0;
	}
	pointerToStr_ptr->mainRegisters.V[x] -= pointerToStr_ptr->mainRegisters.V[y];
}

/* If the LSB of Vx is 1 set Vx, the divide Vx by 2 */
void op_8xy6(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char LSB = (pointerToStr_ptr->mainRegisters.V[x] & 0x1);
	pointerToStr_ptr->mainRegisters.V[0xF] = LSB;
	pointerToStr_ptr->mainRegisters.V[x] >>= 1;
}

/* If Vy > Vx then set Vf and substract Vx from Vy and store the result in Vx */
void op_8xy7(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;
	if(pointerToStr_ptr->mainRegisters.V[x] < pointerToStr_ptr->mainRegisters.V[y])
	{
		pointerToStr_ptr->mainRegisters.V[0xF] = 1;
	}
	else
	{
		pointerToStr_ptr->mainRegisters.V[0xF] = 0;
	}
	pointerToStr_ptr->mainRegisters.V[x] = pointerToStr_ptr->mainRegisters.V[y] - pointerToStr_ptr->mainRegisters.V[x];
}

/* If the MSB of Vx is 1 set Vx, the multiply Vx by 2 */
void op_8xyE(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char MSB = (pointerToStr_ptr->mainRegisters.V[x] & 0x80) >> 7;
	pointerToStr_ptr->mainRegisters.V[0xF] = MSB;
	pointerToStr_ptr->mainRegisters.V[x] <<= 1;
}

/* Skip the next instruction if vx is not equal to vy */
void op_9xy0(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;
	if(pointerToStr_ptr->mainRegisters.V[x] != pointerToStr_ptr->mainRegisters.V[y])
	{
		pointerToStr_ptr->mainRegisters.PC += 2;
	}
	else
	{
		/* Do nothing */
	}
}

/* Set the register I to the value nnn */
void op_Annn(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned short nnn = (opcode & 0x0FFF);
	pointerToStr_ptr->mainRegisters.I = nnn;
}

/* Set the program counter to V0 + nnn */
void op_Bnnn(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned short nnn = (opcode & 0x0FFF);
	pointerToStr_ptr->mainRegisters.PC = nnn + pointerToStr_ptr->mainRegisters.V[0];
}

/* Generate random number between 0 and 255 then AND with kk and store in Vx */
void op_Cxkk(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	int random = (rand()%(255 - 0 + 1)) + 0;
	unsigned char kk = (opcode & 0x00FF);
	unsigned char x = (opcode & 0x0F00) >> 8;
	pointerToStr_ptr->mainRegisters.V[x] = random & kk;
}

/* Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision */
void op_Dxyn(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char n = (opcode & 0x000F);
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;
	unsigned char x_pos = pointerToStr_ptr->mainRegisters.V[x];
	unsigned char y_pos = pointerToStr_ptr->mainRegisters.V[y];
	unsigned char *sprite = &pointerToStr_ptr->mainMemory.memory[pointerToStr_ptr->mainRegisters.I];
	bool collision = draw_chip8_sprites(&pointerToStr_ptr->mainScreen,x_pos,y_pos,sprite,n);
	pointerToStr_ptr->mainRegisters.V[0x0F] = collision;
}

/* Skip next instruction if key with the value of Vx is pressed. */
void op_Ex9E(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	if(check_key_pressed_chip8(&pointerToStr_ptr->mainKeyboard, pointerToStr_ptr->mainRegisters.V[x]))
	{
		pointerToStr_ptr->mainRegisters.PC += 2;
	}
	else
	{
		/* Do nothing */
	}
}

/* Skip next instruction if key with the value of Vx is not pressed. */
void op_ExA1(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	if(!check_key_pressed_chip8(&pointerToStr_ptr->mainKeyboard, pointerToStr_ptr->mainRegisters.V[x]))
	{
		pointerToStr_ptr->mainRegisters.PC += 2;
	}
	else
	{
		/* Do nothing */
	}	
}

/* The value of DT is set into vx */
void op_Fx07(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	pointerToStr_ptr->mainRegisters.V[x] = pointerToStr_ptr->mainRegisters.delay_timer;
}

/* Wait until key is pressed, store the value in the vx */
void op_Fx0A(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	char pressed_key = 0;
	pressed_key = wait_chip8_key_press(pointerToStr_ptr);
	pointerToStr_ptr->mainRegisters.V[x] = pressed_key;
}

/* Set the delay timer to the value of Vx */
void op_Fx15(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	pointerToStr_ptr->mainRegisters.delay_timer = pointerToStr_ptr->mainRegisters.V[x];
}

/* Set the sound timer to the value of Vx */
void op_Fx18(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	pointerToStr_ptr->mainRegisters.sound_timer = pointerToStr_ptr->mainRegisters.V[x];
}

/* Set the value of register I to I + Vx */
void op_Fx1E(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	pointerToStr_ptr->mainRegisters.I += pointerToStr_ptr->mainRegisters.V[x];
}

/* Set the register I to the location of hexadecimal of Vx */
void op_Fx29(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	pointerToStr_ptr->mainRegisters.I = pointerToStr_ptr->mainRegisters.V[x] * DEFAULT_SPRITE_HEIGHT;
}

/* Store BCD respresentation of vx in location I, I+1, I+2 */
void op_Fx33(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char value = pointerToStr_ptr->mainRegisters.V[x];
	set_chip8_memory(&pointerToStr_ptr->mainMemory, pointerToStr_ptr->mainRegisters.I + 2, value % 10);
	value /= 10;
	set_chip8_memory(&pointerToStr_ptr->mainMemory, pointerToStr_ptr->mainRegisters.I + 1, value % 10);
	value /= 10;
	set_chip8_memory(&pointerToStr_ptr->mainMemory, pointerToStr_ptr->mainRegisters.I, value % 10);
}

/* Store value of V0 through Vx at memory starting from I */
void op_Fx55(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char index = 0;
	unsigned char value = 0;
	unsigned char x = (opcode & 0x0F00) >> 8;
	for(index = 0; index <= x; index++)
	{
		value = pointerToStr_ptr->mainRegisters.V[index];
		set_chip8_memory(&pointerToStr_ptr->mainMemory, pointerToStr_ptr->mainRegisters.I + index, value);
	}
}

/* Read from memory starting at I and store in V0 through Vx*/
void op_Fx65(struct chip8* pointerToStr_ptr, unsigned short opcode)
{
	unsigned char index = 0;
	unsigned char value = 0;
	unsigned char x = (opcode & 0x0F00) >> 8;
	
	for(index = 0; index <= x; index++)
	{
		value = get_chip8_memory(&pointerToStr_ptr->mainMemory, pointerToStr_ptr->mainRegisters.I + index);
		pointerToStr_ptr->mainRegisters.V[index] = value;
	}
}
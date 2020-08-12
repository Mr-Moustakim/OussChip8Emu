/**************************************************
 * File name: chip8header.h                       *
 * Version: 1.0                                   *
 * Author: Oussama Moustakim                      *
 * Date: 08/08/2020 <During COVID-19 lockDown>    *
 * Description : The header file contains all the *
 *               necessary defintion required by  *
 *               The chip8: memory, sxcreen,stack *
 *               keyboard.. plus the prototypes   *
 *				 of all used function			  *
 **************************************************/
 
/* Define header guards */
#ifndef CHIP8_HEADER
#define CHIP8_HEADER
/* define the number of all general purpose registers of chip8 which are 16 registers */
#define NBR_REGISTERS  16
/* here we define 4096 memory locations to emulate the real memory */
#define MEMORY_SIZE 4096
/* define the width of our screen to be 64 pixels  */      
#define DISPLAY_WIDTH_PX 64 
/* define the length to be 32 pixels */ 
#define DISPLAY_LENGHT_PX 32
/* A Const to scale or window from original 64*32 to desired dimension */ 
#define WINDOW_SCAL 10
/* The stack has 16 locations at most  */
#define STACK_SIZE 16
/* The Chip-8 has 16 key decoded in Hex from 0 to F */
#define MAX_KEY 16
/* Define the size to store all built-in sprites which are 16 sprite each is 5bytes */
#define SPRITE_SIZE 80
/* Each sprite has the size of 8bytes in total */
#define SPRITE_WIDTH 8
/* This is the starting point of our program */
#define CHIP8_PROGRAM_LOAD_ADDRESS 0x200
/* Each default sprite is 5 byte in Height */
#define DEFAULT_SPRITE_HEIGHT 5
/* Total number of opcode used by the chip8 */
#define TOTAL_OPCODE 34      

/* Define type of boolean values */
typedef enum
{
	FALSE,
	TRUE
}bool;

/* Definition of the chip8's memory */
struct chip8_memory
{
	unsigned char  memory[MEMORY_SIZE]; /* 4096 slots of memory each slot is 1 byte */	
};

/* Definition of all the chip8 registers */
struct chip8_registers
{
	unsigned char  V[NBR_REGISTERS]; /* Here we define all the 16 registers from V0 to VF */
	unsigned short I;                /* Index register  */
	unsigned short PC;               /* Program Counter */
	unsigned char  SP;               /* Stack Pointer   */
	unsigned char  delay_timer;      /* The delay Timer */
	unsigned char  sound_timer;      /* The sound Timer */
};

/* Definition of the stack */
struct chip8_stack
{
	unsigned short stack[STACK_SIZE];
};

/*  Defintion of the keyboard */
struct chip8_keyboard
{
	bool keyboard[MAX_KEY];
	const char* keyboard_map;
};

/* Definition of the display */
struct chip8_display
{
	bool pixels[DISPLAY_WIDTH_PX][DISPLAY_LENGHT_PX];
};

/* Stracture conraining all the element of the chip8's hardware */
struct chip8
{
	struct chip8_memory mainMemory;
	struct chip8_registers mainRegisters; /* we declare our defined register structure */
	struct chip8_stack mainStack;
	struct chip8_keyboard mainKeyboard;
	struct chip8_display mainScreen;
};

/* Prototypes of functions to set and get Values of memory */
void set_chip8_memory(struct chip8_memory* pointerToMemo_Ptr, int indexToArray, unsigned char value);
unsigned char get_chip8_memory(struct chip8_memory* pointerToMemo_Ptr, int indexToArray);

/* Prototypes of functions to push and pop from the stack */
void push_chip8_stack(struct chip8*, unsigned short address);
unsigned short pop_chip8_stack(struct chip8*);

/* Prototypes of functions to control the keyboard */
void hold_down_chip8_keyboard(struct chip8_keyboard *pointerToKey_ptr, int key);
void hold_up_chip8_keyboard(struct chip8_keyboard *pointerTokey_ptr, int key);
bool check_key_pressed_chip8(struct chip8_keyboard *pointerToKey_ptr, int key);
int  map_chip8_keyboard(struct chip8_keyboard* pointerToKey_ptr, char key);
void set_map_chip8_keyboard(struct chip8_keyboard* pointerToKey_ptr, const char* map);
char wait_chip8_key_press(struct chip8* pointerToStr_ptr);

/* Prototypes for initialisation functions */
void init_chip8(struct chip8* pointerToStruct_ptr);

/* Prototypes of functions handling the screen */
void set_on_chip8_pixels(struct chip8_display *pointerToPix_ptr, int x_Axis, int y_Axis);
bool check_chip8_pixels(struct chip8_display *pointerToPix_ptr, int x_Axis, int y_Axis);
bool draw_chip8_sprites(struct chip8_display *pointerToPix_ptr,int x_Axis, int y_Axis, unsigned char *pointerToSprite_ptr, int nbr_bytes);

/* Prototype of function handling the execution of instruction aka opcode */
unsigned char fetch_opcode(unsigned short opcode);

/* Prototype of function to combine two bytes to get opcode */
unsigned short combineBytes_chip8_memory(struct chip8_memory* pointerToMemo_Ptr, int index);

/* Prototypes of all functions emulating the opcode instruction */
void op_00E0(struct chip8* pointerTostr_ptr, unsigned short opcode);
void op_00EE(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_1nnn(struct chip8* pointerTostr_ptr, unsigned short opcode); 
void op_2nnn(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_3xkk(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_4xkk(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_5xy0(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_6xkk(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_7xkk(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_8xy0(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_8xy1(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_8xy2(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_8xy3(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_8xy4(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_8xy5(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_8xy6(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_8xy7(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_8xyE(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_9xy0(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_Annn(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_Bnnn(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_Cxkk(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_Dxyn(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_Ex9E(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_ExA1(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_Fx07(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_Fx0A(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_Fx15(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_Fx18(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_Fx1E(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_Fx29(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_Fx33(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_Fx55(struct chip8* pointerToStr_ptr, unsigned short opcode); 
void op_Fx65(struct chip8* pointerToStr_ptr, unsigned short opcode); 

#endif 

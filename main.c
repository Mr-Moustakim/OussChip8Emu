/*****************************************************
 * File name: main.c                                 *
 * Version: 1.0                                      *
 * Author: Oussama Moustakim                         *
 * Date: 08/08/2020 <During COVID-19 lockDown>       *
 * Description : In this module we create our window *
 *               set the renderer to handle graphics *
 *               Then load the program into memory   *
 *               and create the main treatement loop *
 *****************************************************/
#include <stdio.h>
#include "SDL2/SDL.h"
#include "chip8header.h"
#include <Windows.h>
#include <stddef.h>
#include <string.h>

void load_chip8_memory(struct chip8* chip8, const char* buffer, size_t size);

/* SDL Declaration */
SDL_Window* myWindow;
SDL_Renderer* myRenderer;
SDL_Event myEvent;
SDL_Rect myRect;

/* Keyboard variables */
char pressedKey = 0;
bool isPressed = 0;
int virtualKey = 0;
const char myKeyboard[MAX_KEY] = {

	SDLK_0, SDLK_1, SDLK_2, SDLK_3,
	SDLK_4, SDLK_5, SDLK_6, SDLK_7,
	SDLK_8, SDLK_9, SDLK_a, SDLK_b,
	SDLK_c, SDLK_d, SDLK_e, SDLK_f
};

/* File handling */
char readFileResult = 0;
long fileSize = 0L;
FILE* myFile = NULL;
char* fileName = NULL;


/* Display handling */
int x = 0;
int y = 0;


/* Pointer to functions handling */
unsigned char indexToFunction = 0;
unsigned short opcode = 0x0000;
void (*pointerToFunction[TOTAL_OPCODE])(struct chip8*, unsigned short) = {
op_00E0, op_00EE, op_1nnn, op_2nnn, op_3xkk,
op_4xkk, op_5xy0, op_6xkk, op_7xkk, op_8xy0,
op_8xy1, op_8xy2, op_8xy3, op_8xy4, op_8xy5,
op_8xy6, op_8xy7, op_8xyE, op_9xy0, op_Annn,
op_Bnnn, op_Cxkk, op_Dxyn, op_Ex9E, op_ExA1,
op_Fx07, op_Fx0A, op_Fx15, op_Fx18, op_Fx1E,
op_Fx29, op_Fx33, op_Fx55, op_Fx65};

/* Chip8 essentials */
struct chip8 myChip8;
int quit = 0;



int main(int argc, char** argv)
{
	/* End the program if the value of argument count is less than 2 */
	if(argc < 2)
	{
		printf("You must provide a file to be loaded\n");
		return -1; 
	}
	else
	{
		/* Do nothing */
	}
	
	/* Store the file name */
	fileName = argv[1];
	printf("The name of your file is %s\n", fileName);
	/* Open the file in read-byte only mode */
	myFile = fopen(fileName,"rb");
	
	/* End the program if the opening is failed */
	if(!myFile)
	{
		printf("Error file cannot be open\n");
		return -1;
	}
	else
	{
		/* Do nothing */
	}
	
	/* Go to the end of the file */
	fseek(myFile,0,SEEK_END); 
	/* Once at the end of the file we can calculate the its size */
	fileSize = ftell(myFile);
	/* Using the advantage of VLA (variable lenght array) provided by the C99 */
	char buffer[fileSize];
	/* Go back to the start of the file so we can start reading */
	fseek(myFile,0,SEEK_SET);
	/* Read from the file and stock the result in our buffer */
	readFileResult = fread(buffer, fileSize, 1, myFile);
	
	/* End the program if the reading is failed */
	if(readFileResult != 1)
	{
		printf("Error file cannot be read\n");
		return -1; /* End the program without */
	}
	else
	{
		printf("the size of the file is %d and the open result is %d",(int)fileSize,(int)readFileResult);
	}

	/* Initializing the chip8 system */
	init_chip8(&myChip8); 
    printf("the program counter is at the locatoon %d\n",(int)myChip8.mainRegisters.PC);
	
	/* Load the program from the file into the memory */
	load_chip8_memory(&myChip8,buffer,fileSize);
	
	set_map_chip8_keyboard(&myChip8.mainKeyboard, myKeyboard);
	/* Initializing the SDL library  */
	SDL_Init(SDL_INIT_EVERYTHING);
	/* Creat new windows in the center of the screen with 640px * 320px */
	myWindow = SDL_CreateWindow("my CHIP8 window",
	SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED,
	DISPLAY_WIDTH_PX * WINDOW_SCAL, DISPLAY_LENGHT_PX * WINDOW_SCAL, SDL_WINDOW_SHOWN);
	/* Creat a renderer that takes the created window as argument*/
	myRenderer = SDL_CreateRenderer(myWindow, -1, SDL_TEXTUREACCESS_TARGET);
	/* The  mian loop of excution */
	while(!quit)
	{
		/* Handling the event of the keyboad */
		/* ==> For more details about the event handling  check the official documentation of SDL2.0 libraray <== */
		while(SDL_PollEvent(&myEvent))
		{
			/* Check if a key in the real keyboard has been pressed */
			if (myEvent.type == SDL_KEYDOWN)
			{
				/* Get the pressed key from the user */
				pressedKey = myEvent.key.keysym.sym;
				/* Map the pressed key to our the equivalent key of chip8 */
				virtualKey = map_chip8_keyboard(&myChip8.mainKeyboard, pressedKey);
				if(virtualKey != -1)
				{
					hold_down_chip8_keyboard(&myChip8.mainKeyboard,virtualKey);
					isPressed = check_key_pressed_chip8(&myChip8.mainKeyboard,virtualKey);
				}
				else
				{
					/* Do nothing */
				}

			}
			/* Check if the pressed key has been realesed again */
			else if(myEvent.type == SDL_KEYUP)
			{
				if(virtualKey != -1)
				{
					hold_up_chip8_keyboard(&myChip8.mainKeyboard,virtualKey);
					isPressed = check_key_pressed_chip8(&myChip8.mainKeyboard,virtualKey);
				}
				else
				{
					/* Do nothing */
				}
			}
			/* Set condondition to end the program if the user clicked the CLOSE button */
			else if (myEvent.type == SDL_QUIT)
			{
				quit = 1;
			}
			else
			{
				/* Do nothnign */
			}
			
		}
		SDL_SetRenderDrawColor(myRenderer,0,0,0,0);
		SDL_RenderClear(myRenderer);
		SDL_SetRenderDrawColor(myRenderer, 255, 255, 255, 0);
		/* Loop to draw pixels to the screen */
		for(x = 0; x < DISPLAY_WIDTH_PX; x++)
		{
			for(y = 0; y < DISPLAY_LENGHT_PX; y++)
			{
				if(check_chip8_pixels(&myChip8.mainScreen,x,y))
				{
					myRect.x = x * WINDOW_SCAL;
					myRect.y = y * WINDOW_SCAL;
					myRect.h = WINDOW_SCAL;
					myRect.w = WINDOW_SCAL;
					SDL_RenderFillRect(myRenderer,&myRect);
				}
				else
				{
					/* Do nothing */
				}
			}
		}
		/* refresh the rederer to get the desired effect on the screen */
		SDL_RenderPresent(myRenderer);
		
		/* If sound and delay timers are postive decrent them */
		if(myChip8.mainRegisters.delay_timer > 0)
		{
			Sleep(16.6);
			myChip8.mainRegisters.delay_timer -= 1;
		}
		else
		{
			/* Do nothing */
		}
		if(myChip8.mainRegisters.sound_timer)
		{
			Beep(440, 10 * myChip8.mainRegisters.sound_timer);
			myChip8.mainRegisters.sound_timer = 0;
		}
		else
		{
			/* Do nothing */
		}
		/* Get the opcode from the main memory */
		opcode = combineBytes_chip8_memory(&myChip8.mainMemory, myChip8.mainRegisters.PC);
		/* Increment the program counter */
		myChip8.mainRegisters.PC += 2;
		/* Check the equivalent index for the retrieved opcode */
		indexToFunction = fetch_opcode(opcode);
		/* Call the function relative to the opcode */
		pointerToFunction[indexToFunction](&myChip8, opcode);
	}
	/* Destroy the window if the user clicked the close button */
	SDL_DestroyWindow(myWindow);
	return 0;
}
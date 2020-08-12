/**********************************************************
 * File name: chip8display.c                              *
 * Version: 1.0                                           *
 * Author: Oussama Moustakim                              *
 * Date: 08/08/2020 <During COVID-19 lockDown>            *
 * Description : In this module we create all functions   *
 *               That handles the graphics and drawing to *
 *               main screen.                             *
 **********************************************************/
#include "chip8header.h"
#include <assert.h>
/* This function set the pixels at location (x,y) ON */
void set_on_chip8_pixels(struct chip8_display *pointerToPix_ptr, int x_Axis, int y_Axis)
{
	assert(x_Axis >= 0 && x_Axis < DISPLAY_WIDTH_PX && y_Axis >= 0 && y_Axis < DISPLAY_LENGHT_PX);
	pointerToPix_ptr->pixels[x_Axis][y_Axis] = TRUE;
}

/* This function checks if the pixel at location (x,y) is ON or OFF */
bool check_chip8_pixels(struct chip8_display *pointerToPix_ptr, int x_Axis, int y_Axis)
{
	assert(x_Axis >= 0 && y_Axis < DISPLAY_WIDTH_PX && x_Axis >= 0 && y_Axis < DISPLAY_LENGHT_PX);
	return pointerToPix_ptr->pixels[x_Axis][y_Axis];
}

/* This functions draws sprites as descirbed in the official documentation of chip8 */
bool draw_chip8_sprites(struct chip8_display *pointerToPix_ptr,int x_Axis, int y_Axis, unsigned char *pointerToSprite_ptr, int nbr_bytes)
{
	bool pixel_collision = FALSE;
	unsigned char spriteRaw = 0;
	unsigned char xIndex = 0;
	unsigned char yIndex = 0;
	
	for(yIndex = 0; yIndex < nbr_bytes; yIndex++)
	{
		spriteRaw = pointerToSprite_ptr[yIndex];
		for(xIndex = 0; xIndex < SPRITE_WIDTH; xIndex++)
		{
			if(0 == ((0b10000000 >> xIndex) & spriteRaw))
			{
				continue;
			}
			else
			{
				/* Do nothing */
			}
			if(pointerToPix_ptr->pixels[(x_Axis + xIndex) % DISPLAY_WIDTH_PX][(y_Axis + yIndex) % DISPLAY_LENGHT_PX])
			{
				pixel_collision = TRUE;
			}
			else
			{
				/* Do nothing */
			}
			pointerToPix_ptr->pixels[(x_Axis + xIndex) % DISPLAY_WIDTH_PX][(y_Axis + yIndex) % DISPLAY_LENGHT_PX] ^= TRUE;
		}
	}
	return pixel_collision;
}
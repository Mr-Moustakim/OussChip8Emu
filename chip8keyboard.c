/*************************************************************
 * File name: chip8keyboard.c                                *
 * Version: 1.0                                              *
 * Author: Oussama Moustakim                                 *
 * Date: 08/08/2020 <During COVID-19 lockDown>               *
 * Description : In this module we create all functions      *
 *               That handles the input from user's keyboard *
 *************************************************************/
#include "chip8header.h"
/* This function sets the keyboard to TRUE if the key is pressed */
void hold_down_chip8_keyboard(struct chip8_keyboard *pointerToKey_ptr, int key)
{
    pointerToKey_ptr->keyboard[key] = TRUE; 
}
/* This function sets the keyboard to FALSE if the key is not pressed */
void hold_up_chip8_keyboard(struct chip8_keyboard *pointerToKey_ptr, int key)
{
    pointerToKey_ptr->keyboard[key] = FALSE; /* If the key is not hold its value will be 0 */
}
/* This function checks if the key is pressed or not */
bool check_key_pressed_chip8(struct chip8_keyboard *pointerToKey_ptr, int key)
{
    return pointerToKey_ptr->keyboard[key];  /* Return the value of the key either pressed (1) or not pressed (0) */
}
/* This function sets virtual keyboard of chip8 to the mapped keyboard */
void set_map_chip8_keyboard(struct chip8_keyboard* pointerToKey_ptr, const char* map)
{
	pointerToKey_ptr->keyboard_map = map;
}
/* This function maps the pressed key by the user to the virtual keyboard of the chip8 */
int map_chip8_keyboard(struct chip8_keyboard* pointerToKey_ptr, char key)
{
	unsigned char index = 0;
	/* check if the pressed key equals to one of the virtual key from ('0' to 'F') */
   for(index = 0; index < MAX_KEY; index++)
   {
       if(pointerToKey_ptr->keyboard_map[index] == key)
       {
           return index;
       }
       else
       {
           /* Do nothing */
       }
   }
   return -1;
}
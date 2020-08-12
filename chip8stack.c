/*****************************************************
 * File name: chip8stack.c                           *
 * Version: 1.0                                      *
 * Author: Oussama Moustakim                         *
 * Date: 08/08/2020 <During COVID-19 lockDown>       *
 * Description : In this module we create funtions   *
 *               That to pop from the stack or push  *
 *               values into the stack               *
 *****************************************************/
#include "chip8header.h"
#include "assert.h"

/* When a subroutine is called push the current location of the program counter to the stack */
void push_chip8_stack(struct chip8* pointerToStack_Ptr,unsigned short address)
{
    pointerToStack_Ptr->mainRegisters.SP++;
    assert(pointerToStack_Ptr->mainRegisters.SP < sizeof(pointerToStack_Ptr->mainStack.stack));
    pointerToStack_Ptr->mainStack.stack[pointerToStack_Ptr->mainRegisters.SP] = address;    
}

/* When returning from a subroutine set the program counter to the value at the top of the stack */
unsigned short pop_chip8_stack(struct chip8* pointerToStack_Ptr)
{
    assert(pointerToStack_Ptr->mainRegisters.SP < sizeof(pointerToStack_Ptr->mainStack.stack));
    unsigned short adress = pointerToStack_Ptr->mainStack.stack[pointerToStack_Ptr->mainRegisters.SP];
    pointerToStack_Ptr->mainRegisters.SP -= 1;
    return adress;
}
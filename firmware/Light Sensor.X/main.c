/* 
 * File:   main.c
 * Author: Christian Kramer
 *
 * Created on September 20, 2018, 9:26 PM
 */

// CONFIG
#pragma config IOSCFS = 8MHZ    // Internal Oscillator Frequency Select bit (8 MHz)
#pragma config MCPU = ON        // Master Clear Pull-up Enable bit (Pull-up enabled)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config CP = OFF         // Code protection bit (Code protection off)
#pragma config MCLRE = ON       // GP3/MCLR Pin Function Select bit (GP3/MCLR pin function is MCLR)

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

/*
 * 
 */

char getLightValue()
{
    ADCON0bits.GO = 1;
    return ~ADRES;
}

void main() {
    
    TRISGPIO    = 0x01;
    ADCON0      = 0x41; //01000001
    
    char startValue = getLightValue();
    char count = 0;
    
    while (1) {
        char currentValue = getLightValue();
        if (currentValue > startValue + 5 && count < 100)
        {
            count++;
        }
        if (currentValue < startValue - 5 && count > 0)
        {
            count--;
        }
        if (count == 100)
        {
            GPIO = 0x04;
        }
        if (count == 0)
        {
            GPIO = 0x00;
        }
    }
}


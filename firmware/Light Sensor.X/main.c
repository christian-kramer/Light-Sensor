/* 
 * File:   main.c
 * Author: Christian Kramer
 *
 * Created on September 20, 2018, 9:26 PM
 */

#define _XTAL_FREQ 8000000

// CONFIG
#pragma config IOSCFS = 8MHZ    // Internal Oscillator Frequency Select bit (8 MHz)
#pragma config MCPU = ON        // Master Clear Pull-up Enable bit (Pull-up enabled)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config CP = OFF         // Code protection bit (Code protection off)
#pragma config MCLRE = ON       // GP3/MCLR Pin Function Select bit (GP3/MCLR pin function is MCLR)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xc.h>

/*
 * 
 */


char getLightValue()
{
    ADCON0bits.GO = 1;
    return ~ADRES;
}

void flash()
{
    GPIO = 0x04;
    __delay_ms(10);
    GPIO = 0x00;
}

char calibrate()
{
    char count = 0;
    char baseline = 0;
    
    while (count++ < 254)
    {
        char newValue = getLightValue();
        
        if (newValue > baseline && newValue < 249)
        {
            baseline = newValue;
        }
        
        __delay_ms(2);
    }
    
    return baseline;
}

void main() {
    
    
    OSCCALbits.FOSC4 = 0;
    OPTION = 0b01000000;
    TRISGPIO = 0x01;
    ADCON0 = 0x41; //01000001
    GPIO = 0x00;
    ADRES = 0x00;
    
    flash();
    
    char score = 50;
    
    char startValue = calibrate();
    
    flash();
    
    while (1)
    {
        char currentValue = getLightValue();
        
        
        if (currentValue > startValue + 5 && score < 100)
        {
            score++;
        }
        
        if (currentValue < startValue - 5 && score > 0)
        {
            score--;
        }
        
        if (score == 100)
        {
            GPIO = 0x04;
        }
        
        if (score == 0)
        {
            GPIO = 0x00;
        }
    }
}
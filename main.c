/*
 * File:   main.c
 * Author: Fabian Sohrmann
 * E-mail: mfsohr@utu.fi
 * 
 * Week2, Exercise 1 - Bomb
 * The program simulates diffusing a bomb before the timer reaches 0. A
 * countdown is displayed on a 7-segment LED-display. The countdown counts
 * from 9 to 0. When an interrupt occurs, the countdown stops.
 *
 * Created on 2. marraskuuta 2021, 14:58
 */

#define F_CPU        3333333
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


volatile int g_running = 1;
int main(void) 
{
    // Bit patterns for displaying numbers 9,8,7,...,0
    uint8_t number[] =
    {
        0b11101111, 0b11111111, 0b10000111, 0b11111101, 0b11101101, 
        0b11100110, 0b11001111, 0b11011011, 0b10000110, 0b10111111
    };
    int index = 0;
    int increment = 1;
    PORTC.DIRSET = 0xFF; // Setting all pins in port C to output
    PORTA.DIRCLR = PIN4_bm; // PIN4 to input (not necessay, same as default)
    PORTA.PIN4CTRL |= PORT_PULLUPEN_bm; // Enable PA4 pull-up resistor
    PORTA.PIN4CTRL = PORT_ISC_RISING_gc; // Both edges work
    sei(); // Enable interrupts
    while (g_running) 
    {
        PORTC.OUTCLR = 0xFF; // Clear pins
        // Draw numbers according to number bit patterns in number[]
        PORTC.OUTSET = number[index];  
        _delay_ms(1000);
        if(index <= 8)
        {
            index = index + increment; 
        }
        else
        {
            PORTC.OUTCLR = 0xFF; // Clear pins
            _delay_ms(500);
        }
    }
}

ISR(PORTA_PORT_vect)
{
    PORTA.INTFLAGS = 0xFF; // Clear interrupt flag
    g_running = 0;
}
/*
 * Project Name: Snake Game Project with PIC18F46K22
 * 
 * Authors: Yetkin AKYUZ (yetkin.akyuz@outlook.com)
 *
 * Created on December 29, 2020, 5:35 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include <xc.h>

// *************** CONFIGURATIONS ***************

// CONFIG1H
#pragma config FOSC = INTIO67   // Oscillator Selection bits (Internal oscillator block)
#pragma config PLLCFG = OFF     // 4X PLL Enable (Oscillator used directly)
#pragma config PRICLKEN = ON    // Primary clock enable bit (Primary clock is always enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power-up Timer Enable bit (Power up timer disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 190       // Brown Out Reset Voltage bits (VBOR set to 1.90 V nominal)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (Watch dog timer is always disabled. SWDTEN has no effect.)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC1  // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<5:0> pins are configured as analog input channels on Reset)
#pragma config CCP3MX = PORTB5  // P3A/CCP3 Mux bit (P3A/CCP3 input/output is multiplexed with RB5)
#pragma config HFOFST = ON      // HFINTOSC Fast Start-up (HFINTOSC output and ready status are not delayed by the oscillator stable status)
#pragma config T3CMX = PORTC0   // Timer3 Clock input mux bit (T3CKI is on RC0)
#pragma config P2BMX = PORTD2   // ECCP2 B output mux bit (P2B is on RD2)
#pragma config MCLRE = EXTMCLR  // MCLR Pin Enable bit (MCLR pin enabled, RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled if MCLRE is also 1)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection Block 0 (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection Block 1 (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection Block 2 (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection Block 3 (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection Block 0 (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection Block 1 (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection Block 2 (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection Block 3 (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0 (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection Block 1 (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection Block 2 (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection Block 3 (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

#define _XTAL_FREQ 64000000

// **************** PIN DEFINITIONS ****************

// BUTTONS
#define  buttonRight PORTBbits.RB0
#define  buttonLeft PORTBbits.RB1
#define  buttonUp PORTBbits.RB2
#define  buttonDown PORTBbits.RB3

// COLUMNS
#define PORTROW0 LATAbits.LATA1
#define PORTROW1 LATAbits.LATA2
#define PORTROW2 LATAbits.LATA3

#define PORTROW3 LATDbits.LATD3
#define PORTROW4 LATDbits.LATD4
#define PORTROW5 LATDbits.LATD5
#define PORTROW6 LATDbits.LATD6
#define PORTROW7 LATDbits.LATD7

// ROWS
#define PORTCOL0 LATEbits.LATE1
#define PORTCOL1 LATEbits.LATE2

#define PORTCOL2 LATCbits.LATC2
#define PORTCOL3 LATCbits.LATC3
#define PORTCOL4 LATCbits.LATC4
#define PORTCOL5 LATCbits.LATC5
#define PORTCOL6 LATCbits.LATC6
#define PORTCOL7 LATCbits.LATC7

// Column status lists
const unsigned char Col0[] = {1, 0, 0, 0, 0, 0, 0, 0};
const unsigned char Col1[] = {0, 1, 0, 0, 0, 0, 0, 0};
const unsigned char Col2[] = {0, 0, 1, 0, 0, 0, 0, 0};
const unsigned char Col3[] = {0, 0, 0, 1, 0, 0, 0, 0};
const unsigned char Col4[] = {0, 0, 0, 0, 1, 0, 0, 0};
const unsigned char Col5[] = {0, 0, 0, 0, 0, 1, 0, 0};
const unsigned char Col6[] = {0, 0, 0, 0, 0, 0, 1, 0};
const unsigned char Col7[] = {0, 0, 0, 0, 0, 0, 0, 1};

// Row status lists
const unsigned char Row0[] = {0, 1, 1, 1, 1, 1, 1, 1};
const unsigned char Row1[] = {1, 0, 1, 1, 1, 1, 1, 1};
const unsigned char Row2[] = {1, 1, 0, 1, 1, 1, 1, 1};
const unsigned char Row3[] = {1, 1, 1, 0, 1, 1, 1, 1};
const unsigned char Row4[] = {1, 1, 1, 1, 0, 1, 1, 1};
const unsigned char Row5[] = {1, 1, 1, 1, 1, 0, 1, 1};
const unsigned char Row6[] = {1, 1, 1, 1, 1, 1, 0, 1};
const unsigned char Row7[] = {1, 1, 1, 1, 1, 1, 1, 0};

// OSCILLATOR INITIALIZER FUNCTION
void OSCILLATOR(void) {
    
    OSCCONbits.IRCF = 0x7; // INTERNAL OSCILLATOR
    OSCTUNEbits.PLLEN = 1; // PLL is ON
}

// PIN MANAGER FUNCTION
void PIN_MANAGER(void) {

    /**
    TRISx registers
    */
    TRISB = 0; //INPUT
    
    TRISA = 1; //OUTPUT
    TRISC = 1; //OUTPUT
    TRISD = 1; //OUTPUT
    TRISE = 1; //OUTPUT
    
    /**
    ANSELx registers
    */    
    ANSELA = 0; //DIGITAL IO
    ANSELB = 0; //DIGITAL IO
    ANSELC = 0; //DIGITAL IO
    ANSELD = 0; //DIGITAL IO
    ANSELE = 0; //DIGITAL IO
    
    /**
     WPUx registers
     */
    WPUBbits.WPUB0 = 0;
    WPUBbits.WPUB1 = 0;
    WPUBbits.WPUB2 = 0;
    WPUBbits.WPUB3 = 0;
    
}

// CLEAN THE BOARD FUNCTION
void BOARD_CLEAN(void) {
    
    PORTCOL0 = 0;
    PORTCOL1 = 0;
    PORTCOL2 = 0;
    PORTCOL3 = 0;
    PORTCOL4 = 0;
    PORTCOL5 = 0;
    PORTCOL6 = 0;
    PORTCOL7 = 0;

    PORTROW0 = 1;
    PORTROW1 = 1;
    PORTROW2 = 1;
    PORTROW3 = 1;
    PORTROW4 = 1;
    PORTROW5 = 1;
    PORTROW6 = 1;
    PORTROW7 = 1;
}

// LIGHT ALL OF THE LEDS FUNCTION
void BOARD_FULL(void) {
    
    PORTCOL0 = 1;
    PORTCOL1 = 1;
    PORTCOL2 = 1;
    PORTCOL3 = 1;
    PORTCOL4 = 1;
    PORTCOL5 = 1;
    PORTCOL6 = 1;
    PORTCOL7 = 1;

    PORTROW0 = 0;
    PORTROW1 = 0;
    PORTROW2 = 0;
    PORTROW3 = 0;
    PORTROW4 = 0;
    PORTROW5 = 0;
    PORTROW6 = 0;
    PORTROW7 = 0;
}

// LIGHT THE LEDS FUNCTION
void BOARD_LIGHT(unsigned char X, unsigned char Y) {
    
    PORTCOL0 = Col0[X]; 
    PORTCOL1 = Col1[X];
    PORTCOL2 = Col2[X];
    PORTCOL3 = Col3[X];
    PORTCOL4 = Col4[X];
    PORTCOL5 = Col5[X];
    PORTCOL6 = Col6[X];
    PORTCOL7 = Col7[X];

    PORTROW0 = Row0[Y];
    PORTROW1 = Row1[Y];
    PORTROW2 = Row2[Y];
    PORTROW3 = Row3[Y];
    PORTROW4 = Row4[Y];
    PORTROW5 = Row5[Y];
    PORTROW6 = Row6[Y];
    PORTROW7 = Row7[Y];
}

char BUTTON_CONTROL(char Direction) {
    
    // *************** BUTTONS ****************
        
        // The snake cannot go in the opposite direction on the same axis.
        
        if (buttonRight && Direction != 'L'){
            
            Direction = 'R';
        }
        
        else if (buttonLeft && Direction != 'R') {
            Direction = 'L';           
        }
        
        else if (buttonUp && Direction != 'D') {
            Direction = 'U';         
        }
        
        else if (buttonDown && Direction != 'U') {
            Direction = 'D';           
        }
        
        else {
            //Nothing
        }
    
    __delay_ms(0.005);
    
    return Direction;
}

void PRINT_SNAKE(double Cycle, int SnakeLength, unsigned char SnakeX[], unsigned char SnakeY[], int FoodX, int FoodY, char Direction) {
    
    // *************** SNAKE DELAY CONTROL ***************    
    while(256 - ((0.1 * SnakeLength + 0.6) * Cycle) > 26) {
            
        Cycle--;
    }
    
    for (int j = 0; j < Cycle; j = j++) {
        
        Direction = BUTTON_CONTROL(Direction);
                
        // *************** PRINT SNAKE ****************

        for (int i = 0 ; i < SnakeLength; i++) {
            
            Direction = BUTTON_CONTROL(Direction);

            // Light the LEDs
            BOARD_LIGHT(SnakeX[i], SnakeY[i]);

            // Wait for 0.1 milliseconds
            __delay_ms(0.1);        
        }
        
        // Clean the board
        BOARD_CLEAN();
        
        // Wait for 0.5 milliseconds
        __delay_ms(0.5);
        
        // *************** PRINT FOOD ****************
         
        // Light the LEDs
        BOARD_LIGHT(FoodX, FoodY);

        // Wait for 0.1 milliseconds
        __delay_ms(0.1);
        
        // Clean the board
        BOARD_CLEAN();
    }
}

// MAIN FUNCTION
int main(void) {
    
    // System setup
    OSCILLATOR();
    PIN_MANAGER();
     
    // Light all of the LEDs
    BOARD_FULL();
    
    // Wait for a second
    __delay_ms(1000);
    
    // Clean the board
    BOARD_CLEAN();
    
    // **************** GAME SETUP ****************
        
    // Snake's coordinates. Maximum length of snake is 8 units.
    unsigned char SnakeX[64] = {2,1,0};
    unsigned char SnakeY[64] = {2,2,2};
    
    // Snake's length (Begins with 3 units)
    int SnakeLength = 3;
    
    // Level variable
    int level = 0;
    
    int Lost = 0;
    
    // Food Coordinates for each level
    int FoodXList[64] = {6, 3, 5, 3, 7, 2, 5, 1, 4, 2, 4, 2, 6, 3, 4, 1, 2, 6, 3, 3, 1, 5, 6, 4, 3, 5, 4, 2, 5, 4, 2, 4, 6, 3, 5, 3, 7, 2, 5, 1, 4, 2, 4, 2, 6, 3, 4, 1, 2, 6, 3, 3, 1, 5, 6, 4, 3, 5, 4, 2, 5, 4, 2, 4};
    int FoodYList[64] = {2, 6, 3, 3, 1, 5, 6, 4, 3, 5, 4, 2, 5, 4, 2, 4, 6, 3, 5, 3, 7, 2, 5, 1, 4, 2, 4, 2, 6, 3, 4, 1, 2, 6, 3, 3, 1, 5, 6, 4, 3, 5, 4, 2, 5, 4, 2, 4, 6, 3, 5, 3, 7, 2, 5, 1, 4, 2, 4, 2, 6, 3, 4, 1};
    
    // Present food coordinates
    int FoodX = FoodXList[level];
    int FoodY = FoodYList[level];
            
    // Directions (Begins with RIGHT)
    char Direction = 'R';
    
    // Delay parameters for PRINT_SNAKE()
    double Cycle = 256;
    
    // Clean the board
    BOARD_CLEAN();
    
    // Wait until any button is pressed 
    while (!buttonRight && !buttonLeft && !buttonUp && !buttonDown) {
        
        PRINT_SNAKE(Cycle, SnakeLength, SnakeX, SnakeY, FoodX, FoodY, Direction);
    }
    
    while (buttonRight || buttonLeft || buttonUp || buttonDown);
    
    __delay_ms(150);
    
    // **************** GAME HAS STARTED ****************
    while (1) {
        
        // *************** EATING SELF ****************
        
        // If head's and any part of tail have same coordinates, game is over. Snake ate itself, you lost.
        for (int i = 1; i < SnakeLength - 1; i++) {
            
            if (SnakeX[i] == SnakeX[0] && SnakeY[i] == SnakeY[0]) {
                
                // Light all of the LEDs
                BOARD_FULL();
                
                // Wait for a second
                __delay_ms(1000);
                                
                Lost = 1;
            }
        }
        
        // *************** TAIL MOVEMENT CONTROL ****************
        
        // Each part of tail gets the value of the part which is in front of it.
        for (int i = SnakeLength - 1; i > 0; i--) {
                
                Direction = BUTTON_CONTROL(Direction);
                
                SnakeX[i] = SnakeX[i - 1];
                SnakeY[i] = SnakeY[i - 1];                        
            }
        
        // *************** HEAD MOVEMENT CONTROL ****************
        
        Direction = BUTTON_CONTROL(Direction);
        
        switch(Direction) {
            
            // Right
            case 'R':
                
                if (SnakeX[0] >= 7) {                
                    //Lost = 1;
                    
                    SnakeX[0] = 0;
                }
                
                else {
                    SnakeX[0] = SnakeX[0] + 1;
                }
                
                break;
            
            // Left
            case 'L':
                
                if (SnakeX[0] < 1) {
                    //Lost = 1;
                    
                    SnakeX[0] = 7;
                }

                else {
                    SnakeX[0] = SnakeX[0] - 1;
                }
                
                break;
            
            // Up
            case 'U':
                
                if (SnakeY[0] >= 7) {
                    //Lost = 1;
                    
                    SnakeY[0] = 0;
                }
                
                else {
                    SnakeY[0] = SnakeY[0] + 1;
                }
                
                break;
            
            // Down
            case 'D':
                
                if (SnakeY[0] < 1) {
                    //Lost = 1;
                    
                    SnakeY[0] = 7;
                }

                else {
                    SnakeY[0] = SnakeY[0] - 1;
                }   
                
                break;
        }
                
        // *************** PRINT SNAKE AND FOOD ***************
        
        PRINT_SNAKE(Cycle, SnakeLength, SnakeX, SnakeY, FoodX, FoodY, Direction);
                
        // *************** EATING FOOD ****************
        
        if ((FoodX == SnakeX[0]) && (FoodY == SnakeY[0])) {
            
            // If snake and food have the same coordinates, increase the length of sneak.
            // If the length is more than 7, game is over. You won.
            
            if (++SnakeLength < 10000) {
                
                // Level up
                level++;
                
                // New level food coordinates
                FoodX = FoodXList[level];
                FoodY = FoodYList[level];
                
                // Add new part to snake
                SnakeX[SnakeLength - 1] = 0;
                SnakeY[SnakeLength - 1] = 0;
            }
        }
        
        // Stop the game if we lost
        if (Lost == 1) {
            
            //Light all of the LEDs
            BOARD_FULL();
            __delay_ms(1000);
            
            while (!buttonRight && !buttonLeft && !buttonUp && !buttonDown) {
                PRINT_SNAKE(Cycle, SnakeLength, SnakeX, SnakeY, SnakeX[0], SnakeY[0], Direction);
            }
            
            __delay_ms(50);
            
            while (buttonRight || buttonLeft || buttonUp || buttonDown);
            __delay_ms(50);
            
            //Clean the board
            BOARD_CLEAN();
            
            //Stop the game
            break;
        }
        
    }
    
    // Wait until any button is pressed 
    
    
    while (!buttonRight && !buttonLeft && !buttonUp && !buttonDown);
    
    __delay_ms(150);
    
    return 0;
}

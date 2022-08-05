/* 
 * File:   main2.c
 * Author: Aida Tolosa
 *
 * Created on 29 de julio de 2022, 09:14 AM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


/*------------------------------------------------------------------------------
 * LIBRERIAS
 ------------------------------------------------------------------------------*/
#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include "oscilador.h"
#include "adc2.h"
#include "tmr0_2.h"
#include "LCD_LAB2.h"

#define _XTAL_FREQ 8000000


/*------------------------------------------------------------------------------
 * VARIABLES
 ------------------------------------------------------------------------------*/
uint8_t contador = 0;
uint8_t canal_ADC = 0;
uint8_t VAL_POT_0 = 0;
uint8_t VAL_POT_1 = 0;
uint8_t init_POT_0 = 0;
uint8_t dec_POT_0 = 0;
uint8_t init_POT_1 = 0;
uint8_t dec_POT_1 = 0;
unsigned short VOLTAJE_0 = 0;
unsigned short VOLTAJE_1 = 0;
char s[];

uint8_t bandera = 0;
uint8_t ciclo_trabajo = 0;
void setup(void);
unsigned short map(uint8_t val, uint8_t in_min, uint8_t in_max, 
            unsigned short out_min, unsigned short out_max);


void __interrupt() isr (void){
    if(INTCONbits.T0IF){ //Interrupción TMR0
        contador++;
        if(contador == 100){ //cuenta hasta 5s
            if(bandera == 1){ 
                bandera = 0;
            }
            else if(bandera == 0){ 
                bandera = 1;
            }
            contador = 0;
        }
        tmr0_reload();
    }
    if(PIR1bits.ADIF){ //Verificamos canal
        if(canal_ADC == 0){
            VAL_POT_0 = adc_read();
        }
        else if(canal_ADC == 1){
            VAL_POT_1 = adc_read();
        }
    }
    return;
}
/*------------------------------------------------------------------------------
 * CICLO PRINCIPAL
 ------------------------------------------------------------------------------*/
void main(void) {
    setup();
    unsigned int a;
    Lcd_Init(); //inicialización
    Lcd_Clear(); //limpiamos LCD
    //Lcd_Set_Cursor(1,1); 
    //Lcd_Write_String(" Aida Toloza "); 
    //Lcd_Set_Cursor(2,1); 
    //Lcd_Write_String("20949"); 
    //__delay_ms(2000); //esperamos 2 segundos
    while(1){
        if (canal_ADC == 0){ //primer canal
            adc_start(canal_ADC);
            canal_ADC = 1;
            VOLTAJE_0 = map(VAL_POT_0, 0, 255, 0, 500); //mapeamos el voltaje de 0 a 500
            init_POT_0 = VOLTAJE_0/100; 
            dec_POT_0 = VOLTAJE_0-init_POT_0*100; 
        }
        else if (canal_ADC == 1){//segundo canal
            adc_start(canal_ADC);
            canal_ADC = 0;
            VOLTAJE_1 = map(VAL_POT_1, 0, 255, 0, 500);
            init_POT_1 = VOLTAJE_1/100;
            dec_POT_1 = VOLTAJE_1-init_POT_1*100;
        }
        if(bandera == 1){
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("      POT1        ");
            Lcd_Set_Cursor(2,1);
            sprintf(s, "      %d.%dV     ", init_POT_0, dec_POT_0); //guardamos el string que vamos a mostrar
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String(s);
        }
        else if(bandera == 0){
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("      POT2      ");
            Lcd_Set_Cursor(2,1);
            sprintf(s, "      %d.%dV      ", init_POT_1, dec_POT_1);
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String(s);
        }
        
    }   
    return;
}

/*------------------------------------------------------------------------------
 * CONFIGURACION
 ------------------------------------------------------------------------------*/
void setup(void){
    TRISD = 0X00;
    TRISC = 0X00;
    ANSELH = 0;
    TRISB = 0;
    PORTB = 0;
    init_osc_MHz(3); 
    adc_init(1,0,0);
    PIR1bits.ADIF = 0;          // Limpiamos bandera de ADC
    PIE1bits.ADIE = 1;          // Habilitamos interrupcion de ADC
    INTCONbits.PEIE = 1;        // Habilitamos int. de perifericos
    INTCONbits.GIE = 1;         // Habilitamos int. globales
    PIE1bits.RCIE = 1;
    INTCONbits.T0IE = 1; //Habiliatamos int. TMR0
    tmr0_reload();   
    tmr0_init(256);  
}

unsigned short map(uint8_t x, uint8_t x0, uint8_t x1,
            unsigned short y0, unsigned short y1){
    return (unsigned short)(y0+((float)(y1-y0)/(x1-x0))*(x-x0));
}


/* 
 * File:   Oscilador.c
 * Author: Aida Tolosa
 *
 * Created on 29 de julio de 2022, 09:15 AM
 */

#include <xc.h>
#include "oscilador.h"

/*------------------------------------------------------------------------------
 * Funciones
------------------------------------------------------------------------------*/

void init_osc_MHz(uint8_t freq){
    OSCCONbits.SCS = 1;
    switch(freq){ 
        case 0:  //1MHz
            OSCCONbits.IRCF = 0b100;
            break;
        case 1: //2MHz
            OSCCONbits.IRCF = 0b101;
            break;
        case 2: //4MHz
            OSCCONbits.IRCF = 0b110;
            break;
        case 3: //8MHz
            OSCCONbits.IRCF = 0b111;
            break;
        case 4: //500kHz
            OSCCONbits.IRCF = 0b011;
            break;
        default: //4MHz
            OSCCONbits.IRCF = 0b110;
            break;
    }
}

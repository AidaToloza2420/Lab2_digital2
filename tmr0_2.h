/* 
 * File:   tmr0_2.h
 * Author: Aida Tolosa
 *
 * Created on 4 de agosto de 2022, 01:44 PM
 */

#ifndef TMR0_2_H
#define	TMR0_2_H

#include <xc.h>
#include <stdint.h>

void tmr0_init(uint16_t prescaler);
void tmr0_reload(void);

#endif	/* TMR0_2_H */


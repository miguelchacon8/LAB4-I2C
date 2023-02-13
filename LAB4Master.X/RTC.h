

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef RTC_H
#define	RTC_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include <pic16f887.h>
#include <stdint.h>


#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif


 

uint8_t conv(uint8_t valor);
uint8_t convh(uint8_t valor2);
void convertirHora(void);
void convertirFecha(void);   
void enviar_hora(void);
uint8_t leer_hora(void);
uint8_t leer_sec(void);
uint8_t leer_min(void);
uint8_t leer_dia(void);
uint8_t leer_mes(void);
uint8_t leer_ano(void);

void enviar_fecha(void);    
void leer_fecha(void);    
void mostrar_hora(void);
void mostrar_fecha(void);
    


#endif	/* XC_HEADER_TEMPLATE_H */


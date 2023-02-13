//MASTER
/*
 * File:   main.c
 * Author: Miguel Chacón
 * Programa: LAB 04 I2C
 * Fecha de creación: 09/02/2023
 */

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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

#include "I2C.h"
#include "LCD.h"
#include "mapf.h"
#include "oscilador.h"
#include "setupADC.h"
#include "RTC.h"
#include "conversiones.h"

#include <pic16f887.h>
#include <xc.h>
#include <stdio.h>
#include <string.h>

#define _XTAL_FREQ 8000000 //8MHz

//*****************************************************************************
// Definición de funciones 
//*****************************************************************************

void setup(void);
void valorLCD(void);
void calculovolt(void);
void modoboton(void);
void verificacion(void);

//*****************************************************************************
// Definición de variables 
//*****************************************************************************

//variables para botones
uint8_t modo = 0;
uint8_t cambio = 0;

//valores obtenidos
uint8_t V1 = 0;
uint8_t lecADC = 0;
uint8_t segundos = 0;
uint8_t minutos = 0;
uint8_t horas = 0;
uint8_t dias;
uint8_t meses;
uint8_t anos;

//valores para display
char useg;
char dseg;
char umin;
char dmin;
char uhora;
char dhora;
char udia;
char ddia;
char umes;
char dmes;
char uano;
char dano;

unsigned int VOLT1 = 0;
unsigned int u1 = 0;
unsigned int d1 = 0;

//VARIABLES PARA LCD
char buffer[20];

//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() ISR (void){
	
	if (RBIF == 1){
    if (PORTBbits.RB0 == 0)
    {
        __delay_ms(10);
        if (PORTBbits.RB0 == 1){ //incremento el puerto
            modo++;
            cambio = 0;
            INTCONbits.RBIF = 0;
        }
    }
    else if (PORTBbits.RB1 == 0){
        __delay_ms(10);
        if (PORTBbits.RB1 == 1){
            cambio++;
            INTCONbits.RBIF = 0;
		}
    }
    else if (PORTBbits.RB2 == 0){
        __delay_ms(10);
        if (PORTBbits.RB2 == 1){
            cambio = cambio + 10;
            INTCONbits.RBIF = 0;
    }
    }
    }
}
//******************************************************************************
// Código Principal
//******************************************************************************
void main(void) {
    setupINTOSC(7);         //Oscilador a 8MHz
    setup();
    Lcd_Init();// Inicializa la pantalla LCD
    __delay_us(100);
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String(" T");
    
    
    while(1){ 
        //LECTURA DEL ADC
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        V1 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(20);
        
        //CONVERSIONES DE VALORES
        VOLT1 = map(V1, 0, 255, 10, 30); //se mapea el valor 
        valorLCD();
        
        //Lectura de segundos
        segundos = leer_sec();
        useg = inttochar(descomponer(0, segundos));      
        dseg = inttochar(descomponer(1, segundos));
        //Lectura de minutos
        minutos = leer_min();
        umin = inttochar(descomponer(0, minutos));      
        dmin = inttochar(descomponer(1, minutos));
        //Lectura de horas
        horas = leer_hora();
        uhora = inttochar(descomponer(0, horas));      
        dhora = inttochar(descomponer(1, horas));
        //Lectura de dias
        dias = leer_dia();
        udia = inttochar(descomponer(0, dias));      
        ddia = inttochar(descomponer(1, dias));
        //Lectura de meses
        meses = leer_mes();
        umes = inttochar(descomponer(0, meses));      
        dmes = inttochar(descomponer(1, meses));
        //Lectura de años
        anos = leer_ano();
        uano = inttochar(descomponer(0, anos));      
        dano = inttochar(descomponer(1, anos));

        //FUNCIONES PARA MODOS E INCREMENTOS
        modoboton();

        __delay_ms(2);
    }
}
//******************************************************************************
// Función para configurar GPIOs
//******************************************************************************
void setup(void){
    //PUERTOS
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0b00000111;
    TRISA = 0;
    TRISD = 0;
    
    PORTA = 0; //inicializar puertos
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
    
        //Interrupciones
    INTCONbits.RBIE = 1; 
    INTCONbits.RBIF = 0;
    INTCONbits.GIE = 1; //interrupciones globales
    
    WPUBbits.WPUB0 = 1; //inputs
    WPUBbits.WPUB1 = 1;
    IOCBbits.IOCB0 = 1; //inputs
    IOCBbits.IOCB1 = 1;
    WPUBbits.WPUB2 = 1;
    IOCBbits.IOCB2 = 1; //inputs
    
    OPTION_REGbits.nRBPU = 0; //no RBPU, habilitan los pullups internos
    
    I2C_Master_Init(100000); //Se inicializa la comunicación I2C
}

//  FUNCION PARA OBTENER VALOR EN LCD
void valorLCD(void){
    //Escritura temperatura
    Lcd_Set_Cursor(2,1);
    sprintf(buffer, "%u", VOLT1); //ESCRIBO unidad V1
    Lcd_Write_String(buffer);
    Lcd_Set_Cursor(2,3);
    Lcd_Write_String("C");
    
    //ESCRITURA DE SEGUNDOS
    Lcd_Set_Cursor(1,15);//escribo unidades de seg
    Lcd_Write_Char(useg);
    Lcd_Set_Cursor(1,14);
    Lcd_Write_Char(dseg);
    Lcd_Set_Cursor(1,13);
    Lcd_Write_String(":");
    //ESCRITURA DE MINUTOS
    Lcd_Set_Cursor(1,12);
    Lcd_Write_Char(umin);
    Lcd_Set_Cursor(1,11);
    Lcd_Write_Char(dmin);
    Lcd_Set_Cursor(1,10);
    Lcd_Write_String(":");
    
    //ESCRITURA DE HORA
    Lcd_Set_Cursor(1,9);
    Lcd_Write_Char(uhora);
    Lcd_Set_Cursor(1,8);
    Lcd_Write_Char(dhora);
    
    //ESCRITURA DE DÍA
    Lcd_Set_Cursor(2,10);
    Lcd_Write_String("/");
    Lcd_Set_Cursor(2,9);
    Lcd_Write_Char(udia);
    Lcd_Set_Cursor(2,8);
    Lcd_Write_Char(ddia);
    
    //ESCRITURA DE MES
    Lcd_Set_Cursor(2,13);
    Lcd_Write_String("/");
    Lcd_Set_Cursor(2,12);
    Lcd_Write_Char(umes);
    Lcd_Set_Cursor(2,11);
    Lcd_Write_Char(dmes);
    
    //ESCRITURA DE AÑO
    Lcd_Set_Cursor(2,15);
    Lcd_Write_Char(uano);
    Lcd_Set_Cursor(2,14);
    Lcd_Write_Char(dano);
    
    //Despliege de modo 
    Lcd_Set_Cursor(1,5);
    sprintf(buffer, "%u", modo);  //ESCRIBO decena V1
    Lcd_Write_String(buffer);
}
//******************************************************************************
// Función para calcular valores de unidades y decenas:
//******************************************************************************
void modoboton(void){
    if (modo == 1){         //cambio de minutos
        minutos = cambio; 
        verificacion();
        
        I2C_Master_Start();            //Incia comunicaión I2C
        I2C_Master_Write(0xD0);        //Escoje dirección del reloj
        I2C_Master_Write(0x01);        //Posición de los minutos
        I2C_Master_Write(minutos);         //Posición donde va leer
        I2C_Master_Stop();             //Detiene la comunicaión I2C
    }
    else if(modo == 2){   //cambio de horas
        horas = cambio;
        verificacion();
        I2C_Master_Start();            //Incia comunicaión I2C
        I2C_Master_Write(0xD0);        //Escoje dirección del reloj
        I2C_Master_Write(0x02);        //Posición de las horas
        I2C_Master_Write(horas);         //Posición donde va leer
        I2C_Master_Stop();             //Detiene la comunicaión I2C
        verificacion();
    }
    else if(modo == 3){     //cambio de dias
        dias = cambio;
        verificacion();
        I2C_Master_Start();            //Incia comunicaión I2C
        I2C_Master_Write(0xD0);        //Escoje dirección del reloj
        I2C_Master_Write(0x04);        //Posición de los dias
        I2C_Master_Write(dias);         //Posición donde va leer
        I2C_Master_Stop();             //Detiene la comunicaión I2C    
    }
    else if(modo == 4){     //cambio de meses
        meses = cambio;
        verificacion();
        I2C_Master_Start();            //Incia comunicaión I2C
        I2C_Master_Write(0xD0);        //Escoje dirección del reloj
        I2C_Master_Write(0x05);        //Posición de los segundos
        I2C_Master_Write(meses);         //Posición donde va leer
        I2C_Master_Stop();             //Detiene la comunicaión I2C    
    }
    else if(modo == 5){         //cambio de años
        anos = cambio;
        I2C_Master_Start();            //Incia comunicaión I2C
        I2C_Master_Write(0xD0);        //Escoje dirección del reloj
        I2C_Master_Write(0x06);        //Posición de los segundos
        I2C_Master_Write(anos);         //Posición donde va leer
        I2C_Master_Stop();             //Detiene la comunicaión I2C    
    }
    else if (modo > 5){
        modo = 0;
    }
}

void verificacion(void){
    if (minutos > 59){
        minutos = 0;
        cambio = 0;
    }
    else if (horas > 11){
        horas = 0;
        cambio = 0;
    }
    else if (meses > 12){
        meses = 0;
        cambio = 0;
    }
    else if (dias > 31){
        dias = 0;
        cambio = 0;
    }
}
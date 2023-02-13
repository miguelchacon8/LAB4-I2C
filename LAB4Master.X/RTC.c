/*
 * File:   RTC.c
 * Author: Migue
 *
 * Created on 10 de febrero de 2023, 11:21 AM
 */


#include <xc.h>
#include <pic16f887.h>
#include <stdint.h>

#include "I2C.h"
#include "RTC.h"
#define _XTAL_FREQ  8000000


uint8_t sec, min, hora;
uint8_t dia, mes, ano;
uint8_t valor;
uint8_t valor2;

uint8_t conv(uint8_t valor){
   
    uint8_t un;
    uint8_t dec;
    uint8_t convertido;
    un = valor & 0x0F;
    dec = ((valor & 0xF0)>>4);
    convertido = un+(dec*10);
    return convertido;
}

uint8_t convh(uint8_t valor2){
   
    uint8_t unhora;
    uint8_t dechora;
    uint8_t convertidohora;
    unhora = valor2 & 0x0F;
    dechora = ((valor2 & 0x10)>>4);
    convertidohora = unhora+(dechora*10);
    return convertidohora;
}


uint8_t leer_hora(void){
    
    I2C_Master_Start();            //Incia comunicaión I2C
    I2C_Master_Write(0xD0);        //Escoje dirección del reloj
    I2C_Master_Write(0x02);        //Posición donde va leer
    I2C_Master_RepeatedStart();          //Reinicia la comuniación I2C
    I2C_Master_Write(0xD1);        //Leer posición
    hora = I2C_Master_Read(0);      //lee posicion de reloj
    I2C_Master_Stop();             //Termina comunicaion I2C
    
    hora = convh(hora);
    return hora;
}

uint8_t leer_min(void){
    
    I2C_Master_Start();            //Incia comunicaión I2C
    I2C_Master_Write(0xD0);        //Escoje dirección del reloj
    I2C_Master_Write(0x01);        //Posición donde va leer
    I2C_Master_RepeatedStart();          //Reinicia la comuniación I2C
    I2C_Master_Write(0xD1);        //Leer posición

    min = I2C_Master_Read(0);      //lee posicion de reloj
    I2C_Master_Stop();             //Termina comunicaion I2C
    
    min = conv(min);
    return min;
}

uint8_t leer_sec(void){
    
    I2C_Master_Start();            //Incia comunicaión I2C
    I2C_Master_Write(0xD0);        //Escoje dirección del reloj
    I2C_Master_Write(0x00);        //Posición donde va leer
    I2C_Master_RepeatedStart();          //Reinicia la comuniación I2C
    I2C_Master_Write(0xD1);        //Leer posición
    sec = I2C_Master_Read(0);      //lee posicion de reloj
    I2C_Master_Stop();             //Termina comunicaion I2C
    
    sec = conv(sec);
    return sec;
}

uint8_t leer_dia(void){
    
    I2C_Master_Start();            //Incia comunicaión I2C
    I2C_Master_Write(0xD0);        //Escoje dirección del reloj
    I2C_Master_Write(0x04);        //Posición donde va leer
    I2C_Master_RepeatedStart();          //Reinicia la comuniación I2C
    I2C_Master_Write(0xD1);        //Leer posición
    dia = I2C_Master_Read(0);      //lee posicion de reloj
    I2C_Master_Stop();             //Termina comunicaion I2C
    
    dia = conv(dia);
    return dia;
}

uint8_t leer_mes(void){
    
    I2C_Master_Start();            //Incia comunicaión I2C
    I2C_Master_Write(0xD0);        //Escoje dirección del reloj
    I2C_Master_Write(0x05);        //Posición donde va leer
    I2C_Master_RepeatedStart();          //Reinicia la comuniación I2C
    I2C_Master_Write(0xD1);        //Leer posición
    mes = I2C_Master_Read(0);      //lee posicion de reloj
    I2C_Master_Stop();             //Termina comunicaion I2C
    
    mes = convh(mes);
    return mes;
}
uint8_t leer_ano(void){
    
    I2C_Master_Start();            //Incia comunicaión I2C
    I2C_Master_Write(0xD0);        //Escoje dirección del reloj
    I2C_Master_Write(0x06);        //Posición donde va leer
    I2C_Master_RepeatedStart();          //Reinicia la comuniación I2C
    I2C_Master_Write(0xD1);        //Leer posición
    ano = I2C_Master_Read(0);      //lee posicion de reloj
    I2C_Master_Stop();             //Termina comunicaion I2C
    
    ano = conv(ano);
    return ano;
}

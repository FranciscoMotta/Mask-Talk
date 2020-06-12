/*
 * File:   Principal.c
 * Author: Francisco Motta
 *
 * Created on 11 de junio de 2020, 09:38 AM
 */

#include <xc.h>
#include <stdint.h>
#include "configuracionFusibles.h"
#include "ADC_Inicializar.h"
#include "LCD_2X16.h"

#define _XTAL_FREQ 20000000UL

//DEFINICION DE LOS PINES DE CONTROL DE LA MATRIZ

#define DATA_OUT LATCbits.LATC0
#define CLOCK_OUT LATCbits.LATC1
#define PUERTOS_OUT LATB

#DEFINICION DE LAS FUNCIONES DE TRABAJO

void pinesInicio(void);
void pulsoReloj(void);
void talkMatrix(void);
void notTalk(void);
void Escribir_Valor_En_Lcd(uint16_t valor);

//Principal

void main(void) {
    Iniciar_Lcd(); //Iniciamos el LCD para ver los datos procedentes del microfono
    pinesInicio(); //Iniciamos los pines de trabajo
    Lcd_Escribir_Cadena("HOLA"); // Mensaje de prueba para comprobar el funcionamiento del LCD
    ADC_inicializacion(ADC_ON); //Activamos el conversos análogo digital del pic18f4550
    char var = 0x00; //Creamos una variable del tipo caracter para almacenar los datos 
    int datoDeCuentaXD = 0; //Creamos otra variable para que sirva de conteo para las repeticiones
PASO:
    Posicion_Linea_Superior(0); //Nos dirigimos a la primer linea del LCD
    var = Leer_ADC(CANAL_AN0); //En var guardamos la lectura del canal AN0 donde estaría el microfono 
     /*PARA ESTAR PARTE ES IMPORTANTE REVISAR EL PROTEUS ADJUNTO PARA VER EL SISTEMA DE AMPLIFICACIÓN*/
    Escribir_Valor_En_Lcd(var); //Mostramos el valor de voltaje producido por el microfono al hablar o hacer silencio
      /* NOTAS:
       * Esta parte depende mucho del microfono que uses o del sistema de amplificación que emplees para obtener el voltaje del microfono
       * Como se añadió el LCD , tu puedes ver allí el valor que se registra al hablar o al hacer silencio y poner ello en las condiciones de 
       * funcionamiento
      */
    if (var <= 43) { //En este caso, el microfono registra 0.43 volts cuando hay silencio, osea que a partir de ese voltaje se considera que hablamos
        notTalk(); //Si el dato es menor, mostramos la configuración en el LCD que aparenta estar en silencio
    } else { // De lo contrario, mostramos el movimiento de los labios
        for (datoDeCuentaXD = 0; datoDeCuentaXD < 80; datoDeCuentaXD++) { //Estas son las repeticiones para apreciar mejor el movimiento de los labios
            talkMatrix(); //Mostramos el movimiento
        }
    }
    goto PASO; //Regresamos a hacer todo otra vez
    return;
}

void pulsoReloj(void) {
    CLOCK_OUT = 1;
    __delay_us(100);
    CLOCK_OUT = 0;
    __delay_us(100);
}

void pinesInicio(void) {
    TRISC = 0x00;
    TRISB = 0x00;
    TRISD = 0x00;
}

void Escribir_Valor_En_Lcd(uint16_t valor) {
    int Centenas_Del_Valor_Introducido = 0;
    int Decenas_Del_Valor_Introducido = 0;
    int Unidades_Del_Valor_Introducido = 0;

    /*FRACCIONAMIENTO EN DECENAS, UNIDADES Y CENTENAS*/
    Centenas_Del_Valor_Introducido = valor / 100; /*Dividimos en 100 para quedarnos con solo las centenas como parte entera*/
    Decenas_Del_Valor_Introducido = (valor % 100) / 10; /*Sacámos el módulo 100 al valor introducido y luego lo dividimos entre 10 para quedanos con las decenas como parte entera*/
    Unidades_Del_Valor_Introducido = (valor % 100) % 10; /*Sacámos el módulo 100 al valor introducido y sacamos el módulo 10 para quedanos con las unidades como parte entera*/

    Lcd_Escribir_Caracter(Centenas_Del_Valor_Introducido + 0x30); /*Se muestra el dato correspondiente y se le adiciona 0X30 para convertirlo a ASCII*/
    Lcd_Escribir_Caracter('.');
    Lcd_Escribir_Caracter(Decenas_Del_Valor_Introducido + 0x30); /*Se muestra el dato correspondiente y se le adiciona 0X30 para convertirlo a ASCII*/
    Lcd_Escribir_Caracter(Unidades_Del_Valor_Introducido + 0x30); /*Se muestra el dato correspondiente y se le adiciona 0X30 para convertirlo a ASCII*/
    Lcd_Escribir_Caracter('v');

    return;
}

void notTalk(void) {
    DATA_OUT = 1;
    pulsoReloj();
    DATA_OUT = 0;
    PUERTOS_OUT = ~0x18;
    pulsoReloj();
    PUERTOS_OUT = ~0x18;
    pulsoReloj();
    PUERTOS_OUT = ~0x18;
    pulsoReloj();
    PUERTOS_OUT = ~0x18;
    pulsoReloj();
    PUERTOS_OUT = ~0x18;
    pulsoReloj();
    PUERTOS_OUT = ~0x18;
    pulsoReloj();
    PUERTOS_OUT = ~0x18;
    pulsoReloj();
    PUERTOS_OUT = ~0X18;
    pulsoReloj();
}

void talkMatrix(void) {
    DATA_OUT = 1;
    pulsoReloj();
    DATA_OUT = 0;
    PUERTOS_OUT = ~0x24;
    pulsoReloj();
    PUERTOS_OUT = ~0x42;
    pulsoReloj();
    PUERTOS_OUT = ~0x81;
    pulsoReloj();
    PUERTOS_OUT = ~0x81;
    pulsoReloj();
    PUERTOS_OUT = ~0x42;
    pulsoReloj();
    PUERTOS_OUT = ~0x24;
    pulsoReloj();
    PUERTOS_OUT = ~0x18;
    pulsoReloj();
    PUERTOS_OUT = ~0X18;
    pulsoReloj();
}

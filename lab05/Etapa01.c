#include <delays.h>
#include <P18F4550>
#include "teclado.h"

// Configuracoes
#pragma config FOSC = HS  // Fosc = 20MHz, Tcy = 200ns (PLL nao habilitado)
// sinal de clk do sistema e o oscliador primario
#pragma config CPUDIV = OSC1_PLL2
#pragma config PWRT = ON  // Power Up Timer habilitado
#pragma config WDT = OFF  // Watchdog desativado
#pragma config LVP = OFF  // Desabilita gravacao em baixa tensao

// #define USANDO_O_KIT
#ifndef USANDO_O_KIT
#define CC1 LATAbits.LATA5  // Controle Disp 1
#define CC2 LATAbits.LATA2  // Controle Disp 2
#define CC3 LATEbits.LATE0  // Controle Disp 3
#define CC4 LATEbits.LATE2  // Controle Disp 4
#else
#define CC1 LATAbits.LATA2  // Controle Disp 1
#define CC2 LATAbits.LATA3  // Controle Disp 2
#define CC3 LATAbits.LATA4  // Controle Disp 3
#define CC4 LATAbits.LATA5  // Controle Disp 4
#endif

char const disp7segNums[] = {
    // 0bPGGEDCBA => codigo
    0b00111111,  // 00
    0b00000110,  // 01
    0b01011011,  // 02
    0b01001111,  // 03
    0b01100110,  // 04
    0b01101101,  // 05
    0b01111101,  // 06
    0b00000111,  // 07
    0b01111111,  // 08
    0b01100111,  // 09
    0b01110111,  // 10 - A
    0b01111100,  // 11 - B
    0b00111001,  // 12 - C
    0b01011110,  // 13 - D
    0b01111001,  // 14 - E
    0b01110001,  // 15 - F
    0b01000000,  // 16 - traco
    0b10000000,  // 17 - pontinho
};

char codigoDoBtnPressionado = 17;  // comeca com pontinho

void main(void) {
  // Configura portas como saida:
  TRISA = 0b00000000;
  TRISE = 0b00000000;
  TRISD = 0b00000000;
  // Configura PORTB: metade saida metade entrada e Pull Ups desligados
  TRISB = 0b11110000;
  INTCON2 = 0;
  // Tudo digital por favor
  ADCON1 = 0x0F;

  // Liga somente o ultimo display
  CC1 = 0; CC2 = 0; CC3 = 0; CC4 = 1;

  while (1) {
    // Verifica qual botao esta pressionado
    codigoDoBtnPressionado = getKey();
    // Espera bounce (25*1000*200ns = 5ms)
    Delay1kTCY(25);

    // Se tiver btn pressionado: atualiza o display
    if (codigoDoBtnPressionado != ' ') LATD = disp7segNums[codigoDoBtnPressionado];
    // Senao limpa o display
    else LATD = disp7segNums[17];
  }
}

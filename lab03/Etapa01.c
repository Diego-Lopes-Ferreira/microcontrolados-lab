#include <P18F4550.h>
#include <delays.h>

#pragma config FOSC = HS
#pragma config CPUDIV = OSC1_PLL2
#pragma config PWRT = ON
#pragma config WDT = OFF
#pragma config LVP = OFF

#define USANDO_O_KIT
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

#define BTN0 PORTBbits.RB0
#define BTN1 PORTBbits.RB1
#define BTN2 PORTBbits.RB2

char const tab[] = {
    0b01011011,  // 2 (0)
    0b01001111,  // 3 (1)
    0b01101101,  // 5 (2)
    0b00000111,  // 7 (3)
};

// 0: solto | 1: pressionado
char btn1Pressionado = 0;
char btn2Pressionado = 0;
char contador = 0;
char soma = 1;
char qualDisplay = 4;

unsigned char aux = 0;

void main(void) {
  TRISA = 0b00000000;
  TRISE = 0b00000000;
  TRISD = 0b00000000;
  ADCON1 = 0x0F;  // Todas Portas digitais
  while (1) {
    // BOTAO 1
    if (BTN1 == 0 && btn1Pressionado == 0) {
      btn1Pressionado = 1;
      if (soma == -1)
        soma = 1;
      else if (soma == 1)
        soma = -1;
      Delay1KTCYx(25);  // 5ms
    } else if (BTN1 == 1 && btn1Pressionado == 1) {
      btn1Pressionado = 0;
    }

    // BOTAO 2
    if (BTN2 == 0 && btn2Pressionado == 0) {
      btn2Pressionado = 1;
      qualDisplay--;
      if (qualDisplay == 5) qualDisplay = 1;
      if (qualDisplay == 0) qualDisplay = 4;
      Delay1KTCYx(25);  // 5ms
    } else if (BTN2 == 1 && btn2Pressionado == 1) {
      btn2Pressionado = 0;
    }

    // ATUALIZA O CONTADOR
    contador += soma;
    if (contador == 4) contador = 0;
    if (contador == -1) contador = 3;

    // ATUALIZA QUAL DISPLAY ESTA ATIVO
    CC1 = 0;
    CC2 = 0;
    CC3 = 0;
    CC4 = 0;
    switch (qualDisplay) {
      case 1:
        CC1 = 1;
        break;
      case 2:
        CC2 = 1;
        break;
      case 3:
        CC3 = 1;
        break;
      case 4:
        CC4 = 1;
        break;
    }
    LATD = tab[contador];

    // ESPERA 1 SEGUNDO (50ms * 20 = 1s)
    for (aux = 0; aux < 20; aux++) Delay1KTCYx(250);  // 50ms
  }
}

/*
  Delay1KTCYx(x)
  x * 1000 * 200ns = valor do delay
  Exemplo:
  200 * 1000 * 200ns = 50ms
*/

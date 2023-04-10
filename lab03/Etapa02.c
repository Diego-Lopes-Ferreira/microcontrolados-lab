#include<P18F4550.h>
#include<delays.h>

#pragma config  FOSC = HS           // Fosc = 20MHz, Tcy = 200ns (PLL nao habilitado)
#pragma config  CPUDIV = OSC1_PLL2  // sinal de clk do sistema e o oscliador primario
#pragma config  PWRT = ON           // Power Up Timer habilitado
#pragma config  WDT = OFF           // Watchdog desativado
#pragma config  LVP = OFF           // Desabilita gravacao em baixa tensao

// #define USANDO_O_KIT
#ifndef USANDO_O_KIT
  #define CC1  LATAbits.LATA5  // Controle Disp 1
  #define CC2  LATAbits.LATA2  // Controle Disp 2
  #define CC3  LATEbits.LATE0  // Controle Disp 3
  #define CC4  LATEbits.LATE2  // Controle Disp 4
#else
  #define CC1  LATAbits.LATA2  // Controle Disp 1
  #define CC2  LATAbits.LATA3  // Controle Disp 2
  #define CC3  LATAbits.LATA4  // Controle Disp 3
  #define CC4  LATAbits.LATA5  // Controle Disp 4
#endif

#define DELAY_DOS_LEDS 5

#define BTN0 PORTBbits.RB0
#define BTN1 PORTBbits.RB1
#define BTN2 PORTBbits.RB2

char const tab[] ={
  0b00111111,    //0
  0b00000110,    //1
  0b01011011,    //2
  0b01001111,    //3
  0b01100110,    //4
  0b01101101,    //5
  0b01111101,    //6
  0b00000111,    //7
  0b01111111,    //8
  0b01100111     //9
};

// 0: solto | 1: pressionado
char btn0Pressionado = 0;
char btn1Pressionado = 0;
char btn2Pressionado = 0;

int contagem = 5600;
int aux = 0;
int segundosUnidade = 0;
int segundosDezena = 0;
int miliUnidade = 0;
int miliDezena = 0;

char estamosContando = 1;
char soma = 1;
char qualDisplay = 4;
unsigned char auxDosLeds = 0;

void main(void) {
  TRISA = 0b00000000;
  TRISE = 0b00000000;
  TRISD = 0b00000000;
  ADCON1 = 0x0F;       //Todas Portas digitais
  CC1 = 0;
  CC2 = 0;
  CC3 = 0;
  CC4 = 0;
  while(1) {
    // BOTAO 0
    if (BTN0 == 0 && btn0Pressionado == 0) {
      btn0Pressionado = 1;
      if (estamosContando == 0) estamosContando = 1;
      else if (estamosContando == 1) estamosContando = 0;
      Delay1KTCYx(25); // 5ms
    } else if (BTN0 == 1 && btn0Pressionado == 1) {
      btn0Pressionado = 0;
      Delay1KTCYx(25); // 5ms
    }

    if (estamosContando) contagem--; // 00.00
    if (contagem > 5999) contagem = 0;
    if (contagem == 0) contagem = 5600;

    // CONVERTER O NUMERO PARA OS DIGITOS INDIVIDUAIS
    aux = contagem;
    segundosDezena = aux / 1000;
    aux -= (segundosDezena * 1000);
    segundosUnidade = aux / 100;
    aux -= (segundosUnidade * 100);
    miliDezena = aux / 10;
    aux -= (miliDezena * 10);
    miliUnidade = aux;

    // ATUALIZA OS VALORES DOS DISPLAYS
    auxDosLeds = 0;
    while (auxDosLeds < 2) {
      CC1 = 1;
      LATD = tab[segundosDezena];
      Delay1KTCYx(DELAY_DOS_LEDS);
      CC1 = 0;

      CC2 = 1;
      LATD = tab[segundosUnidade];
      Delay1KTCYx(DELAY_DOS_LEDS);
      CC2 = 0;

      CC3 = 1;
      LATD = tab[miliDezena];
      // LATD = tab[0];
      Delay1KTCYx(DELAY_DOS_LEDS);
      CC3 = 0;

      CC4 = 1;
      LATD = tab[miliUnidade];
      Delay1KTCYx(DELAY_DOS_LEDS);
      CC4 = 0;

      auxDosLeds++;
    }

    // ESPERA 1 SEGUNDO (50ms * 20 = 1s)
    // for (aux= 0; aux < 20; aux++) Delay1KTCYx(250); // 50ms
  }
}

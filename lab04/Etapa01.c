#include<P18F4550.h>
#include<delays.h>

// Configuracoes
#pragma config  FOSC = HS           // Fosc = 20MHz, Tcy = 200ns (PLL nao habilitado)
#pragma config  CPUDIV = OSC1_PLL2  // sinal de clk do sistema e o oscliador primario
#pragma config  PWRT = ON           // Power Up Timer habilitado
#pragma config  WDT = OFF           // Watchdog desativado
#pragma config  LVP = OFF           // Desabilita gravacao em baixa tensao

// #define EUSEILA
#ifndef EUSEILA
  #define CC1  LATAbits.LATA5    // Controle Disp 1
  #define CC2  LATAbits.LATA2    // Controle Disp 2
  #define CC3  LATEbits.LATE0    // Controle Disp 3
  #define CC4  LATEbits.LATE2    // Controle Disp 4
#else
  #define CC1  LATAbits.LATA2    // Controle Disp 1
  #define CC2  LATAbits.LATA3    // Controle Disp 2
  #define CC3  LATAbits.LATA4    // Controle Disp 3
  #define CC4  LATAbits.LATA5    // Controle Disp 4
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

unsigned int contagem = 43190; // 43190

unsigned int aux = 0;
unsigned int segundos = 0;
unsigned int minutos = 0;
unsigned int horas = 0;

unsigned int segundosUnidade = 0;
unsigned int segundosDezena = 0;
unsigned int minutoUnidade = 0;
unsigned int minutoDezena = 0;
unsigned int horaUnidade = 0;
unsigned int horaDezena = 0;

char estamosContando = 1;
char soma = 1;
char qualDisplay = 4;
unsigned char auxDosLeds = 0;

void main(void) {
  TRISA = 0b00000000;
  TRISE = 0b00000000;
  TRISD = 0b00000000;
  ADCON1 = 0x0F;       // Todas Portas digitais
  CC1 = 0;
  CC2 = 0;
  CC3 = 0;
  CC4 = 0;
  while(1) {
    // BOTAO 0
    //if (BTN0 == 0 && btn0Pressionado == 0) {
    //  btn0Pressionado = 1;
    //  if (estamosContando == 0) estamosContando = 1;
    //  else if (estamosContando == 1) estamosContando = 0;
    //  Delay1KTCYx(25); // 5ms
    //} else if (BTN0 == 1 && btn0Pressionado == 1) {
    //  btn0Pressionado = 0;
    //  Delay1KTCYx(25); // 5ms
    //}

    if (estamosContando) contagem++; // 00.00
    if (contagem == 0) contagem = 43199;
    if (contagem > 43199) contagem = 0;

    // CONVERTER O NUMERO PARA OS DIGITOS INDIVIDUAIS
    // 1. Converte pra hms
    aux = contagem;
    horas = aux / 3600;
    aux = aux - (horas * 3600);
    minutos = aux / 60;
    aux = aux - (minutos * 60);
    segundos = aux;

    // 2. Converte pros numeros individuais
    segundosDezena = segundos / 10;
    segundos -= (segundosDezena * 10);
    segundosUnidade = segundos;

    minutoDezena = minutos / 10;
    minutos -= (minutoDezena * 10);
    minutoUnidade = minutos;

    horaDezena = horas / 10;
    horas -= (horaDezena * 10);
    horaUnidade = horas;

    // ATUALIZA OS VALORES DOS DISPLAYS
    auxDosLeds = 0;
    while (auxDosLeds < 250) {
      CC1 = 1;
      LATD = tab[horaDezena];
      Delay1KTCYx(DELAY_DOS_LEDS);
      CC1 = 0;

      CC2 = 1;
      LATD = tab[horaUnidade];
      Delay1KTCYx(DELAY_DOS_LEDS);
      CC2 = 0;

      CC3 = 1;
      LATD = tab[minutoDezena];
      // LATD = tab[0];
      Delay1KTCYx(DELAY_DOS_LEDS);
      CC3 = 0;

      CC4 = 1;
      LATD = tab[minutoUnidade];
      Delay1KTCYx(DELAY_DOS_LEDS);
      CC4 = 0;

      auxDosLeds++;
    }

    // ESPERA 1 SEGUNDO (50ms * 20 = 1s)
    // for (aux= 0; aux < 20; aux++) Delay1KTCYx(250); // 50ms
  }
}

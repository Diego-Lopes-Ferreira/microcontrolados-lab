#include <P18F4550.h>
#include <delays.h>
#include <timers.h>

// Configuracoes
#pragma config FOSC = HS  // Fosc = 20MHz, Tcy = 200ns (PLL nao habilitado)
                          // sinal de clk do sistema e o oscliador primario
#pragma config CPUDIV = OSC1_PLL2
#pragma config PWRT = ON  // Power Up Timer habilitado
#pragma config WDT = OFF  // Watchdog desativado
#pragma config LVP = OFF  // Desabilita gravacao em baixa tensao

#define USANDO_PICSIM
#ifndef USANDO_PICSIM
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

#define DELAY_DOS_LEDS 5  // Delay1KTCYx = 5 * 1000 * 200ns = 1ms
#define DELAY_DOS_BTNS 5  // Delay1KTCYx = 5 * 1000 * 200ns = 1ms
#define BTN0 PORTBbits.RB0
#define BTN1 PORTBbits.RB1
#define BTN2 PORTBbits.RB2

char const tab[] = {
    0b00111111,  // 0
    0b00000110,  // 1
    0b01011011,  // 2
    0b01001111,  // 3
    0b01100110,  // 4
    0b01101101,  // 5
    0b01111101,  // 6
    0b00000111,  // 7
    0b01111111,  // 8
    0b01100111   // 9
};

// 0: solto | 1: pressionado
char btn0Pressionado = 0;
char btn1Pressionado = 0;
char btn2Pressionado = 0;

unsigned int contagem = 43190;  // 43190s = 11:59:50

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
int tempo = 1;
char qualDisplay = 4;
unsigned char auxDosLeds = 0;
unsigned char maxDosLeds = 250;

void main(void) {
  TRISA = 0b00000000;
  TRISE = 0b00000000;
  TRISD = 0b00000000;
  ADCON1 = 0x0F;  // Todas Portas digitais
  CC1 = 0;
  CC2 = 0;
  CC3 = 0;
  CC4 = 0;
  OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_256);
  while (1) {
    if (ReadTimer0() > 0x4C4B) {
      // A cada 4 clocks e a cada 256 vezes ele soma, ou seja:
      // 200ns * 256 * 4 = 204.8 microsegundos
      // Se eu preciso de 1s: 1s / 204.8 microsegundos = 19531 (e uns quebrado)
      // 19531 => 0x4C4B
      WriteTimer0(0);
      if (estamosContando) contagem++;
    }

    // BOTAO 0
    if (BTN0 == 0 && btn0Pressionado == 0) {
      btn0Pressionado = 1;
      if (estamosContando == 0) contagem += 3600;  // incrementa as horas
      Delay1KTCYx(DELAY_DOS_BTNS);
    } else if (BTN0 == 1 && btn0Pressionado == 1) {
      btn0Pressionado = 0;
    }

    // BOTAO 1
    if (BTN1 == 0 && btn1Pressionado == 0) {
      btn1Pressionado = 1;
      if (estamosContando == 0) contagem += 60;  // incrementa os minutos
      Delay1KTCYx(DELAY_DOS_BTNS);
    } else if (BTN1 == 1 && btn1Pressionado == 1) {
      btn1Pressionado = 0;
    }

    // BOTAO 2
    if (BTN2 == 0 && btn2Pressionado == 0) {
      btn2Pressionado = 1;
      if (estamosContando == 0)
        estamosContando = 1;
      else
        estamosContando = 0;
      Delay1KTCYx(DELAY_DOS_BTNS);
    } else if (BTN2 == 1 && btn2Pressionado == 1) {
      btn2Pressionado = 0;
    }

    if (contagem == 0) contagem = 43199;  // 43199s = 11:59:59
    if (contagem > 43199) contagem -= 43199;

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

    /* ATUALIZA OS VALORES DOS DISPLAYS
        Cada Delay1KTCYx esta configurado para 1ms
        ou seja, esse processo todo leva 4ms
    */
    CC1 = 1;
    LATD = tab[minutoDezena];
    Delay1KTCYx(DELAY_DOS_LEDS);
    CC1 = 0;

    CC2 = 1;
    LATD = tab[minutoUnidade];
    Delay1KTCYx(DELAY_DOS_LEDS);
    CC2 = 0;

    CC3 = 1;
    LATD = tab[segundosDezena];
    // LATD = tab[0];
    Delay1KTCYx(DELAY_DOS_LEDS);
    CC3 = 0;

    CC4 = 1;
    LATD = tab[segundosUnidade];
    Delay1KTCYx(DELAY_DOS_LEDS);
    CC4 = 0;
  }
}

#include <P18F4550.h>
#include <delays.h>

#include "xlcd_exsto.h"

// Configuracoes
#pragma config FOSC = HSPLL_HS     // HS (cristal 20MHz) e PLL ativado
#pragma config PLLDIV = 5          // Prescaler do PLL = 20MHz / 5 = 4MHz
#pragma config CPUDIV = OSC4_PLL6  // Fosc -> 96 do PLL / 6 = 16MHz

#pragma config WDT = OFF     // Watchdog desativado
#pragma config PBADEN = OFF  // Pinos do PORTB comecam como digitais
#pragma config LVP = OFF     // Desabilita gravacao em baixa tensao
#pragma config DEBUG = ON    // habilita debug

#define DELAY_DOS_BTNS 100
#define BTN0 PORTBbits.RB0
#define BTN1 PORTBbits.RB1
#define BTN2 PORTBbits.RB2

// 0: solto | 1: pressionado
char btn0Pressionado = 0;
char btn1Pressionado = 0;
char btn2Pressionado = 0;

unsigned char segundos = 0;
unsigned char minutos = 0;
unsigned char horas = 0;

char estamosContando = 1;

void main(void) {
  TRISE = 0x00;
  TRISD = 0x00;
  ADCON1 = 0x0F;  // Todas as portas como digitais

  // INICIALIZACAO LCD
  // Comunicacao nibble e linha para cursor
  OpenXLCD(FOUR_BIT & LINES_5X7);
  WriteCmdXLCD(0x01);  // comando para limpar a tela
  // Delay necessario para a inicializacao do LCD (10000*200n*8 = 16ms)
  Delay10KTCYx(8);

  WriteCmdXLCD(0x0C);    // Comando para desligar cursor
  WriteCmdXLCD(0x80);    // Comando para colocar na primeira linha
  putrsXLCD("Relogio");  // Escreve a string salva em ROM

  while (1) {
    // BOTAO 0: Coloca no modo de configuracao
    if (BTN0 == 0 && btn0Pressionado == 0) {
      btn0Pressionado = 1;
      if (estamosContando == 0)
        estamosContando = 1;
      else
        estamosContando = 0;
      Delay1KTCYx(DELAY_DOS_BTNS);
    } else if (BTN0 == 1 && btn0Pressionado == 1) {
      btn0Pressionado = 0;
    }

    // BOTAO 1: incrementa as horas
    if (BTN1 == 0 && btn1Pressionado == 0) {
      btn1Pressionado = 1;
      if (estamosContando == 0) horas += 1;  // incrementa uma hora
      Delay1KTCYx(DELAY_DOS_BTNS);
    } else if (BTN1 == 1 && btn1Pressionado == 1) {
      btn1Pressionado = 0;
    }

    // BOTAO 2: incrementa minutos
    if (BTN2 == 0 && btn2Pressionado == 0) {
      btn2Pressionado = 1;
      if (estamosContando == 0) minutos += 1;  // incrementa um minuto
      Delay1KTCYx(DELAY_DOS_BTNS);
    } else if (BTN2 == 1 && btn2Pressionado == 1) {
      btn2Pressionado = 0;
    }

    if (estamosContando) {
      // Atraso de 1seg:  201*10000*248 ns = 0,5 s
      Delay10KTCYx(201);
      Delay10KTCYx(201);
      segundos++;
    }

    // se estourar os segundos
    if (segundos >= 60) {
      segundos = 0;
      minutos += 1;
    }
    // se estourar os minutos
    if (minutos >= 60) {
      minutos = 0;
      horas += 1;
    }
    // se estourar as horas
    if (horas >= 24) {
      segundos = 0;
      minutos = 0;
      horas = 0;
    }

    // Mostra no display
    WriteCmdXLCD(0x80);  // Comando para colocar na primeira linha
    if (estamosContando)
      putrsXLCD("Andando  ");
    else
      putrsXLCD("Parado   ");
    WriteCmdXLCD(0xC0);  // Comando para colocar na segunda linha
    putcXLCD(0x30 + (horas / 10));
    putcXLCD(0x30 + (horas % 10));
    putcXLCD(':');
    putcXLCD(0x30 + (minutos / 10));
    putcXLCD(0x30 + (minutos % 10));
    putcXLCD(':');
    putcXLCD(0x30 + (segundos / 10));
    putcXLCD(0x30 + (segundos % 10));
  }
}

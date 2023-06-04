#include <P18F4550.h>
#include <delays.h>

#include "xlcd_exsto.h"

// Configuracoes
#pragma config FOSC = HSPLL_HS     // HS (cristal 20MHz) e PLL ativado
#pragma config PLLDIV = 5          // Prescaler do PLL = 20MHz / 5 = 4MHz
#pragma config CPUDIV = OSC4_PLL6  // Fosc -> 96 do PLL / 6 = 16MHz

#pragma config WDT = OFF    // Watchdog desativado
#pragma config WDTPS = 512  // Watchdog 512 x 4ms = 2.048s

#pragma config PBADEN = OFF  // Pinos do PORTB comecam como digitais
#pragma config LVP = OFF     // Desabilita gravacao em baixa tensao
#pragma config DEBUG = ON    // habilita debug

#define DELAY_DOS_BTNS 100
#define BTN0 PORTBbits.RB0
#define BTN1 PORTBbits.RB1
#define BTN2 PORTBbits.RB2
#define BTN3 PORTBbits.RB3
#define BTN4 PORTBbits.RB4
#define BUZZER_PIN PORTCbits.RC2
#define LEDR PORTAbits.RA3
#define LEDG PORTAbits.RA5

#define TRANSLATE_N 0x30

// 0: solto | 1: pressionado
char btn0Pressionado = 0;
char btn1Pressionado = 0;
char btn2Pressionado = 0;
char btn3Pressionado = 0;
char btn4Pressionado = 0;

char menu = 0;
char buzzer_on = 0;
char ajuste = 0;
char aux = 0;

char segundos = 0;
char minutos = 0;
char horas = 0;

void print2N(char n) {
  putcXLCD(TRANSLATE_N + (n / 10));
  putcXLCD(TRANSLATE_N + (n % 10));
}

void main(void) {
  TRISE = 0x00;
  TRISD = 0x00;
  TRISA = 0x00;
  TRISC = 0;
  TRISB = 0x07;
  INTCON2bits.NOT_RBPU = 0;
  ADCON1 = 0x0F;         // Todas as portas como digitais
  OSCCONbits.IDLEN = 0;  // vai para sleep

  // INICIALIZACAO LCD
  // Comunicacao nibble e linha para cursor
  OpenXLCD(FOUR_BIT & LINES_5X7);
  WriteCmdXLCD(0x01);  // comando para limpar a tela
  // Delay necessario para a inicializacao do LCD (10000*200n*8 = 16ms)
  Delay10KTCYx(8);

  WriteCmdXLCD(0x0C);  // Comando para desligar cursor
  WriteCmdXLCD(0x80);  // Comando para colocar na primeira linha
  putrsXLCD("oi");     // Escreve a string salva em ROM

  while (1) {
    ClrWdt();
    // BOTAO 0: Coloca no modo de configuracao
    if (BTN0 == 0 && btn0Pressionado == 0) {
      btn0Pressionado = 1;
      switch (menu) {
        case 0:
          ajuste += 1;
          if (ajuste > 3) ajuste = 0;
          break;
        case 1:
          buzzer_on = 1;
          break;
        case 2:
          WriteCmdXLCD(0x80);  // Comando para colocar na primeira linha
          putrsXLCD("sleeping   ");
          Delay1KTCYx(30);  // 1000 * 30 * 250ns = ??
          Sleep();
          break;
      }
      Delay1KTCYx(DELAY_DOS_BTNS);
    } else if (BTN0 == 1 && btn0Pressionado == 1) {
      btn0Pressionado = 0;
      Delay1KTCYx(DELAY_DOS_BTNS);
    }

    // BOTAO 1
    if (BTN1 == 0 && btn1Pressionado == 0) {
      btn1Pressionado = 1;
      if (!ajuste) {
        // ajuste do menu
        menu -= 1;
        if (menu < 0) menu = 2;
      } else {
        // ajuste das horas
        switch (ajuste) {
          case 1:
            segundos -= 1;
            break;
          case 2:
            minutos -= 1;
            break;
          case 3:
            horas -= 1;
            break;
        }
      }
      Delay1KTCYx(DELAY_DOS_BTNS);
    } else if (BTN1 == 1 && btn1Pressionado == 1) {
      btn1Pressionado = 0;
      Delay1KTCYx(DELAY_DOS_BTNS);
    }

    // BOTAO 2
    if (BTN2 == 0 && btn2Pressionado == 0) {
      btn2Pressionado = 1;
      if (!ajuste) {
        // ajuste do menu
        menu += 1;
        if (menu > 2) menu = 0;
      } else {
        // ajuste das horas
        switch (ajuste) {
          case 1:
            segundos += 1;
            break;
          case 2:
            minutos += 1;
            break;
          case 3:
            horas += 1;
            break;
        }
      }
      Delay1KTCYx(DELAY_DOS_BTNS);
    } else if (BTN2 == 1 && btn2Pressionado == 1) {
      btn2Pressionado = 0;
      Delay1KTCYx(DELAY_DOS_BTNS);
    }

    // AJUSTE DAS HORAS
    if (ajuste == 0) {
      // Atraso de 1seg:  201*10000*248 ns = 0,5 s
      Delay10KTCYx(201);
      Delay10KTCYx(201);
      segundos++;
    }
    if (segundos >= 60) {
      segundos = 0;
      minutos += 1;
    } else if (segundos < 0) {
      segundos = 59;
      minutos -= 1;
    }
    if (minutos >= 60) {
      minutos = 0;
      horas += 1;
    } else if (minutos < 0) {
      minutos = 59;
      horas -= 1;
    }
    if (horas >= 24) {
      horas = 0;
    } else if (horas < 0) {
      horas = 24;
    }

    // CONTROLE DO BUZZER
    if (buzzer_on) {
      aux = 0;
      while (aux < 100) {
        BUZZER_PIN = 1;
        Delay1KTCYx(10);
        BUZZER_PIN = 0;
        Delay1KTCYx(10);
        aux += 1;
      }
      buzzer_on = 0;
    }

    // LED COLORIDO
    if (ajuste == 0) {
      LEDG = 1;
      LEDR = 0;
    } else {
      LEDG = 0;
      LEDR = 1;
    }

    // Mostra no display
    WriteCmdXLCD(0x80);  // Comando para colocar na primeira linha
    switch (menu) {
      case 0:
        switch (ajuste) {
          case 0:
            putrsXLCD("[1/3] Ajustar ");
            break;
          case 1:
            putrsXLCD("[1/3] Segundos");
            break;
          case 2:
            putrsXLCD("[1/3] Minutos ");
            break;
          case 3:
            putrsXLCD("[1/3] Horas   ");
            break;
        }
        break;
      case 1:
        putrsXLCD("[2/3] PIII    ");
        break;
      case 2:
        putrsXLCD("[3/3] Sleep   ");
        break;
    }

    WriteCmdXLCD(0xC0);  // Comando para colocar na segunda linha
    print2N(horas);
    putcXLCD(':');
    print2N(minutos);
    putcXLCD(':');
    print2N(segundos);
  }
}

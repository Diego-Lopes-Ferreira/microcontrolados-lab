#include <P18F4550.h>
#include <delays.h>

#include "xlcd_exsto.h"

// Configuracoes
#pragma config FOSC = HS  // HS (cristal 20MHz) e PLL ativado
// #pragma config PLLDIV = 5          // Prescaler do PLL = 20MHz / 5 = 4MHz
// #pragma config CPUDIV = OSC4_PLL6  // Fosc -> 96 do PLL / 6 = 16MHz

#pragma config WDT = ON      // Watchdog desativado
#pragma config WDTPS = 512   // Watchdog 512 x 4ms = 2.048s
#pragma config PBADEN = OFF  // Pinos do PORTB comecam como digitais
#pragma config LVP = OFF     // Desabilita gravacao em baixa tensao
#pragma config DEBUG = OFF   // debug

#define DELAY_DOS_BTNS 100
#define BTN0 PORTBbits.RB0
#define BTN1 PORTBbits.RB1
#define BTN2 PORTBbits.RB2
#define BUZZER_PIN PORTCbits.RC2
#define LEDR PORTAbits.RA3
#define LEDG PORTAbits.RA5

// Prototipos das Funcoes de Interrupcao
void ISR_Baixa_Prioridade(void);
void ISR_Alta_Prioridade(void);

#pragma code high_vector = 0x08
void int_alta(void) { _asm GOTO ISR_Alta_Prioridade _endasm }
#pragma code

#pragma code low_vector = 0x18
void int_baixa(void) { _asm GOTO ISR_Baixa_Prioridade _endasm }
#pragma code

char menu = 0;
char buzzer_on = 0;
char ajuste = 0;
char aux = 0;

char segundos = 0;
char minutos = 0;
char horas = 0;

void print2N(char n) {
  putcXLCD(0x30 + (n / 10));
  putcXLCD(0x30 + (n % 10));
}

#pragma interrupt ISR_Alta_Prioridade
void ISR_Alta_Prioridade(void) {
  // INT0
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
  INTCONbits.INT0IF = 0;
}

#pragma interruptlow ISR_Baixa_Prioridade
void ISR_Baixa_Prioridade(void) {
  if (INTCON3bits.INT1IF) {
    // Ocorrencia de INT1
    if (ajuste == 0) {
      // ajuste do menu
      menu -= 1;
      if (menu < 0) menu = 2;
    } else {
      // ajuste das horas
      if (ajuste == 1)
        segundos -= 1;
      else if (ajuste == 2)
        minutos -= 1;
      else if (ajuste == 3)
        horas -= 1;
    }
    INTCON3bits.INT1IF = 0;
  } else if (INTCON3bits.INT2IF) {
    // Ocorrencia de INT2
    if (ajuste == 0) {
      // ajuste do menu
      menu += 1;
      if (menu > 2) menu = 0;
    } else {
      // ajuste das horas
      if (ajuste == 1)
        segundos += 1;
      else if (ajuste == 2)
        minutos += 1;
      else if (ajuste == 3)
        horas += 1;
    }
    INTCON3bits.INT2IF = 0;
  }
}

void main(void) {
  TRISA = 0x00;
  TRISB = 0b00001111;
  TRISC = 0x00;
  TRISD = 0x00;
  TRISE = 0x00;
  INTCON2bits.NOT_RBPU = 0;  // Habilita pull-ups PORTB
  ADCON1 = 0x0F;             // Todas as portas como digitais
  OSCCONbits.IDLEN = 0;      // vai para sleep

  // Configuracao das interrupcoes
  RCONbits.IPEN = 1;  // habilita prioridade

  // Configuracao de INT0
  INTCONbits.INT0IE = 1;  // habilita INT0
  INTCONbits.INT0IF = 0;  // zera flag da INT0
                          // (sempre de alta prioridade)

  // Configuracao de INT1
  INTCON3bits.INT1IE = 1;  // habilita INT1
  INTCON3bits.INT1IF = 0;  // zera flag da INT1
  INTCON3bits.INT1IP = 0;  // interrupcao INT1 e de baixa prioridade

  // Configuracao de INT2
  INTCON3bits.INT2IE = 1;  // habilita INT2
  INTCON3bits.INT2IF = 0;  // zera flag da INT2
  INTCON3bits.INT2IP = 0;  // interrupcao INT2 e de baixa prioridade

  INTCONbits.GIEL = 1;  // habilita interrupcoes de perifericos
  INTCONbits.GIEH = 1;  // habilita interrupcoes geral

  // INICIALIZACAO LCD
  OpenXLCD(FOUR_BIT & LINES_5X7);  // Comunicacao nibble e linha para cursor
  WriteCmdXLCD(0x01);              // comando para limpar a tela
  Delay10KTCYx(8);  // Delay da inicializacao do LCD (10000*200n*8 = 16ms)

  WriteCmdXLCD(0x0C);  // Comando para desligar cursor
  WriteCmdXLCD(0x80);  // Comando para colocar na primeira linha
  putrsXLCD("oi");     // Escreve a string salva em ROM

  while (1) {
    ClrWdt();

    // AJUSTE DAS HORAS
    if (ajuste == 0) {
      // Atraso de 1seg:  200ns * 10000 * 250 = 500ms
      Delay10KTCYx(250);
      Delay10KTCYx(250);
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
      LEDG = 1;
      LEDR = 0;
      aux = 0;
      while (aux < 100) {
        BUZZER_PIN = 1;
        Delay1KTCYx(10);
        BUZZER_PIN = 0;
        Delay1KTCYx(10);
        aux += 1;
      }
      buzzer_on = 0;
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

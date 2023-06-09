// Lab08/Etapa01.c
#include <P18F4550.h>
#include <delays.h>
#include <timers.h>

#include "funcoes.h"
#include "teclado.h"
#include "xlcd_exsto.h"

// Configuracoes
#pragma config FOSC = HS  // HS (cristal 20MHz)

#pragma config WDT = OFF     // Watchdog desativado
#pragma config WDTPS = 512   // Watchdog 512 x 4ms = 2.048s
#pragma config PBADEN = OFF  // Pinos do PORTB comecam como digitais
#pragma config LVP = OFF     // Desabilita gravacao em baixa tensao
#pragma config DEBUG = OFF   // debug

char menu = 0;
char ajuste = 0;
char aux = 0;
char estouEnviandoSerial = 0;

char segundos = 0;
char minutos = 0;
char horas = 0;

char btn0Pressionado = 0;
char btn1Pressionado = 0;
char btn2Pressionado = 0;

char teclaDigitada = ' ';

void main(void) {
  TRISA = 0x00;
  TRISB = 0b00001111;
  TRISC = 0x00;
  TRISD = 0x00;
  TRISE = 0x00;
  INTCON2bits.NOT_RBPU = 0;  // Habilita pull-ups PORTB
  ADCON1 = 0x0F;             // Todas as portas como digitais
  OSCCONbits.IDLEN = 0;      // vai para sleep

  configura_isr();

  // Configuracao do TIMER0
  // T = TCY * Prescaler * (MAX - INICIAL)
  // 1s = 200ns * 256 * (65536 - 46004)
  OpenTimer0(TIMER_INT_ON &   // habilita interrupcao
             T0_16BIT &       // 16 bits
             T0_SOURCE_INT &  // fonte clock interno
             T0_PS_1_256);    // prescaler em 256
  INTCON2bits.TMR0IP = 1;     // interrupcao TMR0 e de alta prioridade
  WriteTimer0(46004);         // CARGA INICIAL = 46004

  // Config Registradores USART
  TXSTAbits.SYNC = 0;     // Modo assincrono
  BAUDCONbits.BRG16 = 0;  // 8-bit Baud Rate Generator
  TXSTAbits.BRGH = 1;     // Baud rate = high speed
  RCSTAbits.SPEN = 1;     // Serial port enabled
  TXSTAbits.TXEN = 1;     // Transmit enabled
  SPBRG = 0x40;           // 64
  SPBRGH = 0x00;

  // INICIALIZACAO LCD
  OpenXLCD(FOUR_BIT & LINES_5X7);  // Comunicacao nibble e linha para cursor
  WriteCmdXLCD(0x01);              // comando para limpar a tela
  Delay10KTCYx(8);  // Delay da inicializacao do LCD (10000*200ns*8 = 16ms)

  WriteCmdXLCD(0x0C);   // Comando para desligar cursor
  WriteCmdXLCD(0x80);   // Comando para colocar na primeira linha
  putrsXLCD("Lab 08");  // Escreve a string salva em ROM
  while (1) {
    limpa_flags();

    if (estouEnviandoSerial == 1) {
      teclaDigitada = getKey();
      if (teclaDigitada != ' ') {
        if (teclaDigitada > 57) {
          INTCONbits.INT0IE = 1;   // habilita INT0
          INTCON3bits.INT1IE = 1;  // habilita INT1
          INTCON3bits.INT2IE = 1;  // habilita INT2
          estouEnviandoSerial = 0;
        } else {
          // Se estiver enviando ainda, faca nada:
          while (TXSTAbits.TRMT == 0) {
          }
          TXREG = teclaDigitada;
        }
      }
    }

    ajusta_horas();
    mostraMenuLCD();
  }
}

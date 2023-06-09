// Lab09/Etapa02.c
// DOIS PWMs

#include <P18F4550.h>
#include <delays.h>
#include <timers.h>

#include "xlcd_exsto.h"

// Configuracoes
#pragma config FOSC = HS     // HS (cristal 20MHz) => TCY = 200ns
#pragma config WDT = OFF     // Watchdog desativado
#pragma config WDTPS = 512   // Watchdog 512 x 4ms = 2.048s
#pragma config PBADEN = OFF  // Pinos do PORTB comecam como digitais
#pragma config LVP = OFF     // Desabilita gravacao em baixa tensao
#pragma config DEBUG = OFF   // debug

char btn0Pressionado;
char btn1Pressionado;

unsigned int pwm_ventoinha = 0;
unsigned int pwm_lampada = 0;

void ajusta_dc(unsigned int dc_cpp1, unsigned int dc_cpp2) {
  /*
    >> 0 = 01 23456789
    >> 1 = 00 12345678
    >> 2 = 00 01234567
  */
  CCPR1L = (char)(dc_cpp1 >> 2);    // Retira os dois bits menos significativos
  CCP1CONbits.DC1B0 = dc_cpp1 % 2;  // Utiliza o LSB
  dc_cpp1 = dc_cpp1 >> 1;           // Retira o LSB
  CCP1CONbits.DC1B1 = dc_cpp1 % 2;  // Utiliza o proximo bit

  CCPR2L = (char)(dc_cpp2 >> 2);    // Retira os dois bits menos significativos
  CCP2CONbits.DC2B0 = dc_cpp2 % 2;  // Utiliza o LSB
  dc_cpp2 = dc_cpp2 >> 1;           // Retira o LSB
  CCP2CONbits.DC2B1 = dc_cpp2 % 2;  // Utiliza o proximo bit
}

void ISR_Baixa_Prioridade(void);
void ISR_Alta_Prioridade(void);

#pragma code high_vector = 0x08
void int_alta(void) { _asm GOTO ISR_Alta_Prioridade _endasm }
#pragma code

#pragma code low_vector = 0x18
void int_baixa(void) { _asm GOTO ISR_Baixa_Prioridade _endasm }
#pragma code

#pragma interrupt ISR_Alta_Prioridade
void ISR_Alta_Prioridade(void) {
  if (INTCONbits.INT0IF) {  // INT 0
    INTCONbits.INT0IF = 0;
    if (btn0Pressionado == 1) return;
    btn0Pressionado = 1;

    pwm_ventoinha += 10;
    if (pwm_ventoinha > 100) pwm_ventoinha = 0;
  } else if (INTCON3bits.INT1IF) {
    INTCON3bits.INT1IF = 0;
    if (btn1Pressionado == 1) return;

    btn1Pressionado = 1;
    pwm_lampada += 10;
    if (pwm_lampada > 100) pwm_lampada = 0;
  }
}

#pragma interruptlow ISR_Baixa_Prioridade
void ISR_Baixa_Prioridade(void) {}

void main(void) {
  TRISA = 0x00;
  TRISB = 0b00001111;
  TRISC = 0x00;
  TRISD = 0x00;
  TRISE = 0x00;
  ADCON1 = 0x0F;             // Todas as portas como digitais
  INTCON2bits.NOT_RBPU = 0;  // Habilita pull-ups PORTB

  OSCCONbits.IDLEN = 0;  // vai para sleep

  RCONbits.IPEN = 1;  // habilita prioridade

  INTCONbits.INT0IE = 1;  // habilita INT0
  INTCONbits.INT0IF = 0;  // zera flag da INT0
                          // (sempre de alta prioridade)

  INTCON3bits.INT1IE = 1;  // habilita INT1
  INTCON3bits.INT1IF = 0;  // zera flag da INT1
  INTCON3bits.INT1IP = 1;  // interrupcao INT1 e de alta prioridade

  // Configuracoes do Timer 2: Prescaller de 16
  T2CON = 0b00000111;
  PR2 = 61;

  // Modo PWM: active high
  CCP1CON = 0b00001100;
  CCP2CON = 0b00001100;

  INTCONbits.GIEL = 1;  // habilita int. perifericos
  INTCONbits.GIEH = 1;  // habilita int.

  // INICIALIZACAO LCD
  OpenXLCD(FOUR_BIT & LINES_5X7);  // Comunicacao nibble e linha para cursor
  WriteCmdXLCD(0x01);              // comando para limpar a tela
  Delay10KTCYx(8);  // Delay da inicializacao do LCD (10000*200ns*8 = 16ms)

  WriteCmdXLCD(0x0C);             // Comando para desligar cursor
  WriteCmdXLCD(0x80);             // Comando para colocar na primeira linha
  putrsXLCD("Lab 09 - Etapa 2");  // Escreve a string salva em ROM

  while (1) {
    WriteCmdXLCD(0x80);        // primeira linha
    putrsXLCD("Ventoinha: ");  // Escreve a string salva em ROM
    putcXLCD(0x30 + (pwm_ventoinha / 10));
    putcXLCD(0x30 + (pwm_ventoinha % 10));

    WriteCmdXLCD(0xC0);        // segunda linha
    putrsXLCD("Lampada  : ");  // Escreve a string salva em ROM
    putcXLCD(0x30 + (pwm_lampada / 10));
    putcXLCD(0x30 + (pwm_lampada % 10));

    /*
      100 = 62
      pwm = D

      => D = pwm * 62 / 100
    */
    ajusta_dc(pwm_lampada * 4 * 62 / 100, pwm_ventoinha * 4 * 62 / 100);
    analogwrite(255);

    // Limpar as flags:
    if (btn0Pressionado == 1) {
      Delay1KTCYx(20);  // 200ns * 1000 * 20 = 4ms
      btn0Pressionado = 0;
    }

    if (btn1Pressionado == 1) {
      Delay1KTCYx(20);  // 200ns * 1000 * 20 = 4ms
      btn1Pressionado = 0;
    }
  }
}

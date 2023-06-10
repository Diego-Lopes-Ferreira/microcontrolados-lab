#include "globals.h"
#include "main.h"

void configura_perifericos(void) {
  RCONbits.IPEN = 1;  // habilita prioridade

  // TIMER0
  // T = TCY * Prescaler * (MAX - INICIAL)
  // 10ms = 200ns * 256 * (255 - 60)
  OpenTimer0(TIMER_INT_ON &   // habilita interrupcao
             T0_8BIT &        // 16 bits
             T0_SOURCE_INT &  // fonte clock interno
             T0_PS_1_256);    // prescaler em 256
  INTCON2bits.TMR0IP = 1;     // interrupcao TMR0 e de alta prioridade
  WriteTimer0(60);

  // TIMER1
  // T = TCY * Prescaler * (MAX - INICIAL)
  // 10ms = 200ns * 8 * (65536 - 3036)
  OpenTimer1(TIMER_INT_ON &   // Interrupcao Habilitada
             T1_16BIT_RW &    // 16 bits
             T1_SOURCE_INT &  // clock interno
             T1_PS_1_8);      // 1:8
  PIR1bits.TMR1IF = 0;        // limpa flag
  WriteTimer1(3036);

  // USART
  TXSTAbits.SYNC = 0;     // Modo assincrono
  BAUDCONbits.BRG16 = 0;  // 8-bit Baud Rate Generator
  TXSTAbits.BRGH = 1;     // Baud rate = high speed
  RCSTAbits.SPEN = 1;     // Serial port enabled
  TXSTAbits.TXEN = 1;     // Transmit enabled
  // n = [Fosc / (16 * BAUDRATE)] - 1
  // n = [20000000 / (16 * 19200)] - 1
  SPBRG = 0x40;  // 64 = 19200 bits/s
  SPBRGH = 0x00;

  // CONVERSOR A/D
  ADCON0 = 0b00000001;  // enable canal 0
  ADCON1 = 0b00001110;  // Referencia Tensao de Alimentacao e RA0 Analogico
  ADCON2 = 0b10111101;  // direita | Tempo de aquisicao: 20 | Fosc/16
  PIE1bits.ADIE = 1;    // Interrupcao habilitada
  IPR1bits.ADIP = 1;    // alta prioridade
  PIR1bits.ADIF = 0;    // limpa flag

  // PWM
  T2CON = 0b00000111;  // Timer 2: Prescaller de 16
  PR2 = 99;            // (99+1) * 200ns * 16 = 320us = 3125Hz
  // Modo PWM: active high
  CCP1CON = 0b00001100;
  CCP2CON = 0b00001100;

  INTCONbits.GIEL = 1;  // habilita interrupcoes de perifericos
  INTCONbits.GIEH = 1;  // habilita interrupcoes geral
}

void envia_serial(const rom char* mensagem) {
  while (*mensagem) {
    while (TXSTAbits.TRMT == 0) {
    }
    TXREG = *mensagem;
    mensagem++;
  }
}

void envia_numero_serial(char numero, char tres_casas) {
  if (tres_casas) {
    while (TXSTAbits.TRMT == 0) {
    }
    TXREG = ((numero / 10) / 10) + 0x30;
  }

  while (TXSTAbits.TRMT == 0) {
  }
  TXREG = ((numero / 10) - 10 * ((numero / 10) / 10)) + 0x30;

  while (TXSTAbits.TRMT == 0) {
  }
  TXREG = (numero % 10) + 0x30;
}

void ajusta_dc_1(unsigned int dc_cpp1) {
  //  10 bits: CCPRL DC1B1 DC1B0
  //  >> 0 = 01 23456789 => % 2 = 9 => DC1B1
  //  >> 1 = 00 12345678 => % 2 = 8 => DC1B0
  //  >> 2 = 00 01234567 => (char) = 01234567 => CCPRL
  // dc_cpp1 = dc_cpp1 * 4 * (PR2 + 1) / 100;
  dc_cpp1 = dc_cpp1 * 4;  // otimizado pois PR2 = 99
  CCPR1L = (char)(dc_cpp1 >> 2);
  CCP1CONbits.DC1B0 = dc_cpp1 % 2;
  dc_cpp1 = dc_cpp1 >> 1;
  CCP1CONbits.DC1B1 = dc_cpp1 % 2;
}

void ajusta_dc_2(unsigned int dc_cpp2) {
  // dc_cpp2 = dc_cpp2 * 4 * (PR2 + 1) / 100;
  dc_cpp2 = dc_cpp2 * 4 * (PR2 + 1) / 100;  // otimizado pois PR2 = 99
  CCPR2L = (char)(dc_cpp2 >> 2);
  CCP2CONbits.DC2B0 = dc_cpp2 % 2;
  dc_cpp2 = dc_cpp2 >> 1;
  CCP2CONbits.DC2B1 = dc_cpp2 % 2;
}

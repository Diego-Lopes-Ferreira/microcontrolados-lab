#include "globals.h"
#include "main.h"

void configura_timers(void) {
  // TIMER0
  // T = TCY * Prescaler * (MAX - INICIAL)
  // 10ms = 200ns * 256 * (255 - 60)
  OpenTimer0(TIMER_INT_ON &   // habilita interrupcao
             T0_8BIT &        // 16 bits
             T0_SOURCE_INT &  // fonte clock interno
             T0_PS_1_256);    // prescaler em 256
  INTCON2bits.TMR0IP = 1;     // interrupcao TMR0 e de alta prioridade
  WriteTimer0(60);

  // TIMER1 (A/D)
  // T = TCY * Prescaler * (MAX - INICIAL)
  // 1ms = 200ns * 8 * (65536 - 64911)
  OpenTimer1(TIMER_INT_ON &   // Interrupcao Habilitada
             T1_16BIT_RW &    // 16 bits
             T1_SOURCE_INT &  // clock interno
             T1_PS_1_8);      // 1:8
  PIR1bits.TMR1IF = 0;        // limpa flag
  WriteTimer1(64911);

  // TIMER1 (PWM)
  // Timer 2: Prescaller de 16
  T2CON = 0b00000111;
}

void configura_perifericos(void) {
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
  ADCON0 = 0b00000001;  // Canal0=RA0 (xx0000) flag inicio (0) A/D Ativado (1)
  ADCON1 = 0b00001110;  // Vref-=Vss e Vref+=Vdd (xx00) | RA0 analogico (1110)
  ADCON2 = 0b10111101;  // Direita (1x) | Taq: 20 (111) | Fosc/16 (101)
  PIE1bits.ADIE = 1;    // Interrupcao habilitada
  IPR1bits.ADIP = 1;    // alta prioridade
  PIR1bits.ADIF = 0;    // limpa flag

  // PWM
  // Timer 2: Prescaller de 16
  PR2 = 99;  // (99+1) * 200ns * 16 = 320us = 3125Hz
  // Modo PWM: active high
  CCP1CON = 0b00001100;
  CCP2CON = 0b00001100;
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

void controla_temperatura(void) {
  long valor_medido;
  unsigned int pwm1_erro_anterior;
  unsigned int A = 1;
  unsigned int B = 2;

  // 10bits xxxxxx01 00000000
  valor_medido = (256 * (unsigned char)ADRESH) + (unsigned char)ADRESL;
  tensao = 5000 * (long)valor_medido / 1023;  // 0-1023 = 0-5000mV
  temperatura_atual = (long)tensao / 10;      // LM35 = 10mV/deg

  pwm1_erro_anterior = pwm1_erro;
  pwm1_erro = temperatura_alvo - temperatura_atual;
  pwm1_anterior = pwm1;

  // u = [u_1] + [A*(e_0 - e_1)] + [B*(e_0)]
  // A   => ganho integrativo
  // B   => ganho proporcional

  pwm1 += A * (pwm1_erro - pwm1_erro_anterior) + B * (pwm1_erro);
  if (pwm1 > 100) {
    pwm1 = 100;
  }
  if (pwm1 > 65000) {
    pwm1 = 0;
  }
}

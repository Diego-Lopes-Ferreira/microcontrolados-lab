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
  // 100ms = 200ns * 8 * (65536 - 36)

  // NOTE(DIEGO): Por alguma razao a funcao pronta trava o PWM2, entao
  //              o TMR1 sera configurado na mao mesmo...

  // 16bits (1) | Clock not from tmr1 osc (0) | Prescale 1:8 (11)
  // Enable tmr1 osc (0) | Sync (x) | Clock Source internal (0) | On/Off (1)
  T1CON = 0b10110001;
  PIE1bits.TMR1IE = 1;  // liga interrupcao
  PIR1bits.TMR1IF = 0;  // limpa flag
  TMR1H = 0;
  TMR1L = 36;

  // TIMER2 (PWM)
  // Postscale 1:1(x0000) | Timer2 On (1) | Prescale 1:16 (1x)
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

void envia_numero_serial(int numero, char casas_decimais) {
  if (casas_decimais >= 4) {
    while (TXSTAbits.TRMT == 0) {
    }
    TXREG = 0x30 + numero / 1000;  // X000
  }
  if (casas_decimais >= 3) {
    while (TXSTAbits.TRMT == 0) {
    }
    TXREG = 0x30 + (numero % 1000) / 100;  // 0X00
  }
  if (casas_decimais >= 2) {
    while (TXSTAbits.TRMT == 0) {
    }
    TXREG = 0x30 + ((numero % 1000) % 100) / 10;  // 00X0
  }
  if (casas_decimais >= 1) {
    while (TXSTAbits.TRMT == 0) {
    }
    TXREG = 0x30 + ((numero % 1000) % 100) % 10;  // 000X
  }
}

void ajusta_dc_1(int dc_cpp1) {
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

void ajusta_dc_2(int dc_cpp2) {
  // dc_cpp2 = dc_cpp2 * 4 * (PR2 + 1) / 100;
  dc_cpp2 = dc_cpp2 * 4;  // otimizado pois PR2 = 99
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
  if (tensao > 1500) {
    tensao = 1500;  // limite do LM35
  }
  temperatura_atual = (long)tensao / 10;  // LM35 = 10mV/deg

  pwm1_erro_anterior = pwm1_erro;
  pwm1_erro = temperatura_alvo - temperatura_atual;
  pwm1_anterior = pwm1;

  // u = [u_1] + [A*(e_0 - e_1)] + [B*(e_0)]
  // A   => ganho integrativo
  // B   => ganho proporcional

  pwm1 += A * (pwm1_erro - pwm1_erro_anterior) + B * (pwm1_erro);
  if (pwm1 > 100) {
    pwm1 = 99;  // 100% fica estranho na simulacao
  }
  if (pwm1 < 0) {
    pwm1 = 0;
  }
}

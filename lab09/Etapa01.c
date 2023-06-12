// Lab09/Etapa01.c
// Código testa

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

char modo = 0;

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
  if (INTCONbits.TMR0IF) {  // TIMER 0
    if (modo == 0) {
      // CPPR = (1/440) / (200ns * 8) = 1420 = 0x058C
      modo = 1;
      CCPR1H = 0x01;  // 440Hz
      CCPR1L = 0x63;
    } else if (modo == 1) {
      // CPPR = (1/880) / (200ns * 8) = 710  = 0x02C6
      modo = 0;
      CCPR1H = 0x05;  // 880Hz
      CCPR1L = 0x8C;
    }

    WriteTimer0(55771);
    INTCONbits.TMR0IF = 0;
  } else if (PIR1bits.CCP1IF) {  // CPP VIU QUE O TIMER 1 BATEU O VALOR DEFINIDO
    PIR1bits.CCP1IF = 0;
    LATCbits.LATC2 = !LATCbits.LATC2;
    WriteTimer1(0);
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

  // Configuracao do TIMER0
  // T = TCY * Prescaler * (MAX - INICIAL)
  // 0,5s = 200ns * 256 * (65536 - 55771)
  OpenTimer0(TIMER_INT_ON &   // habilita interrupcao
             T0_16BIT &       // 16 bits
             T0_SOURCE_INT &  // fonte clock interno
             T0_PS_1_256);    // prescaler em 256
  INTCON2bits.TMR0IP = 1;     // interrupcao TMR0 e de alta prioridade
  WriteTimer0(55771);          // CARGA INICIAL = 55771

  // Configuracao do TIMER1
  OpenTimer1(TIMER_INT_OFF &  // desabilita interrupcao
             T1_16BIT_RW &    // 16 bits rw?
             T1_SOURCE_INT &  // fonte clock interno
             T1_PS_1_8);      // prescaler em 1:8
  T3CONbits.T3CCP1 = 0;
  T3CONbits.T3CCP2 = 0;
  WriteTimer1(0);

  // Configuracao do CPP 1
  CCP1CON = 0b00001011;
  PIE1bits.CCP1IE = 1;  // habilita
  IPR1bits.CCP1IP = 1;  // prioridade: alta
  PIR1bits.CCP1IF = 0;  // flag
  // CPPR = Tempo_alvo / (TCY * Prescaler_timer)
  // CPPR = (1/440) / (200ns * 8) = 1420 / 2 = 710 = 0x058C
  // CPPR = (1/880) / (200ns * 8) = 710  / 2 = 355 = 0x0163
  CCPR1H = 0x05;
  CCPR1L = 0x8C;

  INTCONbits.GIEL = 1;  // habilita int. perifericos
  INTCONbits.GIEH = 1;  // habilita int.

  // INICIALIZACAO LCD
  OpenXLCD(FOUR_BIT & LINES_5X7);  // Comunicacao nibble e linha para cursor
  WriteCmdXLCD(0x01);              // comando para limpar a tela
  Delay10KTCYx(8);  // Delay da inicializacao do LCD (10000*200ns*8 = 16ms)

  WriteCmdXLCD(0x0C);             // Comando para desligar cursor
  WriteCmdXLCD(0x80);             // Comando para colocar na primeira linha
  putrsXLCD("Lab 09 - Etapa 1");  // Escreve a string salva em ROM
  while (1) {
    WriteCmdXLCD(0xC0);  // segunda linha
    if (modo == 0) {
      putrsXLCD("Freq 880Hz");  // Escreve a string salva em ROM
    } else if (modo == 1) {
      putrsXLCD("Freq 440Hz");  // Escreve a string salva em ROM
    }
  }
}

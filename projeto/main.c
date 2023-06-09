// projeto/main.c

#include "main.h"

// Configuracoes
#pragma config FOSC = HS           // HS (cristal 20MHz) => TCY = 200ns
#pragma config CPUDIV = OSC1_PLL2  // Fosc / 1
#pragma config WDT = OFF           // Watchdog desativado
#pragma config PBADEN = OFF        // Pinos do PORTB comecam como digitais
#pragma config LVP = OFF           // Desabilita gravacao em baixa tensao
#pragma config DEBUG = OFF         // debug

// Tempos
char horas = 12;
char minutos = 34;
char segundos = 56;
char horas_maq = 0;
char minutos_maq = 0;
char segundos_maq = 0;
char horas_alvo = 0;
char minutos_alvo = 0;
char segundos_alvo = 0;

// Estados
char menu_1 = 0;
char menu_2 = 0;
char posicao_cursor = 0;
char maquina_ativada = 0;
char tecla_pressionada = 0;
char monitoramento_ativado = 0;
char tempo_pausado = 0;
char alarme_ligado = 0;
unsigned int pwm1 = 50;
unsigned int pwm2 = 20;

// Dados
char temperatura_alvo = 0;   // em graus
char temperatura_atual = 0;  // em graus
char tecla_digitada = ' ';

// Flags
char flag_tmr0_010ms = 0;
char aux_tmr0_100 = 0;

char i = 0;
char j = 0;
char k = 0;

void main(void) {
  // 1 - saida | 0 - entrada
  TRISA = 0b00000001;
  TRISB = 0b11110000;
  TRISC = 0b00000001;
  TRISD = 0x00;
  TRISE = 0x00;
  ADCON1 = 0x0F;             // Todas as portas como digitais
  INTCON2bits.NOT_RBPU = 0;  // Habilita pull-ups PORTB

  OSCCONbits.IDLEN = 0;  // vai para sleep
  RCONbits.IPEN = 1;     // habilita prioridade

  // Configuracao do TIMER0
  // T = TCY * Prescaler * (MAX - INICIAL)
  // 0,01s = 200ns * 256 * (255 - 60)
  OpenTimer0(TIMER_INT_ON &   // habilita interrupcao
             T0_8BIT &        // 16 bits
             T0_SOURCE_INT &  // fonte clock interno
             T0_PS_1_256);    // prescaler em 256
  INTCON2bits.TMR0IP = 1;     // interrupcao TMR0 e de alta prioridade
  WriteTimer0(60);            // CARGA INICIAL = 60

  // Configuracao do TIMER1
  // T = TCY * Prescaler * (MAX - INICIAL)
  // 10ms = 200ns * 8 * (65536 - 3036)
  OpenTimer1(TIMER_INT_ON &   // Interrupcao Habilitada
             T1_16BIT_RW &    // 16 bits
             T1_SOURCE_INT &  // clock interno
             T1_PS_1_8);      // 1:8
  WriteTimer1(3036);

      // Config Registradores USART
      TXSTAbits.SYNC = 0;  // Modo assincrono
  BAUDCONbits.BRG16 = 0;   // 8-bit Baud Rate Generator
  TXSTAbits.BRGH = 1;      // Baud rate = high speed
  RCSTAbits.SPEN = 1;      // Serial port enabled
  TXSTAbits.TXEN = 1;      // Transmit enabled
  SPBRG = 0x40;            // 64
  SPBRGH = 0x00;

  // PWM
  // Configuracoes do Timer 2: Prescaller de 16
  T2CON = 0b00000111;
  PR2 = 61;
  // Modo PWM: active high
  CCP1CON = 0b00001100;
  CCP2CON = 0b00001100;

  INTCONbits.GIEL = 1;  // habilita interrupcoes de perifericos
  INTCONbits.GIEH = 1;  // habilita interrupcoes geral

  // INICIALIZACAO LCD
  OpenXLCD(FOUR_BIT & LINES_5X7);  // Comunicacao nibble e linha para cursor
  WriteCmdXLCD(0x01);              // comando para limpar a tela
  Delay10KTCYx(8);  // Delay da inicializacao do LCD (10000*200ns*8 = 16ms)

  WriteCmdXLCD(0x0C);    // desliga cursor
  WriteCmdXLCD(0x80);    // primeira linha
  putrsXLCD("Projeto");  // Escreve a string salva em ROM

  ajusta_dc_1(pwm1);
  ajusta_dc_2(pwm2);

  while (1) {
    if (flag_tmr0_010ms == 1) {
      // lida_com_o_menu();
      atualiza_menu();
      // ajusta_horas(&horas, &minutos, &segundos);
      // ajusta_horas(&horas_maq, &minutos_maq, &segundos_maq);
      // ajusta_horas(&horas_alvo, &minutos_alvo, &segundos_alvo);

      // if (monitoramento_ativado == 1) {
      //   // "DADO pwm: 100 ccp: 100 temp: 100"
      //   envia_serial("DADO pwm:");
      //   envia_serial("10");
      //   envia_serial(" ccp:");
      //   envia_serial("10");
      //   envia_serial(" temp:'");
      //   envia_numero_serial(temperatura_atual, 0);
      //   envia_serial("'\n\r");
      // }
      flag_tmr0_010ms = 0;
    }
  }
}

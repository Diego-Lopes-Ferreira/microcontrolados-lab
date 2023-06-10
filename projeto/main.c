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

// Dados
unsigned int pwm1 = 50;
unsigned int pwm1_anterior = 0;
unsigned int pwm1_erro = 0;
unsigned int pwm2 = 20;
unsigned int pwm2_anterior = 0;
unsigned int pwm2_erro = 0;
char temperatura_alvo = 0;   // em graus C
char temperatura_atual = 0;  // em graus C
char tecla_digitada = ' ';

// Flags
char flag_tmr0_010ms = 0;
char aux_tmr0_100 = 0;
char ad_finalizado = 0;

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
  INTCON2bits.NOT_RBPU = 0;  // Habilita pull-ups PORTB
  RCONbits.IPEN = 1;         // habilita prioridade

  configura_timers();
  configura_perifericos();

  INTCONbits.GIEL = 1;  // habilita interrupcoes de perifericos
  INTCONbits.GIEH = 1;  // habilita interrupcoes geral

  inicializa_lcd();
  // ajusta_dc_1(pwm1);
  // ajusta_dc_2(pwm2);

  while (1) {
    // tecla_digitada = getKey();
    // tela_testes();

    if (ad_finalizado == 1) {
      if (maquina_ativada) {
        controla_temperatura();
      } else {
        pwm1 = 0;
        pwm2 = 0;
      }
      ajusta_dc_1(pwm1);
      ajusta_dc_2(pwm2);
      ad_finalizado = 0;
    }

    if (flag_tmr0_010ms == 1) {
      lida_com_o_menu();
      atualiza_menu();
      ajusta_horas(&horas, &minutos, &segundos, 24);
      ajusta_horas(&horas_maq, &minutos_maq, &segundos_maq, 99);
      ajusta_horas(&horas_alvo, &minutos_alvo, &segundos_alvo, 99);

      if (monitoramento_ativado == 1) {
        // "DADO pwm: 100 ccp: 100 temp: 100"
        envia_serial("DADO pwm:");
        envia_serial("10");
        envia_serial(" ccp:");
        envia_serial("10");
        envia_serial(" temp:'");
        envia_numero_serial(temperatura_atual, 0);
        envia_serial("'\n\r");
      }
      flag_tmr0_010ms = 0;
    }
  }
}

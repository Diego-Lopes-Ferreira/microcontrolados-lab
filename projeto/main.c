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
char segundos_alvo = 59;

// Estados do Menu
char menu_1 = 0;
char menu_2 = 0;
char posicao_cursor = 0;
char tecla_pressionada = 0;
char tecla_digitada = ' ';
char pisca_tempo_restante = 0;
char acao_usuario = 0;

// Estados da Maquina
char maquina_ativada = 0;
char tempo_pausado = 0;
char monitoramento_ativado = 0;
char alarme_ligado = 0;

// Estados do Controle
unsigned int pwm1 = 0;  // RC2=Cooler
unsigned int pwm1_anterior = 0;
unsigned int pwm1_erro = 0;
unsigned int pwm2 = 0;  // RC1=Aquecedo
unsigned int pwm2_anterior = 0;
unsigned int pwm2_erro = 0;
long tensao = 0;
char temperatura_alvo = 10;  // em graus C
char temperatura_atual = 0;  // em graus C

// Flags
char flag_tmr0_010ms = 0;
char flag_tmr0_500ms = 0;
char flag_tmr0_1s = 0;
char aux_tmr0_050 = 0;
char aux_tmr0_100 = 0;
char ad_finalizado = 0;

void main(void) {
  // 1 - saida | 0 - entrada
  TRISA = 0b00000001;        // Sensor de temperatura em RA0
  TRISB = 0b11110000;        // Teclado matricial
  TRISC = 0b00000000;        // Saidas do PWM
  TRISD = 0b00000000;        // Display LCD
  TRISE = 0b00000000;        // Display LCD
  INTCON2bits.NOT_RBPU = 0;  // Habilita pull-ups PORTB

  RCONbits.IPEN = 1;  // habilita prioridade
  configura_timers();
  configura_perifericos();
  INTCONbits.GIEL = 1;  // habilita interrupcoes de perifericos
  INTCONbits.GIEH = 1;  // habilita interrupcoes geral

  inicializa_lcd();
  // ajusta_dc_1(pwm1);
  // ajusta_dc_2(pwm2);

  while (1) {
    if (ad_finalizado == 1) {
      if (maquina_ativada == 1) {
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
      flag_tmr0_1s = 0;
    }

    if (flag_tmr0_500ms == 1) {
      if (alarme_ligado == 1) {
        inverteLED();
      } else {
        PORTAbits.RA3 = 0;
        PORTAbits.RA5 = 0;
      }

      pisca_tempo_restante = !pisca_tempo_restante;

      flag_tmr0_500ms = 0;
    }

    if (flag_tmr0_010ms == 1) {
      lida_com_o_menu();
      ajusta_horas(&horas, &minutos, &segundos, 24);
      ajusta_horas(&horas_maq, &minutos_maq, &segundos_maq, 99);
      ajusta_horas(&horas_alvo, &minutos_alvo, &segundos_alvo, 99);
      atualiza_menu();

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

    // Aquecedor desliga com a maquina
    if (maquina_ativada == 1) {
      pwm2 = 30;
    } else {
      pwm2 = 0;
    }
  }
}

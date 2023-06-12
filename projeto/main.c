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
char monitoramento_ativado = 1;
char alarme_ligado = 0;

// Estados do Controle
int pwm1 = 0;  // RC2=Cooler
int pwm1_anterior = 0;
int pwm1_erro = 0;
int pwm2 = 0;  // RC1=Aquecedo
int pwm2_anterior = 0;
int pwm2_erro = 0;
long tensao = 0;
int temperatura_alvo = 80;  // em graus C
int temperatura_atual = 0;  // em graus C

// Flags
char flag_tmr0_010ms = 0;
char flag_tmr0_050ms = 0;
char flag_tmr0_500ms = 0;
char flag_tmr0_1s = 0;
char aux_tmr0_005 = 0;
char aux_tmr0_050 = 0;
char aux_tmr0_100 = 0;
char ad_finalizado = 0;

void main(void) {
  // 1 - entrada | 0 - saida
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

  envia_serial("=====\n\r");
  while (1) {
    if (maquina_ativada == 1) {
      if (ad_finalizado == 1) {
        controla_temperatura();  // pwm1 = controle
        ad_finalizado = 0;
      }
      pwm1 = 50;
      ajusta_dc_1(pwm1);
      pwm2 = 30;  // Aquecedor ligado
      ajusta_dc_2(pwm2);
    } else {
      pwm1 = 0;
      ajusta_dc_1(pwm1);
      pwm2 = 0;
      ajusta_dc_2(pwm2);
    }

    if (flag_tmr0_010ms == 1) {
      flag_tmr0_1s = 0;
    }

    if (flag_tmr0_050ms == 1) {
      lida_com_o_menu();
      ajusta_horas(&horas, &minutos, &segundos, 24);
      ajusta_horas(&horas_maq, &minutos_maq, &segundos_maq, 99);
      ajusta_horas(&horas_alvo, &minutos_alvo, &segundos_alvo, 99);
      atualiza_menu();
      flag_tmr0_010ms = 0;
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

    if (flag_tmr0_1s == 1) {
      if (monitoramento_ativado == 1) {
        // “00:00:00 [005] PWM1=000%  PWM2=000%  V=0000mV  T=000C”
        envia_numero_serial(horas, 2);
        envia_serial(":");
        envia_numero_serial(minutos, 2);
        envia_serial(":");
        envia_numero_serial(segundos, 2);

        envia_serial(" [005] PWM1=");
        envia_numero_serial(pwm1, 3);
        envia_serial("% PWM2=");
        envia_numero_serial(pwm2, 3);
        envia_serial("% V=");
        envia_numero_serial((int)tensao, 4);
        envia_serial("mV T=");
        envia_numero_serial((int)temperatura_atual, 3);
        envia_serial("C\n\r");
      }
      flag_tmr0_1s = 0;
    }
  }
}

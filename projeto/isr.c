// projeto/isr.c

#include "globals.h"
#include "main.h"

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
  if (INTCONbits.TMR0IF) {
    // TMR0 finalizado: 10ms => ajuste tempos e configuracao de flags
    // Flag de 10ms (sempre acontece)
    flag_tmr0_010ms = 1;

    // Flag de 50ms
    aux_tmr0_005++;
    if (aux_tmr0_005 == 4) {
      flag_tmr0_050ms = 1;
      aux_tmr0_005 = 0;
    }

    // Flag de 500ms
    aux_tmr0_050++;
    if (aux_tmr0_050 == 49) {
      flag_tmr0_500ms = 1;
      aux_tmr0_050 = 0;
    }

    // Flag de 1s
    aux_tmr0_100++;
    if (aux_tmr0_100 == 99) {
      flag_tmr0_1s = 1;
      aux_tmr0_100 = 0;

      // Incrementa o horario
      if (!(menu_1 == 1 && menu_2 == 1)) segundos++;

      // Decrementa o tempo de maquina
      if (maquina_ativada == 1) {
        segundos_maq--;
        // Desliga a maquina e ativa alarme de fim de ciclo
        if (segundos_maq < 0 && minutos_maq == 0 && horas_maq == 0) {
          segundos_maq = 0;
          alarme_ligado = 1;
          tempo_pausado = 0;
          maquina_ativada = 0;
        }
      }
    }
    WriteTimer0(60);  // 200ns * 256 * (255-60) = 10ms
    INTCONbits.TMR0IF = 0;
  } else if (PIR1bits.TMR1IF) {
    // TMR1 finalizado: 100ms => inicia conversao A/D
    if (maquina_ativada == 1 && ad_finalizado == 0) {
      ADCON0bits.GO_DONE = 1;
    }
    TMR1H = 0;
    TMR1L = 36;
    PIR1bits.TMR1IF = 0;
  } else if (PIR1bits.ADIF) {
    // Conversao A/D finalizada: 20us => seta flag
    ad_finalizado = 1;
    PIR1bits.ADIF = 0;
  }
}

#pragma interruptlow ISR_Baixa_Prioridade
void ISR_Baixa_Prioridade(void) {}

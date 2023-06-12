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
    aux_tmr0_100++;
    aux_tmr0_050++;

    // Flag de 10ms (sempre acontece)
    flag_tmr0_010ms = 1;

    // Flag de 500ms
    if (aux_tmr0_050 == 49) {
      flag_tmr0_500ms = 1;
      aux_tmr0_050 = 0;
    }

    // Flag de 1s
    if (aux_tmr0_100 == 99) {
      flag_tmr0_1s = 1;
      aux_tmr0_100 = 0;
      if (!(menu_1 == 1 && menu_2 == 1)) segundos++;
      if (maquina_ativada == 1) {
        segundos_maq--;
        if (segundos_maq < 0 && minutos_maq == 0 && horas_maq == 0) {
          segundos_maq = 0;
          alarme_ligado = 1;
          maquina_ativada = 0;
        }
      }
    }
    WriteTimer0(60);  // 200ns * 256 * (255-60) = 10ms
    INTCONbits.TMR0IF = 0;
  } else if (PIR1bits.TMR1IF) {
    if (maquina_ativada == 1 && ad_finalizado == 0) {
      ADCON0bits.GO_DONE = 1;  // inicia conversao A/D
    }
    WriteTimer1(64911);
    PIR1bits.TMR1IF = 0;
  } else if (PIR1bits.ADIF) {
    ad_finalizado = 1;
    PIR1bits.ADIF = 0;
  }
}

#pragma interruptlow ISR_Baixa_Prioridade
void ISR_Baixa_Prioridade(void) {}

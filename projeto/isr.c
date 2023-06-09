#include "main.h"
#include "globals.h"

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
    flag_tmr0_010ms = 1;
    aux_tmr0_100++;
    if (aux_tmr0_100 == 99) {
      aux_tmr0_100 = 0;
      if (menu_1 != 1 || menu_2 == 0) segundos++;
      if (maquina_ativada == 1) {
        segundos_maq--;
        if (segundos_maq < 0 && minutos_maq == 0 && horas_maq == 0) {
          segundos_maq = 0;
          alarme_ligado = 1;
          maquina_ativada = 0;
        }
      }
    }
    WriteTimer0(60);
    INTCONbits.TMR0IF = 0;
  }
}

#pragma interruptlow ISR_Baixa_Prioridade
void ISR_Baixa_Prioridade(void) {}

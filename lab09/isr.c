#include "isr.h"

#include <timers.h>

#include "funcoes.h"

// #pragma interrupt ISR_Alta_Prioridade // teoricamente ta no .h
void ISR_Alta_Prioridade(void) {
  extern char btn0Pressionado;
  extern char menu;
  extern char ajuste;
  extern char segundos;
  extern char estouEnviandoSerial;

  if (INTCONbits.TMR0IF) {
    if (estouEnviandoSerial == 0) {
      if (ajuste == 0) segundos++;
    }
    inverteLED();
    WriteTimer0(46004);
    INTCONbits.TMR0IF = 0;
  } else if (INTCONbits.INT0IF) {
    INTCONbits.INT0IF = 0;  // zera a flag
    if (btn0Pressionado == 1) return;

    btn0Pressionado = 1;
    if (menu == 0) {
      ajuste += 1;
      if (ajuste > 3) ajuste = 0;
    } else if (menu == 1) {
      INTCONbits.INT0IE = 0;   // habilita INT0
      INTCON3bits.INT1IE = 0;  // habilita INT1
      INTCON3bits.INT2IE = 0;  // habilita INT2

      estouEnviandoSerial = 1;
    }
  }
}

// #pragma interruptlow ISR_Baixa_Prioridade // teoricamente ta no .h
void ISR_Baixa_Prioridade(void) {
  extern char btn1Pressionado;
  extern char btn2Pressionado;
  extern char menu;
  extern char ajuste;
  extern char horas;
  extern char minutos;
  extern char segundos;

  if (INTCON3bits.INT1IF) {
    INTCON3bits.INT1IF = 0;
    if (btn1Pressionado == 1) return;

    btn1Pressionado = 1;
    if (ajuste == 0) {  // ajuste do menu
      menu -= 1;
      if (menu < 0) menu = 1;
    } else {  // ajuste das horas
      if (ajuste == 1)
        segundos -= 1;
      else if (ajuste == 2)
        minutos -= 1;
      else if (ajuste == 3)
        horas -= 1;
    }
  } else if (INTCON3bits.INT2IF) {
    INTCON3bits.INT2IF = 0;
    if (btn2Pressionado == 1) return;
    btn2Pressionado = 1;

    if (ajuste == 0) {  // ajuste do menu
      menu += 1;
      if (menu > 1) menu = 0;
    } else {  // ajuste das horas
      if (ajuste == 1)
        segundos += 1;
      else if (ajuste == 2)
        minutos += 1;
      else if (ajuste == 3)
        horas += 1;
    }
  }
}

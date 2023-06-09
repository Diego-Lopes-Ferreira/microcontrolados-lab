#include "funcoes.h"

#include <delays.h>

#include "xlcd_exsto.h"

void configura_isr(void) {
  RCONbits.IPEN = 1;  // habilita prioridade

  INTCONbits.INT0IE = 1;  // habilita INT0
  INTCONbits.INT0IF = 0;  // zera flag da INT0
                          // (sempre de alta prioridade)

  INTCON3bits.INT1IE = 1;  // habilita INT1
  INTCON3bits.INT1IF = 0;  // zera flag da INT1
  INTCON3bits.INT1IP = 0;  // interrupcao INT1 e de baixa prioridade

  INTCON3bits.INT2IE = 1;  // habilita INT2
  INTCON3bits.INT2IF = 0;  // zera flag da INT2
  INTCON3bits.INT2IP = 0;  // interrupcao INT2 e de baixa prioridade

  INTCONbits.GIEL = 1;  // habilita interrupcoes de perifericos
  INTCONbits.GIEH = 1;  // habilita interrupcoes geral
}

void limpa_flags(void) {
  extern char btn0Pressionado;
  extern char btn1Pressionado;
  extern char btn2Pressionado;

  if (btn0Pressionado == 1) {
    Delay1KTCYx(20);  // 200ns * 1000 * 20 = 4ms
    btn0Pressionado = 0;
  }

  if (btn1Pressionado == 1) {
    Delay1KTCYx(20);  // 200ns * 1000 * 20 = 4ms
    btn1Pressionado = 0;
  }

  if (btn2Pressionado == 1) {
    Delay1KTCYx(20);  // 200ns * 1000 * 20 = 4ms
    btn2Pressionado = 0;
  }
}

void imprime_horario(void) {
  extern char horas;
  extern char minutos;
  extern char segundos;

  putcXLCD(0x30 + (horas / 10));
  putcXLCD(0x30 + (horas % 10));
  putcXLCD(':');
  putcXLCD(0x30 + (minutos / 10));
  putcXLCD(0x30 + (minutos % 10));
  putcXLCD(':');
  putcXLCD(0x30 + (segundos / 10));
  putcXLCD(0x30 + (segundos % 10));
}

void ajusta_horas(void) {
  extern char horas;
  extern char minutos;
  extern char segundos;

  if (segundos >= 60) {
    segundos = 0;
    minutos += 1;
  } else if (segundos < 0) {
    segundos = 59;
    minutos -= 1;
  }
  if (minutos >= 60) {
    minutos = 0;
    horas += 1;
  } else if (minutos < 0) {
    minutos = 59;
    horas -= 1;
  }
  if (horas >= 24) {
    horas = 0;
  } else if (horas < 0) {
    horas = 23;
  }
}

void mostraMenuLCD(void) {
  extern char estouEnviandoSerial;
  extern char teclaDigitada;
  extern char menu;
  extern char ajuste;

  // Mostra no display
  WriteCmdXLCD(0x80);  // Comando para colocar na primeira linha
  if (estouEnviandoSerial == 1) {
    putrsXLCD("'");
    putcXLCD(teclaDigitada);
    putrsXLCD("'");
  } else {
    imprime_horario();
  }

  WriteCmdXLCD(0xC0);  // Comando para colocar na segunda linha
  switch (menu) {
    case 0:
      if (ajuste == 0) {
        putrsXLCD("[1/2] Ajustar ");
      } else if (ajuste == 1) {
        putrsXLCD("[1/2] Segundos");
      } else if (ajuste == 2) {
        putrsXLCD("[1/2] Minutos ");
      } else if (ajuste == 3) {
        putrsXLCD("[1/2] Horas   ");
      }
      break;
    case 1:
      if (estouEnviandoSerial == 0) {
        putrsXLCD("[2/2] Serial  ");
      } else {
        putrsXLCD("[2/2] Enviando");
      }
      break;
  }
}

void inverteLED(void) {
  if (PORTAbits.RA3 == PORTAbits.RA5) {
    PORTAbits.RA3 = 1;
    PORTAbits.RA5 = 0;
  }
  PORTAbits.RA3 = !PORTAbits.RA3;
  PORTAbits.RA5 = !PORTAbits.RA5;
}

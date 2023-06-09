#include "teclado.h"

#include <P18F4550.h>

char getKey(void) {
  /*
  Saidas: char (' ' = leitura vazia)

  1. Armazena o estado atual da maquina (TRIS, ADCON e NOT_RBPU)
  2. Configura o TRIS, o ADCON e o NOT_RBPU
  3. Faz a varredura
    3.1 Desativa a coluna 3 (RB0 = 0) e verifica as linhas
    3.2 Desativa a coluna 2 (RB1 = 0) e verifica as linhas
    3.3 Desativa a coluna 1 (RB2 = 0) e verifica as linhas
    3.4 Desativa a coluna 0 (RB3 = 0) e verifica as linhas
  4. Descobre qual o char lido
  5. Volta as configuracoes de TRIS, ADCON e NOT_RBPU
  */

  char k = ' ';
  const unsigned char keymap[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  char prevTRISB = TRISB;
  char prevTRISD = TRISD;
  char prevADCON1 = ADCON1;
  char prevNOT_RBPU = INTCON2bits.NOT_RBPU; // pull-ups

  ADCON1 = 0x0F;             // Todas Portas digitais
  INTCON2bits.NOT_RBPU = 0;  // habilita pull-ups da PORTB
  TRISB = 0xF0;
  TRISD = 0x00;

  LATB = 0b11111110;  // Habilita Coluna 3
  Nop();
  Nop();
  if (L0 == 0) {
    k = 10;
  } else if (L1 == 0) {
    k = 11;
  } else if (L2 == 0) {
    k = 12;
  } else if (L3 == 0) {
    k = 13;
  }

  LATB = 0b11111101;  // Habilita Coluna 2
  Nop();
  Nop();
  if (L0 == 0) {
    k = 3;
  } else if (L1 == 0) {
    k = 6;
  } else if (L2 == 0) {
    k = 9;
  } else if (L3 == 0) {
    k = 14;
  }

  LATB = 0b11111011;  // Habilita Coluna 1
  Nop();
  Nop();
  if (L0 == 0) {
    k = 2;
  } else if (L1 == 0) {
    k = 5;
  } else if (L2 == 0) {
    k = 8;
  } else if (L3 == 0) {
    k = 0;
  }

  LATB = 0b11110111;  // Habilita Coluna 0
  Nop();
  Nop();
  if (L0 == 0) {
    k = 1;
  } else if (L1 == 0) {
    k = 4;
  } else if (L2 == 0) {
    k = 7;
  } else if (L3 == 0) {
    k = 15;
  }

  if (k != ' ') k = keymap[k];

  TRISB = prevTRISB;
  TRISD = prevTRISD;
  ADCON1 = prevADCON1;                  // Todas Portas digitais
  INTCON2bits.NOT_RBPU = prevNOT_RBPU;  // habilita pull-ups da PORTB

  return (k);
}

#include "globals.h"
#include "main.h"

char getKey(void) {
  /*
  Saidas: char (' ' = leitura vazia)
  1. Desativa a coluna 3 (RB0 = 0) e verifica as linhas
  2. Desativa a coluna 2 (RB1 = 0) e verifica as linhas
  3. Desativa a coluna 1 (RB2 = 0) e verifica as linhas
  4. Desativa a coluna 0 (RB3 = 0) e verifica as linhas
  */

  char k = ' ';

  LATB = 0b11111110;  // Habilita Coluna 3
  Nop();
  Nop();
  if (L0 == 0) {
    k = 'A';
  } else if (L1 == 0) {
    k = 'B';
  } else if (L2 == 0) {
    k = 'C';
  } else if (L3 == 0) {
    k = 'D';
  }

  LATB = 0b11111101;  // Habilita Coluna 2
  Nop();
  Nop();
  if (L0 == 0) {
    k = '3';
  } else if (L1 == 0) {
    k = '6';
  } else if (L2 == 0) {
    k = '9';
  } else if (L3 == 0) {
    k = 'E';
  }

  LATB = 0b11111011;  // Habilita Coluna 1
  Nop();
  Nop();
  if (L0 == 0) {
    k = '2';
  } else if (L1 == 0) {
    k = '5';
  } else if (L2 == 0) {
    k = '8';
  } else if (L3 == 0) {
    k = '0';
  }

  LATB = 0b11110111;  // Habilita Coluna 0
  Nop();
  Nop();
  if (L0 == 0) {
    k = '1';
  } else if (L1 == 0) {
    k = '4';
  } else if (L2 == 0) {
    k = '7';
  } else if (L3 == 0) {
    k = 'F';
  }

  return k;
}

#include "teclado.h"

// funcao para receber tecla
char getKey(void) {
  char k = ' ';
  LATB = 0b11111110;  // Habilita Coluna 3
  Nop();
  Nop();
  if (!L0)
    k = 10;
  else if (!L1)
    k = 11;
  else if (!L2)
    k = 12;
  else if (!L3)
    k = 13;

  LATB = 0b11111101;  // Habilita Coluna 2
  Nop();
  Nop();
  if (!L0)
    k = 3;
  else if (!L1)
    k = 6;
  else if (!L2)
    k = 9;
  else if (!L3)
    k = 14;

  LATB = 0b11111011;  // Habilita Coluna 1
  Nop();
  Nop();
  if (!L0)
    k = 2;
  else if (!L1)
    k = 5;
  else if (!L2)
    k = 8;
  else if (!L3)
    k = 0;

  LATB = 0b11110111;  // Habilita Coluna 0
  Nop();
  Nop();
  if (!L0)
    k = 1;
  else if (!L1)
    k = 4;
  else if (!L2)
    k = 7;
  else if (!L3)
    k = 15;

  return (k);
}

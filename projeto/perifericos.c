#include "globals.h"
#include "main.h"

void envia_serial(const rom char* mensagem) {
  while (*mensagem) {
    while (TXSTAbits.TRMT == 0) {
    }
    TXREG = *mensagem;
    mensagem++;
  }
}

void envia_numero_serial(char numero, char tres_casas) {
  if (tres_casas) {
    while (TXSTAbits.TRMT == 0) {
    }
    TXREG = ((numero / 10) / 10) + 0x30;
  }

  while (TXSTAbits.TRMT == 0) {
  }
  TXREG = ((numero / 10) - 10 * ((numero / 10) / 10)) + 0x30;

  while (TXSTAbits.TRMT == 0) {
  }
  TXREG = (numero % 10) + 0x30;
}

void ajusta_dc_1(unsigned int dc_cpp1) {
  //  10 bits: CCPRL DC1B1 DC1B0
  //  >> 0 = 01 23456789 => % 2 = 9 => DC1B1
  //  >> 1 = 00 12345678 => % 2 = 8 => DC1B0
  //  >> 2 = 00 01234567 => (char) = 01234567 => CCPRL
  dc_cpp1 = dc_cpp1 * 4 * (PR2 + 1) / 100;
  CCPR1L = (char)(dc_cpp1 >> 2);
  CCP1CONbits.DC1B0 = dc_cpp1 % 2;
  dc_cpp1 = dc_cpp1 >> 1;
  CCP1CONbits.DC1B1 = dc_cpp1 % 2;
}

void ajusta_dc_2(unsigned int dc_cpp2) {
  dc_cpp2 = dc_cpp2 * 4 * (PR2 + 1) / 100;
  CCPR2L = (char)(dc_cpp2 >> 2);
  CCP2CONbits.DC2B0 = dc_cpp2 % 2;
  dc_cpp2 = dc_cpp2 >> 1;
  CCP2CONbits.DC2B1 = dc_cpp2 % 2;
}

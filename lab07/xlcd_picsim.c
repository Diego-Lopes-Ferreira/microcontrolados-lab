#include "xlcd_picsim.h"

#include <P18F4550.h>

// Para fosc = 20 MHz
void atraso_ms(unsigned int valor) {
  unsigned int i;
  unsigned char j;

  for (i = 0; i < valor; i++) {
    for (j = 0; j < 200; j++) {
      Nop();
      Nop();
      Nop();
      Nop();
      Nop();
    }
  }
}

void lcd_wr(unsigned char val) { LPORT = val; }

void WriteCmdXLCD(unsigned char val) {
  LENA = 1;
  lcd_wr(val);
  LDAT = 0;
  atraso_ms(3);
  LENA = 0;
  atraso_ms(3);
  LENA = 1;
}

void WriteDataXLCD(unsigned char val) {
  LENA = 1;
  lcd_wr(val);
  LDAT = 1;
  atraso_ms(3);
  LENA = 0;
  atraso_ms(3);
  LENA = 1;
}

void OpenXLCD(unsigned char lcdtype) {
  LENA = 0;
  LDAT = 0;
  atraso_ms(20);
  LENA = 1;
  WriteCmdXLCD(L_CFG);
  atraso_ms(5);
  WriteCmdXLCD(L_CFG);
  atraso_ms(1);
  WriteCmdXLCD(L_CFG);  // configura
  WriteCmdXLCD(L_OFF);
  WriteCmdXLCD(L_ON);   // liga
  WriteCmdXLCD(L_CLR);  // limpa
  WriteCmdXLCD(L_CFG);  // configura
  WriteCmdXLCD(L_L1);
}

void putrsXLCD(const rom unsigned char* str) {
  while (*str) {
    WriteDataXLCD(*str);
    str++;
  }
}

void putsXLCD(unsigned char* str) {
  while (*str) {
    WriteDataXLCD(*str);
    str++;
  }
}

void printStrLine(const rom unsigned char* str, char line) {
  // Only works for 10 char display
  char i = 0;
  if (line == 1) {
    WriteCmdXLCD(0x80);
  } else if (line == 2) {
    WriteCmdXLCD(0xC0);
  } else {
    return;
  }

  while (*str) {
    WriteDataXLCD(*str);
    str++;
    i++;
  }
  while (i < 10) {
    WriteDataXLCD(' ');
    i++;
  }
}

void printStrLine(unsigned char* str, char line) {
  // Only works for 10 char display
  char i = 0;
  if (line == 1) {
    WriteCmdXLCD(0x80);
  } else if (line == 2) {
    WriteCmdXLCD(0xC0);
  } else {
    return;
  }

  while (*str) {
    WriteDataXLCD(*str);
    str++;
    i++;
  }
  while (i < 10) {
    WriteDataXLCD(' ');
    i++;
  }
}

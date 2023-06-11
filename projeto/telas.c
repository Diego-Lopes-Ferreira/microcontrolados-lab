#include "globals.h"
#include "main.h"

void inicializa_lcd(void) {
  OpenXLCD(FOUR_BIT & LINES_5X7);  // Comunicacao nibble e linha para cursor
  WriteCmdXLCD(0x01);              // comando para limpar a tela
  Delay10KTCYx(8);  // Delay da inicializacao do LCD (10000*200ns*8 = 16ms)

  WriteCmdXLCD(0x0C);    // desliga cursor
  WriteCmdXLCD(0x80);    // primeira linha
  putrsXLCD("Projeto");  // Escreve a string salva em ROM
}

void atualiza_menu(void) {
  if (menu_1 == 0) {
    tela_menu_externo();
  } else {
    tela_menu_interno();
  }
}

void tela_menu_externo(void) {
  // "00:00:00        "
  // "00C | 00mA | 00%"
  WriteCmdXLCD(0x80);  // primeira linha

  if (maquina_ativada == 0 && tempo_pausado == 0) {  // maquina desligada
    imprime_horario(horas, minutos, segundos);
  } else if (maquina_ativada == 0 && tempo_pausado == 1) {  // maquina pausada
    if (pisca_tempo_restante == 1) {
      imprime_horario(horas_maq, minutos_maq, segundos_maq);
    } else {
      putrsXLCD("                ");
    }
  } else {  // maquina ligada
    imprime_horario(horas_maq, minutos_maq, segundos_maq);
  }

  WriteCmdXLCD(0xC0);  // segunda linha

  if (maquina_ativada == 0 && tempo_pausado == 0) {  // Maquina desligada
    putrsXLCD("Maquina OFF     ");
  } else if (maquina_ativada == 0 && tempo_pausado == 1) {  // Maquina pausada
    putrsXLCD("Maquina OFF*    ");
  } else {                                      // maquina ligada
    putcXLCD(0x30 + (temperatura_atual / 10));  // 1 01 "0               "
    putcXLCD(0x30 + (temperatura_atual % 10));  // 1 02 "00              "
    putrsXLCD("C | ");                          // 4 06 "00C |           "
    putcXLCD(0x30 + (temperatura_atual / 10));  // 1 07 "00C | 00        "
    putcXLCD(0x30 + (temperatura_atual % 10));  // 1 08 "00C | 00        "
    putrsXLCD("mA | ");                         // 5 13 "00C | 00mA |    "
    putcXLCD(0x30 + (pwm1 / 10));               // 1 14 "00C | 00mA | 0  "
    putcXLCD(0x30 + (pwm1 % 10));               // 1 15 "00C | 00mA | 00 "
    putrsXLCD("%");                             // 1 16 "00C | 00mA | 00%"
  }
}

void tela_menu_interno(void) {
  WriteCmdXLCD(0x80);  // primeira linha
  if (menu_1 == 1) {
    // "[1/4] Horario   "
    // "[1/4] Horario*  "
    putrsXLCD("[1/4] Horario");
    if (menu_2 > 0) {
      putrsXLCD("*  ");
    } else {
      putrsXLCD("   ");
    }
  } else if (menu_1 == 2) {
    // "[2/4] Timer     "
    // "[2/4] Timer*    "
    putrsXLCD("[2/4] Timer");
    if (menu_2 > 0) {
      putrsXLCD("*    ");
    } else {
      putrsXLCD("     ");
    }
  } else if (menu_1 == 3) {
    // "[3/4] Temp.    "
    // "[3/4] Temp.*   "
    putrsXLCD("[3/4] Temp.");
    if (menu_2 > 0) {
      putrsXLCD("*   ");
    } else {
      putrsXLCD("    ");
    }
  } else if (menu_1 == 4) {
    // "[4/4] Dados:OFF "
    // "[4/4] Dados:OFF*"
    putrsXLCD("[4/4] Dados:");
    if (monitoramento_ativado == 1) {
      putrsXLCD("ON ");
    } else {
      putrsXLCD("OFF");
    }
    if (menu_2 > 0) {
      putrsXLCD("*");
    } else {
      putrsXLCD(" ");
    }
  }

  WriteCmdXLCD(0xC0);  // segunda linha
  if (menu_1 == 1) {
    // "12:34:56        "
    imprime_horario(horas, minutos, segundos);
  } else if (menu_1 == 2) {
    // "00:00:00        "
    imprime_horario(horas_alvo, minutos_alvo, segundos_alvo);
  } else if (menu_1 == 3) {
    // "00              "
    putcXLCD(0x30 + (temperatura_alvo / 10));
    putcXLCD(0x30 + (temperatura_alvo % 10));
    putrsXLCD("              ");
  } else if (menu_1 == 4) {
    // "Desativar       " ou "Ativar          "
    if (monitoramento_ativado == 1) {
      putrsXLCD("Desativar       ");
    } else {
      putrsXLCD("Ativar          ");
    }
  }
}

void tela_testes(void) {
  // " 'F' 1:0 2:0    "
  // "00C | 00mA | 00%"
  WriteCmdXLCD(0x80);  // primeira linha
  putrsXLCD(" '");
  putcXLCD(tecla_digitada);
  putrsXLCD("' 1:");
  putcXLCD(menu_1 + 0x30);
  putrsXLCD(" 2:");
  putcXLCD(menu_2 + 0x30);
  WriteCmdXLCD(0xC0);                         // segunda linha
  putcXLCD(0x30 + (temperatura_atual / 10));  // 1 01
  putcXLCD(0x30 + (temperatura_atual % 10));  // 1 02
  putrsXLCD("C | ");                          // 4 06
  putcXLCD(0x30 + (temperatura_atual / 10));  // 1 07
  putcXLCD(0x30 + (temperatura_atual % 10));  // 1 08
  putrsXLCD("mA | ");                         // 5 13
  putcXLCD(0x30 + (pwm1 / 10));               // 1 14
  putcXLCD(0x30 + (pwm1 % 10));               // 1 15
  putrsXLCD("%");                             // 1 16
  // imprime_horario(horas, minutos, segundos);
}

void imprime_horario(char h, char m, char s) {
  // "00:00:00        "
  putcXLCD(0x30 + (h / 10));
  putcXLCD(0x30 + (h % 10));
  putcXLCD(':');
  putcXLCD(0x30 + (m / 10));
  putcXLCD(0x30 + (m % 10));
  putcXLCD(':');
  putcXLCD(0x30 + (s / 10));
  putcXLCD(0x30 + (s % 10));
  putrsXLCD("        ");
}

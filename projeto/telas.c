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
  // Alarme ligado sobrescreve as outras telas
  if (alarme_ligado == 1) {
    tela_alarme();
    return;
  }

  // Senao, mostre os menus de acordo:
  if (menu_1 == 0) {
    tela_menu_externo();
  } else {
    tela_menu_interno();
    // usuario editando algo: pisca cursor
    if (menu_2 == 1) {
      lcd_pisca_indicador_cursor();
    }
  }
}

void tela_alarme(void) {
  WriteCmdXLCD(0x0C);  // desliga cursor
  // "00:00:00        "
  // "Fim Ciclo       "
  WriteCmdXLCD(0x80);  // primeira linha
  if (pisca_tempo_restante == 1) {
    imprime_horario(horas_maq, minutos_maq, segundos_maq);
  } else {
    putrsXLCD("                ");
  }
  WriteCmdXLCD(0xC0);  // segunda linha
  putrsXLCD("Fim Ciclo       ");
}

void tela_menu_externo(void) {
  WriteCmdXLCD(0x0C);  // desliga cursor
  //  Ligada               Pausada              Desligada
  // "00:00:00        " | "00:00:00        " | "12:34:56        "
  // "00C | 00mA | 00%" | "Maquina OFF*    " | "Maquina OFF     "
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
  } else {  // maquina ligada

    // "0000mV 00C  100%"
    putcXLCD(0x30 + (tensao / 1000));                 // 1 00 "0               "
    putcXLCD(0x30 + (tensao % 1000) / 100);           // 1 00 "00              "
    putcXLCD(0x30 + ((tensao % 1000) % 100) / 10);    // 1 00 "000             "
    putcXLCD(0x30 + ((tensao % 1000) % 100) % 10);    // 1 00 "0000            "
    putrsXLCD("mV=");                                 // 5 00 "0000mV=         "
    putcXLCD(0x30 + (temperatura_atual / 100));       // 1 01 "0000mV=0        "
    putcXLCD(0x30 + (temperatura_atual % 100) / 10);  // 1 01 "0000mV=00       "
    putcXLCD(0x30 + (temperatura_atual % 100) % 10);  // 1 02 "0000mV=000      "
    putrsXLCD("C ");                                  // 2 04 "0000mV=000C     "
    putcXLCD(0x30 + (pwm1 / 100));                    // 1 00 "0000mV=000C 1   "
    putcXLCD(0x30 + (pwm1 % 100) / 10);               // 1 00 "0000mV=000C 10  "
    putcXLCD(0x30 + (pwm1 % 100) % 10);               // 1 00 "0000mV=000C 100 "
    putrsXLCD("%");                                   // 1 00 "0000mV=000C 100%"
  }
}

void tela_menu_interno(void) {
  WriteCmdXLCD(0x0C);  // desliga cursor
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
    putcXLCD(0x30 + (temperatura_alvo / 100));
    putcXLCD(0x30 + ((temperatura_alvo % 100) / 10));
    putcXLCD(0x30 + ((temperatura_alvo % 100) % 10));
    putrsXLCD("             ");
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
  WriteCmdXLCD(0x0C);  // desliga cursor
  // " 'F' 1:0 2:0    "
  // "00C | 00mA | 00%"
  WriteCmdXLCD(0x80);  // primeira linha
  putrsXLCD("'");
  putcXLCD(tecla_digitada);
  putrsXLCD("'1:");
  putcXLCD(menu_1 + 0x30);
  putrsXLCD("2:");
  putcXLCD(menu_2 + 0x30);
  putrsXLCD("P:");
  putcXLCD(posicao_cursor + 0x30);
  WriteCmdXLCD(0xC0);  // segunda linha
  // putcXLCD(0x30 + (temperatura_atual / 10));  // 1 01
  // putcXLCD(0x30 + (temperatura_atual % 10));  // 1 02
  // putrsXLCD("C | ");                          // 4 06
  // putcXLCD(0x30 + (temperatura_atual / 10));  // 1 07
  // putcXLCD(0x30 + (temperatura_atual % 10));  // 1 08
  // putrsXLCD("mA | ");                         // 5 13
  // putcXLCD(0x30 + (pwm1 / 10));               // 1 14
  // putcXLCD(0x30 + (pwm1 % 10));               // 1 15
  // putrsXLCD("%");                             // 1 16
  imprime_horario(horas, minutos, segundos);
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

void lcd_pisca_indicador_cursor(void) {
  // Pisca o caracter na posicao do cursor a cada 0,5s
  // (coloca um espaco no lugar dele)
  char i = 0;
  char iteracoes = 0;
  if (pisca_tempo_restante == 0) {
    return;
  }

  if (posicao_cursor == 1) iteracoes = 0;  // "0
  if (posicao_cursor == 2) iteracoes = 1;  // "00
  if (posicao_cursor == 3) iteracoes = 3;  // "00:0
  if (posicao_cursor == 4) iteracoes = 4;  // "00:00
  if (posicao_cursor == 5) iteracoes = 6;  // "00:00:0
  if (posicao_cursor == 6) iteracoes = 7;  // "00:00:00

  WriteCmdXLCD(0xC0);  // segunda linha
  for (i = 0; i < iteracoes; i++) {
    WriteCmdXLCD(0x14);  // desloca cursor direita
  }
  putrsXLCD(" ");
}

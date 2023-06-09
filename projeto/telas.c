#include "globals.h"
#include "main.h"

void atualiza_menu(void) {
  if (menu_1 == 0) {
    tela_menu_externo();
  } else {
    tela_menu_interno();
  }

  // TESTE DA LEITURA DE TECLAS
  // WriteCmdXLCD(0x80);  // primeira linha
  // tecla_digitada = getKey();
  // putrsXLCD(" '");
  // putcXLCD(tecla_digitada);
  // putrsXLCD("' 1:");
  // putcXLCD(menu_1 + 0x30);
  // putrsXLCD(" 2:");
  // putcXLCD(menu_2 + 0x30);
  // WriteCmdXLCD(0xC0);  // segunda linha
  // putcXLCD(0x30 + (temperatura_atual / 10));  // 1 01
  // putcXLCD(0x30 + (temperatura_atual % 10));  // 1 02
  // putrsXLCD("C | ");                          // 4 06
  // putcXLCD(0x30 + (temperatura_atual / 10));  // 1 07
  // putcXLCD(0x30 + (temperatura_atual % 10));  // 1 08
  // putrsXLCD("mA | ");                         // 5 13
  // putcXLCD(0x30 + (pwm1 / 10));               // 1 14
  // putcXLCD(0x30 + (pwm1 % 10));               // 1 15
  // putrsXLCD("%");                             // 1 16
  // imprime_horario(horas, minutos, segundos);
}

void tela_menu_externo(void) {
  // "00:00:00        "
  // "00C | 00mA | 00%"
  WriteCmdXLCD(0x80);  // primeira linha
  if (maquina_ativada == 0) {
    imprime_horario(horas, minutos, segundos);
  } else {
    imprime_horario(horas_maq, minutos_maq, segundos_maq);
  }

  WriteCmdXLCD(0xC0);  // segunda linha
  if (maquina_ativada == 0) {
    if (tempo_pausado == 1) {
      putrsXLCD("Maquina OFF*    ");
    } else {
      putrsXLCD("Maquina OFF     ");
    }
  } else {
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
    imprime_horario(horas, minutos, segundos);
  } else if (menu_1 == 2) {
    imprime_horario(horas_alvo, minutos_alvo, segundos_alvo);
  } else if (menu_1 == 3) {
    putcXLCD(0x30 + (temperatura_alvo / 10));
    putcXLCD(0x30 + (temperatura_alvo % 10));
    putrsXLCD("      ");
  } else if (menu_1 == 4) {
    if (monitoramento_ativado == 1) {
      putrsXLCD("Desativar   ");
    } else {
      putrsXLCD("Ativar      ");
    }
  }
}

#include "globals.h"
#include "main.h"

void ajusta_horas(char* horas, char* minutos, char* segundos) {
  if (*segundos >= 60) {
    *segundos = 0;
    *minutos += 1;
  } else if (*segundos < 0) {
    *segundos = 59;
    *minutos -= 1;
  }
  if (*minutos >= 60) {
    *minutos = 0;
    *horas += 1;
  } else if (*minutos < 0) {
    *minutos = 59;
    *horas -= 1;
  }
  if (*horas >= 24) {
    *horas = 0;
  } else if (*horas < 0) {
    *horas = 23;
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

void lida_com_o_menu(void) {
  tecla_digitada = getKey();

  if (tecla_digitada == ' ') {
    if (tecla_pressionada == 1) tecla_pressionada = 0;
    return;
  }
  if (tecla_pressionada == 1) return;
  tecla_pressionada = 1;

  if (alarme_ligado == 1) {
    if (tecla_digitada == 'F') alarme_ligado = 0;
    return;
  }

  if (menu_1 == 0) {
    if (tecla_digitada == '0') {
      menu_1 = 1;
    } else if (tecla_digitada == 'F') {
      if (maquina_ativada == 1) {
        tempo_pausado = 1;
      } else {
        tempo_pausado = 0;
      }
      maquina_ativada = 0;
    } else if (tecla_digitada == 'E') {
      if (temperatura_alvo > 0 &&
          (segundos_alvo > 0 || minutos_alvo > 0 || horas_alvo > 0)) {
        if (tempo_pausado == 0) {
          horas_maq = horas_alvo;
          minutos_maq = minutos_alvo;
          segundos_maq = segundos_alvo;
        }
        maquina_ativada = 1;
      }
    }
    return;
  }  // if (menu_1 == 0)

  if (menu_1 > 0 && menu_2 == 0) {
    if (tecla_digitada == 'A') {
      menu_1 -= 1;
      if (menu_1 < 1) menu_1 = 4;
    } else if (tecla_digitada == 'B') {
      menu_1 += 1;
      if (menu_1 > 4) menu_1 = 1;
    } else if (tecla_digitada == 'F') {
      menu_1 = 0;
      posicao_cursor = 1;
    } else if (tecla_digitada == 'E') {
      if (menu_1 == 4) {
        monitoramento_ativado = !monitoramento_ativado;
      } else {
        menu_2 += 1;
        posicao_cursor = 1;
      }
    }
    return;
  }  // if (menu_1 > 0 && menu_2 == 0)

  if (menu_1 > 0 && menu_2 == 1) {
    if (tecla_digitada == 'C') {
      // move cursor esquerda
      posicao_cursor += 1;
      if (posicao_cursor == 7) posicao_cursor = 6;
    } else if (tecla_digitada == 'D') {
      // move cursor direita
      posicao_cursor -= 1;
      if (posicao_cursor == 0) posicao_cursor = 1;
    } else if (tecla_digitada == 'E') {
      // enter
      menu_2 = 0;
      posicao_cursor = 1;
    } else if (tecla_digitada <= '9') {
      // se o cursor estiver na casa das dezenas => so pode de 0 a 5
      if (posicao_cursor % 2 == 0 && posicao_cursor < 5 && tecla_digitada > '5')
        return;
      if (menu_1 == 1) {
        insere_tecla_horario(tecla_digitada, &horas, &minutos, &segundos);
      } else if (menu_1 == 2) {
        insere_tecla_horario(tecla_digitada, &horas_alvo, &minutos_alvo,
                             &segundos_alvo);
      } else if (menu_1 == 3) {
        if (temperatura_alvo == -1) temperatura_alvo = 0;
        temperatura_alvo -= 10 * (temperatura_alvo / 10);
        temperatura_alvo = (temperatura_alvo * 10) + (tecla_digitada - 0x30);
      }
    }
  }  // if (menu_1 > 0 && menu_2 == 1)
}

void insere_tecla_horario(char n, char* horas, char* minutos, char* segundos) {
  n -= 0x30;
  if (posicao_cursor == 1) {
    *segundos = n + (10 * (*segundos / 10));
  } else if (posicao_cursor == 2) {
    *segundos = (n * 10) + (*segundos % 10);
  } else if (posicao_cursor == 3) {
    *minutos = n + (10 * (*minutos / 10));
  } else if (posicao_cursor == 4) {
    *minutos = (n * 10) + (*minutos % 10);
  } else if (posicao_cursor == 5) {
    *horas = n + (10 * (*horas / 10));
  } else if (posicao_cursor == 6) {
    *horas = (n * 10) + (*horas % 10);
  }
}

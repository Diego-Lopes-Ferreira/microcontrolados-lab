// projeto/funcoes.c

#include "globals.h"
#include "main.h"

void ajusta_horas(char* horas, char* minutos, char* segundos, char max_horas) {
  // Recebe um ponteiro para as variaveis e faz o ajuste de acordo
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
  if (*horas >= max_horas) {
    *horas = 0;
  } else if (*horas < 0) {
    *horas = 23;
  }
}

void inverteLED(void) {
  if (PORTAbits.RA3 == PORTAbits.RA5) {
    // Se os dois estao desligados, liga um primeiro
    PORTAbits.RA3 = 1;
    PORTAbits.RA5 = 0;
  }
  PORTAbits.RA3 = !PORTAbits.RA3;
  PORTAbits.RA5 = !PORTAbits.RA5;
}

void lida_com_o_menu(void) {
  // Funcao que interpreta as acoes do usuario
  tecla_digitada = getKey();

  // Caso nada esteja pressionado, reset do estado intero
  if (tecla_digitada == ' ') {
    if (tecla_pressionada == 1) tecla_pressionada = 0;
    return;
  }

  // Se uma tecla esta pressionada, nao faca nada (senao seta a flag)
  if (tecla_pressionada == 1) return;
  tecla_pressionada = 1;

  // Se o alarme estiver ligado, precisa apertar F pra desligar
  if (alarme_ligado == 1) {
    if (tecla_digitada == 'F') alarme_ligado = 0;
    return;
  }

  // Menu Externo:
  if (menu_1 == 0) {
    if (tecla_digitada == '0') {
      // Acessa o menu
      menu_1 = 1;
    } else if (tecla_digitada == 'F') {
      // Pausa ou reseta a maquina
      if (maquina_ativada == 1) {
        tempo_pausado = 1;
      } else {
        tempo_pausado = 0;
      }
      maquina_ativada = 0;
    } else if (tecla_digitada == 'E') {
      // Inicia ou retoma a operacao da maquina
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
  }

  // Menu interno navegando:
  if (menu_1 > 0 && menu_2 == 0) {
    if (tecla_digitada == 'A') {
      // Item anterior do menu
      menu_1 -= 1;
      if (menu_1 < 1) menu_1 = 4;
    } else if (tecla_digitada == 'B') {
      // Proximo item do menu
      menu_1 += 1;
      if (menu_1 > 4) menu_1 = 1;
    } else if (tecla_digitada == 'F') {
      // Sai do menu
      menu_1 = 0;
      posicao_cursor = 1;
    } else if (tecla_digitada == 'E') {
      // Acessa a configuracao (ou ativa o monitoramento)
      if (menu_1 == 4) {
        monitoramento_ativado = !monitoramento_ativado;
      } else {
        menu_2 += 1;
        posicao_cursor = 1;
      }
    }
    return;
  }

  // Menu interno configurando:
  if (menu_1 > 0 && menu_2 == 1) {
    if (tecla_digitada == 'C') {
      // move cursor esquerda
      posicao_cursor -= 1;
      if (posicao_cursor == 0) posicao_cursor = 6;
    } else if (tecla_digitada == 'D') {
      // move cursor direita
      posicao_cursor += 1;
      if (posicao_cursor == 7) posicao_cursor = 1;
    } else if (tecla_digitada == 'E') {
      // enter
      menu_2 = 0;
      posicao_cursor = 1;
    } else if (tecla_digitada <= '9') {
      // Se for um numero:
      // se o cursor estiver em P: "xx:Px:Px" => so pode de 0 a 5
      if (posicao_cursor / 2 == 0 && posicao_cursor > 1 && tecla_digitada > '5') return;
      if (menu_1 == 1) {
        insere_tecla_horario(tecla_digitada, &horas, &minutos, &segundos);
      } else if (menu_1 == 2) {
        insere_tecla_horario(tecla_digitada, &horas_alvo, &minutos_alvo, &segundos_alvo);
      } else if (menu_1 == 3) {
        insere_tecla_temperatura_alvo(tecla_digitada);
      }
    }
  }
}

void insere_tecla_horario(char n, char* horas, char* minutos, char* segundos) {
  // Insere o numero na posicao do cursor
  n -= 0x30;
  if (posicao_cursor == 6) {
    *segundos = n + (10 * (*segundos / 10));  // 00:00:0X (n*1)
  } else if (posicao_cursor == 5) {
    *segundos = (n * 10) + (*segundos % 10);  // 00:00:X0 (n*10)
  } else if (posicao_cursor == 4) {
    *minutos = n + (10 * (*minutos / 10));  // 00:0X:00 (n*1)
  } else if (posicao_cursor == 3) {
    *minutos = (n * 10) + (*minutos % 10);  // 00:X0:00 (n*10)
  } else if (posicao_cursor == 2) {
    *horas = n + (10 * (*horas / 10));  // 0X:00:00 (n*1)
  } else if (posicao_cursor == 1) {
    *horas = (n * 10) + (*horas % 10);  // X0:00:00 (n*10)
  }
  posicao_cursor += 1;
  if (posicao_cursor > 6) posicao_cursor = 1;
}

void insere_tecla_temperatura_alvo(char numero) {
  numero -= 0x30;
  if (posicao_cursor == 1) {
    // 987 => 087 => X87 (Maximo 150deg => maximo 1)
    if (numero > 1) numero = 1;
    temperatura_alvo -= (temperatura_alvo / 100) * 100;
    temperatura_alvo += 100 * numero;
  } else if (posicao_cursor == 2) {
    // 987 => 907 => 9X7 (Maximo 150deg => maximo 5)
    if (temperatura_alvo / 100 > 0 && numero > 5) numero = 5;
    temperatura_alvo -= ((temperatura_alvo / 10) % 10) * 10;
    temperatura_alvo += 10 * numero;
  } else if (posicao_cursor == 3) {
    // 987 => 980 => 98X
    temperatura_alvo -= temperatura_alvo % 10;
    temperatura_alvo += numero;
  }
  if (temperatura_alvo > 150) temperatura_alvo = 150;

  posicao_cursor += 1;
  if (posicao_cursor > 3) posicao_cursor = 1;
}

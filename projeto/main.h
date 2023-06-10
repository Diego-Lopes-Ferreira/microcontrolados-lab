#include <P18F4550.H>
#include <delays.h>
#include <timers.h>

#include "xlcd_exsto.h"

// funcoes.c
void ajusta_horas(char* horas, char* minutos, char* segundos, char max_horas);
void inverteLED(void);
void lida_com_o_menu(void);
void insere_tecla_horario(char n, char* horas, char* minutos, char* segundos);

// perifericos.c
void configura_timers(void);
void configura_perifericos(void);
void envia_serial(const rom char* mensagem);
void envia_numero_serial(char numero, char tres_casas);
void ajusta_dc_1(unsigned int dc_cpp1);
void ajusta_dc_2(unsigned int dc_cpp2);
void controla_temperatura(void);

// teclado.c
char getKey(void);  // prototipo da funcao

// telas.c
void inicializa_lcd(void);
void atualiza_menu(void);
void tela_menu_externo(void);
void tela_menu_interno(void);
void tela_testes(void);
void imprime_horario(char h, char m, char s);

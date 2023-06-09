#include <P18F4550.H>
#include <delays.h>
#include <timers.h>

#include "xlcd_exsto.h"

#define L0 PORTBbits.RB4
#define L1 PORTBbits.RB5
#define L2 PORTBbits.RB6
#define L3 PORTBbits.RB7


void imprime_horario(char h, char m, char s);
void ajusta_horas(char* horas, char* minutos, char* segundos);

// funcoes.c
void inverteLED(void);
void lida_com_o_menu(void);
void ajustaHorario(char n, char* horas, char* minutos, char* segundos);

// perifericos.c
void envia_serial(const rom char* mensagem);
void envia_numero_serial(char numero, char tres_casas);
void ajusta_dc(unsigned int dc_cpp1, unsigned int dc_cpp2);
void ajusta_dc_1(unsigned int dc_cpp1);
void ajusta_dc_2(unsigned int dc_cpp2);

// teclado.c
char getKey(void);  // prototipo da funcao

// telas.c
void atualiza_menu(void);
void tela_menu_externo(void);
void tela_menu_interno(void);

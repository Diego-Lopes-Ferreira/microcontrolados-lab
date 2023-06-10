// Tempos
extern char horas = 12;
extern char minutos = 34;
extern char segundos = 56;
extern char horas_maq = 0;
extern char minutos_maq = 0;
extern char segundos_maq = 0;
extern char horas_alvo = 0;
extern char minutos_alvo = 0;
extern char segundos_alvo = 0;

// Estados do Menu
extern char menu_1 = 0;
extern char menu_2 = 0;
extern char posicao_cursor = 0;
extern char tecla_pressionada = 0;
extern char tecla_digitada = ' ';

// Estados da Maquina
extern char maquina_ativada = 0;
extern char tempo_pausado = 0;
extern char monitoramento_ativado = 0;
extern char alarme_ligado = 0;

// Estados do Controle
extern unsigned int pwm1 = 50;
extern unsigned int pwm1_anterior = 0;
extern unsigned int pwm1_erro = 0;
extern unsigned int pwm2 = 20;
extern unsigned int pwm2_anterior = 0;
extern unsigned int pwm2_erro = 0;
extern char temperatura_alvo = 0;   // em graus C
extern char temperatura_atual = 0;  // em graus C

// Flags
extern char flag_tmr0_010ms = 0;
extern char flag_tmr0_050ms = 0;
extern char aux_tmr0_050 = 0;
extern char aux_tmr0_100 = 0;
extern char ad_finalizado = 0;

extern char i;
extern char j;
extern char k;

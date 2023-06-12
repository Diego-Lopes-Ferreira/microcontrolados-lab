// Tempos
extern char horas;
extern char minutos;
extern char segundos;
extern char horas_maq;
extern char minutos_maq;
extern char segundos_maq;
extern char horas_alvo;
extern char minutos_alvo;
extern char segundos_alvo;

// Estados do Menu
extern char menu_1;
extern char menu_2;
extern char posicao_cursor;
extern char tecla_pressionada;
extern char tecla_digitada;
extern char pisca_tempo_restante;
extern char acao_usuario;

// Estados da Maquina
extern char maquina_ativada;
extern char tempo_pausado;
extern char monitoramento_ativado;
extern char alarme_ligado;

// Estados do Controle
extern int pwm1;
extern int pwm1_anterior;
extern int pwm1_erro;
extern int pwm2;
extern int pwm2_anterior;
extern int pwm2_erro;
extern long tensao;
extern int temperatura_alvo;   // em graus C
extern int temperatura_atual;  // em graus C

// Flags
extern char flag_tmr0_010ms;
extern char flag_tmr0_050ms;
extern char flag_tmr0_500ms;
extern char flag_tmr0_1s;
extern char aux_tmr0_005;
extern char aux_tmr0_050;
extern char aux_tmr0_100;
extern char ad_finalizado;

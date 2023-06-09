

[ ] Timer
  - Configurar o timer 0
[ ] Menu com BTN0, BTN1, BTN2 e LCD
  - Configurar INT0, INT1 e INT2
  - Criar variaveis de controle do menu
[ ] Acionamento com PWM
  - Definir a frequencia
  - Calcular o PR2
  - Definir variaveis de controle
[ ] Monitoramento remoto via interface serial
  - Definir o que enviar
  - variavel de controle
[ ] Leitura de grandeza com A/D
  - ??


Maq ociosa => mostra horario
Maq trabalhando => mostra tempo
MAQUINA NO MENU
|--------------------------|
| [1/5] MENU AQUI          |
| acao que vc ta fazendo   |
|--------------------------|

MAQUINA OCIOSA
|--------------------------|
| 10:00:00                 |
| aperte BTN0              |
|--------------------------|

MAQUINA TRABALHANDO
|--------------------------|
| 00:05:34                 |
| Temp: xxC | xx mA        |
|--------------------------|

char menu_1;
char menu_2;
1 Ajusta horário
  1 - ajuste
2 Ajuste de tempo alvo
  1 - ajuste
3 Ajuste temperatura
  1 - temperatura
4 Ativa / desativar monitoramento
  1 - ativa ou desativa


char horas;
char minutos;
char segundos;

char horas_maq;
char minutos_maq;
char segundos_maq;

char temperatura; // em ºC
char monitoramento_ativado;
char maquina_ativada;

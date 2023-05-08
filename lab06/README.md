# Objetivos
- Praticar a configuração dos osciladores
- Praticar a utilização das funcionalidades especiais do PIC
- Praticar a utilização do LCD

# Etapa 1: Relógio com LCD e PLL
Projete um relógio no formato 24 horas que apresente as horas, minutos e segundos no LCD. Utilize o oscilador com cristal de 20MHz e PLL configurado para Fosc de 16 MHz. Considere as funcionalidades a seguir ao se pressionar e liberar os botões:
- O botão em RB0 coloca o relógio no modo de ajuste do horário. Se pressionado novamente, retorna o relógio para operação normal. Indique, de alguma forma, no LCD, que o sistema se encontra em ajuste
- O botão em RB1 ajusta as horas de forma crescente
- O botão em RB2 ajusta os minutos, também de forma crescente

# Etapa 2: Funcionalidades Especiais
Modifique a etapa anterior configurando o estouro do _Watchdog_ do PIC para 2.048 segundos, porém o mantenha desativado inicialmente.
- O pino RB3 ativa o _Watchdog_ por software
- O pino RB4 coloca o PIC em modo _sleep_
- No laço principal do código, reinicie o contador do _Watchdog_ com o comando ClrWdt() antes do estouro

Verifique o que acontece quando o _sleep_ é ativado com o _Watchdog_ ligado e desligado.

Utilize o DIP switch (ao lado do teclado matricial no Kit da Exsto) para acessar os pinos RB3 e RB4.

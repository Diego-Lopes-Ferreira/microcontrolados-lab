# Objetivos
- Praticar a programação em Assembly;
- Praticar a utilização dos simuladores e kit XM118;
- Praticar a interface com display de 7 segmentos.

# Etapa 1. Contador BCD Crescente
Faça um firmware em Assembly para o PIC18F4550 do circuito da Figura 1 para implementar um contador BCD (0 a 9) que apresente a contagem em um display de 7 segmentos. O botão BT1, conectado em RB1, incrementa a contagem sempre que for acionado e liberado. Elimine, em firmware, o efeito de trepidação (bounce) que possa existir no botão. Teste no simulador (PICSimLab ou Proteus) e no kit XM118. Neste último, para acionamento do display, usa-se o pino RE2.

# Etapa 2. Contador BCD Crescente e Decrescente
Mantendo a funcionalidade da etapa anterior, modifique o firmware para que o contador também possa ser decrementado toda vez que o botão BT2, conectado em RB2 for acionado e liberado. Novamente, elimine a trepidação dos botões em firmware e teste no simulador e kit.

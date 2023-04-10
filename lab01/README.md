# Objetivos:
- Praticar a programação Assembly;
- Praticar a simulação de circuitos com PIC usando PICSimLab/Proteus;
- Praticar a interface com os pinos I/O do PIC e uso do kit XM118.

# Etapa 1. Troca de Estado de LED com Botão.
No MPLAB, modifique o Exemplo 1 (feito em aula) para que, ao se pressionar o botão BT2, conectado a RB2(INT2), troque-se o estado do LED D0 ao invés de o acender somente quando o botão estiver pressionado. Utilize a estratégia de atraso para eliminar a trepidação. Verifique o funcionamento do sistema no PICSimLab ou no Proteus, utilizando o circuito da Figura 1. No PICSimLab, lembre-se que os LEDs são ativos em nível alto. Em seguida, verifique o funcionamento no kit XM118.

# Etapa 2. Acionamento de LEDs com Dois Botões.
Modifique a programação do PIC da etapa anterior para utilizar os dois botões do circuito, BT1 e BT2. O botão BT1, ao ser pressionado e liberado, troca o estado do LED D0 e o botão BT2, também ao ser pressionado e liberado, troca o estado de D1. Retire a trepidação dos botões com a estratégia do atraso. Novamente, verifique o funcionamento do sistema no PICSimLab ou Proteus e, em seguida, no kit XM118

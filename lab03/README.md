# Objetivos
- Familiarização com a programação C18 e suas funções;
- Praticar o interfaceamento com displays de 7 Segmentos.

# Etapa 1. Contador de Números Primos
Com base no circuito da Figura 1, que apresenta conexões com displays de 7 segmentos no PICSimLab, faça um firmware em C18 que implemente um contador de números primos de um dígito (2, 3, 5 e 7) e apresente em um display de 7 segmentos. Considere as funções abaixo:
- os números devem mudar a cada 1 segundo;
- o sentido da contagem (crescente ou decrescente) é controlado pelo botão BT1, ligado em RB1;
- O botão BT2, ligado em RB2, tem a função de mudar o display utilizado. Cada vez que o botão for pressionado, deve-se mudar para o display da esquerda, ao final, volta-se para o primeiro;
- deve-se retirar, em software, o efeito da trepidação (bounce) do botão. Uma sugestão é apresentada na Figura 2.

As conexões entre PIC e transistores de acionamento dos displays são diferentes no PICSimLab e kit XM118 da Exsto, sendo:
| Display           | PIC Sim Lab | XN118      |
| ------------------| ----------- | ---------- |
| Display 1 (DISP1) | PORTA, RA2  | PORTA, RA5 |
| Display 2 (DISP2) | PORTA, RA3  | PORTA, RA2 |
| Display 3 (DISP3) | PORTA, RA4  | PORTE, RE0 |
| Display 4 (DISP4) | PORTA, RA5  | PORTE, RE2 |

Etapa 2. Cronômetro
Faça um firmware em C18 para o PIC18F4550 do circuito da Figura 1, que implemente um cronômetro com 4 dígitos: os dois mais significativos indicam segundos (dezena e unidade) e os dois menos significativos indicam centésimos de segundos. Além disso, o botão BT0, ligado em RB0, pausa a contagem ao ser pressionado e a resume quando acionado novamente.

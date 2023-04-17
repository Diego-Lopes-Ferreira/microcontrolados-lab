# Objetivos
- Praticar o interfaceamento com teclado matricial.

# Etapa 1. Teclado com um Display
Com base no circuito da Figura 1, faça um firmware em C18 que detecte o botão pressionado no teclado matricial e mostre o número correspondente no display DISP4. Verifique o funcionamento no PICSimLab (ou Proteus) e kit XM118.

# Etapa 2. Sistema de Acesso
Usando o teclado matricial e os displays de 7 segmentos, construa um sistema de acesso com quatro dígitos. Ao se pressionar um botão, seu número é mostrado no DISP1, o próximo botão é apresentado no DISP2 e assim sucessivamente até o quarto botão em DISP4. Se o valor digitado for “1509”, os displays mostram “AAAA” por 3 segundos, caso contrário, mostram “−−−−” pelo mesmo tempo e o sistema reinicia.

# Interface Matricial

> Não esqueça de colocar as saídas como analógicas e desligar os pull ups do PORTB
```c
INTCON2 = 0;
ADCON1 = 0x0F;
```

## Kit XM118 da Exsto
| Teclado | PIC | IO |
|---|---|---|
| C3 | B0 | Saída |
| C2 | B1 | Saída |
| C1 | B2 | Saída |
| C0 | B3 | Saída |
| L0 | B4 | Entrada |
| L1 | B5 | Entrada |
| L2 | B6 | Entrada |
| L3 | B7 | Entrada |

## PIC SIM LAB
| Teclado | PIC | IO |
|---|---|---|
| C3 | B0 | Saída |
| C2 | B1 | Saída |
| C1 | B2 | Saída |
| C0 | B3 | Saída |
| L0 | D0 | Entrada |
| L1 | D1 | Entrada |
| L2 | D2 | Entrada |
| L3 | D3 | Entrada |

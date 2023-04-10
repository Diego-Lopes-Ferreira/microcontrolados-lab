; ET37I - Sistemas Microcontrolados
; Laboratorio 01 - Ex01

T_ATRASO  EQU .250 ; Atraso do botao

#define  BOTAO_0  PORTB, 0 ; Pino do BTN0
#define  BOTAO_1  PORTB, 1 ; Pino do BTN1
#define  BOTAO_2  PORTB, 2 ; Pino do BTN2

#define USANDO_O_KIT
#ifndef USANDO_O_KIT
  #define CC1 LATA, 2
  #define CC2 LATA, 3
  #define CC3 LATA, 4
  #define CC4 LATA, 5
#else
  #define CC1 LATA, 5
  #define CC2 LATA, 2
  #define CC3 LATE, 0
  #define CC4 LATE, 2
#endif

; CONFIGURACOES
; FOSC   = HS : Oscilador de 20MHz -> Tcy = 200ns
; WDT    = OFF: WDT desabilitado
; PBADEN = OFF: Pinos de PORTB [4:0] configurados como I/O digital.
; LVP    = OFF: Gravacao em modo de baixa tensao desabilitada
; DEBUG  = ON : Depurador habilitado, RB6 and RB7 usados para debug
  CONFIG  FOSC = HS, WDT = OFF, PBADEN = OFF, LVP = OFF, DEBUG = ON
#INCLUDE <P18F4550.INC>

  CBLOCK  0X00 ; ACCESS BANK
    CONTADOR   ; ARMAZENA O VALOR DA CONTAGEM
    ATRASO     ; ATRASO PARA O BOTAO
  ENDC

  ORG     0x00
  GOTO    INICIO

; Subrotina de conversao de um numero para o display de 7 segmentos
CONV_DISP_7SEG
  MOVF   CONTADOR, W  ; Move o contador para o W
  ANDLW  B'00001111'  ; Passa uma mascara
  MULLW  .2           ; Multiplica por 2 (PC vai de 2 em 2)
  MOVF   PRODL, W     ; Soma W ao PC
  ADDWF  PCL, F
  ;       PGFEDCBA
  RETLW B'00111111'  ; 0H - 00
  RETLW B'00000110'  ; 1H - 01
  RETLW B'01011011'  ; 2H - 02
  RETLW B'01001111'  ; 3H - 03
  RETLW B'01100110'  ; 4H - 04
  RETLW B'01101101'  ; 5H - 05
  RETLW B'01111101'  ; 6H - 06
  RETLW B'00000111'  ; 7H - 07
  RETLW B'01111111'  ; 8H - 08
  RETLW B'01101111'  ; 9H - 09
  RETLW B'01110111'  ; AH - 10
  RETLW B'01111100'  ; BH - 11
  RETLW B'00111001'  ; CH - 12
  RETLW B'01011110'  ; DH - 13
  RETLW B'01111001'  ; EH - 14
  RETLW B'01110001'  ; FH - 15


INICIO
  MOVLW  B'00000001'
  MOVWF  TRISB    ; RB1 entrada
  MOVLW  B'00000000'
  MOVWF  TRISD    ; PORTD inteiro como saida
  MOVWF  TRISE    ; PORTE inteiro como saida
  MOVWF  INTCON2  ; Pull-Ups habilitados
  MOVWF  INTCON   ; Interrupcoes desligadas

  CLRF   CONTADOR  ; Inicia o contador em 0

  ; Carrega 0 no display
  MOVLW  B'00111111'
  MOVWF  LATD
  BSF    CC4


MAIN
  ; Inicializa o atraso em "T_ATRASO"
  MOVLW T_ATRASO
  MOVWF ATRASO


VERIFICA_BT1
  BTFSC   BOTAO_1         ; Botao 1 pressionado?
  BRA     MAIN            ; Nao: volte ao main, sim:
  DECFSZ  ATRASO, F       ; Decrementa o atraso e verifica se deu 0
  BRA     VERIFICA_BT1    ; Nao deu: decrementa de novo. Se deu 0, pula essa linha
  INCF    CONTADOR, F     ; Incrementa o contador
  CALL    CONV_DISP_7SEG  ; Coloca o "codigo" do display no W
  MOVWF   LATD            ; Atualiza o PORTD


ESPERA_BT1
  BTFSS  BOTAO_1     ; Botao continua pressionado?
  BRA    ESPERA_BT1  ; Sim: continua esperando
  BRA    MAIN        ; Nao: volta pro MAIN

  END   ; Fim do programa

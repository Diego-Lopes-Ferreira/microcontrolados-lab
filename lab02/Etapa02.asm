  CONFIG  FOSC = HS, WDT = OFF, PBADEN = OFF

#INCLUDE <P18F4550.INC> ; MICROCONTROLADOR PIC18F4550

#DEFINE BTN1     PORTB,1
#DEFINE BTN2     PORTB,2
#DEFINE DISPLAY  LATD
#DEFINE ACIONA   PORTA,5

CBLOCK  0X00
  ATRASO
  CONTADOR
ENDC

  ORG 0x00
  GOTO INICIO

CONV_DISP_7SEG
  MOVF CONTADOR, W
  ANDLW B'00001111'
  MULLW .2
  MOVF PRODL,W
  ADDWF PCL,F

  RETLW  B'00111111'  ; 0H - 0 - 00
  RETLW  B'00000110'  ; 1H - 1 - 01
  RETLW  B'01011011'  ; 2H - 2 - 02
  RETLW  B'01001111'  ; 3H - 3 - 03
  RETLW  B'01100110'  ; 4H - 4 - 04
  RETLW  B'01101101'  ; 5H - 5 - 05
  RETLW  B'01111101'  ; 6H - 6 - 06
  RETLW  B'00000111'  ; 7H - 7 - 07
  RETLW  B'01111111'  ; 8H - 8 - 08
  RETLW  B'01101111'  ; 9H - 9 - 09
  RETLW  B'01110111'  ; AH - A - 10
  RETLW  B'01111100'  ; BH - B - 11
  RETLW  B'00111001'  ; CH - C - 12
  RETLW  B'01011110'  ; DH - D - 13
  RETLW  B'01111001'  ; EH - E - 14
  RETLW  B'01110001'  ; FH - F - 15

INICIO
  ; RB1 e RB2 como entrada
  MOVLW B'00000110'
  MOVWF TRISB
  ; PORTD como saida
  MOVLW B'00000000'
  MOVWF TRISD
  ; PORTA como saida
  MOVLW B'00000000'
  MOVWF TRISA
  ; Pull up habilitado
  MOVLW B'00000000'
  MOVWF INTCON2
  ; Interrupcoes desligadas
  MOVLW B'00000000'
  MOVWF INTCON
  ; Aciona o display e "limpa" ele
  BSF   ACIONA
  CLRF  DISPLAY

MAIN_LOOP
  MOVLW  B'11111111' ; 255
  MOVWF  ATRASO

VERIFICA_BTN1
  BTFSC   BTN1
  BRA     VERIFICA_BTN2
  DECFSZ  ATRASO, F
  BRA     VERIFICA_BTN1
  BRA     AUMENTA_MAIS_UM

VERIFICA_BTN2
  BTFSC   BTN2
  BRA     MAIN_LOOP
  DECFSZ  ATRASO, F
  BRA     VERIFICA_BTN2
  BRA     DECREMENTA_MAIS_UM

DECREMENTA_MAIS_UM
  DECFSZ  CONTADOR, F
  BRA     ATUALIZA_DISPLAY
  MOVLW   B'00001001' ; 9
  MOVWF   CONTADOR
  BRA     ATUALIZA_DISPLAY

AUMENTA_MAIS_UM
  INCF    CONTADOR, F
  MOVLW   B'00001010' ; 10
  CPFSEQ  CONTADOR
  BRA     AUTALIZA_DISPLAY
  CLRF    CONTADOR
  BRA     AUTALIZA_DISPLAY

ATUALIZA_DISPLAY
  CALL   CONV_DISP_7SEG
  MOVWF  DISPLAY
  BTFSS  BTN1
  BRA    ESPERA_BTN1
  BRA    ESPERA_BTN2

ESPERA_BTN1
  CALL   CONV_DISP_7SEG
  MOVWF  DISPLAY
  BTFSS  BTN1
  BRA    ESPERA_BTN1
  BRA    MAIN_LOOP

ESPERA_BTN2
  CALL   CONV_DISP_7SEG
  MOVWF  DISPLAY
  BTFSS  BTN2
  BRA    ESPERA_BTN2
  BRA    MAIN_LOOP

  END

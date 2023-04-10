; ET37I - Sistemas Microcontrolados
; LABORATORIO 01 - ETAPA 1: BOTAO ON/OFF

; CONFIGURACOES
; Deve-se iniciar a linha abaixo com tabulacao
  CONFIG  FOSC = HS, WDT = OFF, PBADEN = OFF
; FOSC = HS: Oscilador de 20MHz -> Tcy = 200ns
; WDT = OFF: WDT desabilitado
; PBADEN = OFF:  Pinos de PORTB [4:0] configurados como I/O digital.

#include <P18F4550.INC>

#define  BOTAO  PORTB, 2
#define  LED    PORTD, 0

CBLOCK  0X00
  ATRASO
ENDC

  ORG  0x00
  GOTO  INICIO

; INICIO DO PROGRAMA
INICIO
  MOVLW  B'00000100'
  MOVWF  TRISB
  MOVWF  TRISD
  MOVWF  INTCON2
  MOVWF  INTCON
  BCF    LED

MAIN_LOOP
  MOVLW  B'11111111' ; 255
  MOVWF  ATRASO

VERIFICA_BT2
  BTFSC  BOTAO
  BRA    MAIN_LOOP

  DECFSZ  ATRASO,F
  BRA     VERIFICA_BT2
  BRA     BOTAO_INVERTE

BOTAO_INVERTE
  BTFSC  LED
  BRA    APAGA_LED
  BRA    ACENDE_LED

APAGA_LED
  BCF  LED
  BRA  ESPERA_BT

ACENDE_LED
  BSF  LED
  BRA  ESPERA_BT

ESPERA_BT
  BTFSS  BOTAO
  BRA    ESPERA_BT
  BRA    MAIN_LOOP

  END

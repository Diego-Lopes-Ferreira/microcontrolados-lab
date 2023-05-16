# Objetivos
- Construir menus no LCD
- Praticar o uso de interrupções externas com botões

# Atividade
Construir um menu no LCD que apresente, na linha 1, o horário atual no formato HH:MM:SS e, na linha 2, um menu deslizante com as 3 opções abaixo:

1. Ajustar horário
1. Liga / Desliga buzzer
1. Entra em sleep

O menu da segunda linha deve ser manipulado com os 3 botões: RB0, RB1 e RB2:

| Interrupção | Porta | Funcionalidade |
| --- | --- | --- |
| INT0 | RB0 | Realiza a tarefa descrita
| INT1 | RB1 | Muda para a próxima opção
| INT2 | RB2 | Muda para a opção anterior

O LED bicolor deve ficar verde quando o relógio estiver em funcionamento e em vermelho quando estiver em ajuste.

> Para que o buzzer seja acionado, deve-se gerar um sinal pulsado no pino

# TODO
- [ ] Criar e configurar as interrupções dos botões
- [ ] Criar a função que lida com as interrupções

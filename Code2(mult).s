;Multiplicacao:
;1. Olha se o MSB(bit mais importante) do multiplicador(N2) eh 1, se for vai adicionar
    ;o multiplicando(N1) ao resultado, caso contrario faz nada
;2. Faz o shift para a esquerda de N1
;3. Terminou os 8 bits? Se sim, fim do programa, caso contrario volta ao 1.

;No caso desse exemplo faremos a multiplicacao de 11*14, onde 11 sera o multiplicando(N1)
    ;e 14 o multiplicador(N2)

;Faz com que essas variaveis recebam enderecos de memoria
MEM     N1     =$51   ;endereco para o multiplicando
        N2     =$52   ;endereco para o multiplicador
        TEMP   =$53   ;temporario, para guarda o bit depois do ASL
        RESUL1 =$54   ;onde a "maior" parte do resultado vai ser guardada
        RESUL2 =$55   ;onde a "menor" parte do resultado vai ser guardada
                        ;o resultado eh separado em 2 para caso de conta com 2 valores
                            ;de 8 bits, resultando em algo com 16 bits
;Exemplo do resul1 e resul2: para o numero 219 = 11011011 em binario, resul1 guardaria
    ;1101 e resul2 seria 1011

;Faz as preparacoes para iniciar o calculo
INICIO  LDA     #0      ;carrega 0 no acumulador(A)
        STA     TEMP    ;limpa o endereco (zerando ele)
        STA     RESUL1  ;   "           "
        STA     RESUL2  ;   "           "
        LDX     #8      ;carrega 8 em X, que sera usado como contador
        LDA     #11     ;A=11
        STA     N1      ;N1=11
        LDA     #14     ;A=14
        STA     N2      ;N2=14

;Compara o MSB e faz a soma
MUL     LSR     N2      ;faz o shift pra direita do multiplicador (tambem faz com que
                            ;o bit mais importante de N2 va para a flag carry (C))
        BCC     PASSA   ;compara se C e igual a 0, se for faz o pulo
        LDA     RESUL2  ;carrega A com a menor resultado ate agora
        CLC             ;limpa o C, pra nao interferir no resultado
        ADC     N1      ;adiciona o multiplicando ao resultado
        STA     RESUL2  ;guarda esse novo calor
        LDA     RESUL1  ;carrega A com a maior metade resultado ate agora
        ADC     TEMP
        STA     RESUL1  ;guarda esse novo valor

;Faz o shift do N1 pra esquerda
PASSA   ASL     N1      ;faz o shift do multiplicando pra esquerda e manda o bit que sera
                            ;"perdido" para C
        ROL     TEMP    ;carrega esse bit de C para TEMP
        DEX             ;diminui X em 1
        BNE     MUL     ;volta pro MUL se a flag zero(Z)=0, Z vai ser setado pra 1 quando
                            ;o DEX resultar em X=0

        BRK             ;encerra

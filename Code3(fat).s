;Fatorial: 5!=120
    ;Nesse código optamos por usar um metodo de multiplicacao mais simples, ele faz o calculo fatorial de um numero e suporta um resultado
    ;de ate 8 bits

;Faz com que essas variaveis recebam enderecos de memoria
MEM     NUM    =$56     ;numero que sera usado para calcular o fatorial
        AUX    =$57     ;variavel auxiliar
        RESUL  =$58     ;resultado da conta

;Faz as preparacoes para iniciar o calculo
INICIO  LDA     #5      ;carrega 5 no acumulador
        STA     NUM     ;salva o valor do acumulador em NUM
        LDA     #1      ;A=1
        STA     RESUL   ;RESUL=A
        LDX     #1      ;X=1
        LDY     #0      ;Y=0
        CMP     NUM     ;compara se A=NUM, se for Z=1
        BEQ     FIM     ;se Z=1, faz o pulo

;Faz as multiplicacoes
MUL     CLC             ;limpa o carry
        STA     AUX     ;AUX=A
        ADC     RESUL   ;adiciona A=A+A
        STA     RESUL   ;salva esse novo valor
        INY             ;I++
        CPX     Y       ;compara X com Y, se igual, Z=1
        BEQ     PASSA   ;se X=Y, passa
        LDA     AUX     ;A=AUX
        JMP     MUL     ;volta pro inicio do loop

;Incrementa o contador X e compara com NUM
PASSA   INX             ;X++
        TXA             ;copia o valor de X pra A
        CMP     NUM     ;se X=NUM, Z=1
        BEQ     FIM     ;se Z=1, fim do programa
        LDA     RESUL   ;A=AUX
        LDY     #0      ;Y=0
        JMP     MUL     ;volta pro inicio do loop

FIM     BRK             ;encerra o programa
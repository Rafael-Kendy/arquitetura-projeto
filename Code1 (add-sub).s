;Soma e subtracao simples: RESUL = 10 + (5-3)
    ;O resultado da maioria das operacoes fica automaticamente salvo no registrador
    ;acumulador(A), ele tambem eh usado como argumento da maioria das operacoes.

INICIO  RESUL  =$0400
        LDA     #0      ;carrega 0 A
        STA     RESUL   ;limpa o endereco
        CLC             ;limpa a flag carry pra nao interferir na conta

        LDA     #5      ;carrega A com 5 (A=5)
        SBC     #3      ;subtrai o valor em A por 3 (A=A-3)
        ADC     #10     ;adiciona o valor de A em 10 (A=A+10)

        STA     RESUL   ;guarda o valor de A em RESUL

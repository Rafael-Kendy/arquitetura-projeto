#include <stdio.h>
#include <stdlib.h>

#define SIZE 256

unsigned char ram;//vetor de inteiros de 16 bits cada
unsigned char opcode, outro;
int pc=0, ci=0;//program counter, current instruction

//le o arquivo===================================================================================================================
int le(unsigned char *buffer, FILE *arq){
    fread(buffer, sizeof(unsigned char), 4, arq);
    unsigned int resul=(buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];//junta esses 4 bytes em uma variavel
    return resul;
}//le

//main===========================================================================================================================
int main(){
    int inst_lidas;
    char *nome_arq, *msg;
    unsigned int opcode, variavel;
    unsigned char buffer[4];
    size_t bytesLidos;
    int menu;

//Abertura do arquivo=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    printf("Qual codigo deseja visualizar? 1-Simples, 2-Medio, 3-Complexo: ");
    scanf("%i", &menu);
    setbuf(stdin, NULL);
    if(menu!=1 && menu!=2 && menu!=3){
        do{
            printf("Escolha uma das opcoes: ");
            scanf("%i", &menu);
            setbuf(stdin, NULL);
        }while(menu!=1 && menu!=2 && menu!=3);
    }//if errado

    if(menu==1){
        nome_arq="Code1.dat";
        msg="No codigo escolhido, o valor 5 e subtraido por 3, resultando em 2. Em seguida, 10 e adicionado a 2, resultando em 12. O valor final de 12 e armazenado na variavel RESUL, no endereco $50.";
    }else if(menu==2){
        nome_arq="Code2.dat";
    }else if(menu==3){
        nome_arq="Code3.dat";
    }//if op menu

    FILE *arq=fopen(nome_arq, "rb");
    if(arq==NULL){
        printf("Erro ao abrir arquivo.");
        exit(1);
    }//if erro

//declara os registradores e as flags=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    int acumulador, x, y;
    int carry, zero;
    int resul;

//reconhece as instrucoes=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    system("cls");
    printf("%s\n", msg);
    printf("\nPressione ENTER para a proxima instrucao.\n\n");
    do{
        opcode=le(buffer, arq);
        switch(opcode){
        //LDA-------------------------------------------------------------------
            case 0x10101001:
                variavel=le(buffer, arq);
                printf("\tLDA(%08X) com imediato(%08X): ", opcode, variavel);

                    switch (variavel){
                        case 0x00000000:
                            acumulador=0;
                            printf("A=%i.\n", acumulador);
                        break;
                        case 0x00000101:
                            acumulador=5;
                            printf("A=%i.\n", acumulador);
                        break;
                    }//switch variavel

                break;
        //STA-------------------------------------------------------------------
            case 0x10000101:
                variavel=le(buffer, arq);
                printf("\tSTA(%08X) com endereco zero page(%08X): ", opcode, variavel);

                    switch (variavel){
                        case 0x01010000:
                            resul=acumulador;
                            printf("$50=%i.\n", acumulador);
                        break;
                    }//switch variavel

                break;
        //SBC-------------------------------------------------------------------
            case 0x11101001:
                variavel=le(buffer, arq);
                printf("\tSBC(%08X) com imediato(%08X): ", opcode, variavel);

                    switch (variavel){
                        case 0x00000011:
                            acumulador=acumulador-3;
                            printf("A=A-3 -> A=%i.\n", acumulador);
                        break;
                    }//switch variavel

                break;
        //ADC-------------------------------------------------------------------
            case 0x01101001:
                variavel=le(buffer, arq);
                printf("\tADC(%08X) com imediato(%08X): ", opcode, variavel);

                    switch (variavel){
                        case 0x00001010:
                            acumulador=acumulador+10;
                            printf("A=A+10 -> A=%i.\n", acumulador);
                        break;
                    }//switch variavel

                break;
        //CLC-------------------------------------------------------------------
            case 0x00011000:
                carry=0;
                printf("\tCLC(%08X): C=%i.\n", opcode, carry);
                break;
        //BRK-------------------------------------------------------------------
            case 0x00000000:
                printf("\tBRK(%08X).\n", opcode);
                break;
            }//switch opcode

        //----------------------------------------------------------------------
        getchar();
    }while(opcode!=0x00000000);

//enceramento=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    fclose(arq);
    return 0;
}//main
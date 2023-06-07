#include <stdio.h>
#include <stdlib.h>

#define SIZE 64000//8kb
#define RESUL "$50"
#define N1 "$51"
#define N2 "$52"
#define TEMP "$53"
#define RESUL1 "$54"
#define RESUL2 "$55"

unsigned int ram[SIZE];

typedef struct {
    int A;//Acumulador
    int X;//Registrador X
    int Y;//Registrador Y
    int PC;//Contador do programa
}Registradores;

typedef struct{
    int C;//Carry
    int Z;//Zero
    int I;//Interrupções
    int D;//Modo Decimal
    int B;//Break
    int V;//Overflow
    int N;//Negativo
}Flags;

typedef struct {
    Registradores reg;//Registradores
    Flags flags;//Flags
}EstadoCPU;

//main===========================================================================================================================
int main(){
    int inst_lidas=0;//Qtde de instrucoes lidas
    char *nome_arq, *msg;//Nome do arquivo e mensagem de explicacao sobre o codigo
    unsigned int opcode, variavel;
    int menu;//Controla qual codigo foi escolhido
    EstadoCPU CPU;//Estado da CPU
    unsigned char temp[4];

//Abertura do arquivo=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    printf("Qual codigo deseja visualizar? 1-, 2-, 3-: ");
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
        msg="Explicacao codigo 1.\n";
    }else if(menu==2){
        nome_arq="Code2.dat";
        msg="Explicacao codigo 2.\n";
    }else if(menu==3){
        nome_arq="Code3.dat";
        msg="Explicacao codigo 3.\n";
    }//if op menu

    FILE *arq=fopen(nome_arq, "rb");
    if(arq==NULL){
        printf("Erro ao abrir arquivo.");
        exit(1);
    }//if erro

//Faz a leitura do arquivo e inicializa a CPU=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    for(int i=0; i<100; i++){
        fread(temp, sizeof(unsigned char), 4, arq);
        ram[i]=(temp[0] << 24) | (temp[1] << 16) | (temp[2] << 8) | temp[3];//Junta esses 4 em uma variavel
    }//for
    
    CPU.reg.A=0;
    CPU.reg.X=0;
    CPU.reg.Y=0;
    CPU.reg.PC=0;
    CPU.flags.C=0;
    CPU.flags.Z=0;
    CPU.flags.I=0;
    CPU.flags.D=0;
    CPU.flags.B=0;
    CPU.flags.V=0;
    CPU.flags.N=0;

    int IC=0;//Para navegar dentro de uma instrucao
    int c1_resul;//Variaveis do codigo 1
    int c2_temp, c2_resul1, c2_resul2, c2_n1, c2_n2;//Variaveis do codigo 2

//Reconhece as instrucoes=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    system("cls");
    printf("%s\n", msg);
    printf("\nPressione ENTER para a proxima instrucao.\n\n");

    do{
        opcode=ram[IC];
        printf("\t[PC-%02i]\n", CPU.reg.PC);
        CPU.reg.PC++;
        IC++;
        switch(opcode){
        //LDA-------------------------------------------------------------------
            case 0x10101001:
                variavel=ram[IC];
                printf("\tLDA(%08X) com imediato(%08X): ", opcode, variavel);

                switch (variavel){
                    case 0x00000000:
                        CPU.reg.A=0;
                        printf("A=%i.\n", CPU.reg.A);
                        break;
                    case 0x00000101:
                        CPU.reg.A=5;
                        printf("A=%i.\n", CPU.reg.A);
                        break;
                    case 0x00001011:
                        CPU.reg.A=11;
                        printf("A=%i.\n", CPU.reg.A);
                        break;
                    case 0x00001110:
                        CPU.reg.A=14;
                        printf("A=%i.\n", CPU.reg.A);
                        break;
                }//switch variavel

                IC++;
                break;

            case 0x10100101:
                variavel=ram[IC];
                printf("\tLDA(%08X) com endereco zero page(%08X): ", opcode, variavel);

                switch (variavel){
                    case 0x01010100:
                        CPU.reg.A=c2_resul1;
                        printf("A=%i(%s).\n", CPU.reg.A, RESUL1);
                        break;
                }//switch variavel

                IC++;
                break;
        //LDX-------------------------------------------------------------------
            case 0x10100010:
                variavel=ram[IC];
                printf("\tLDX(%08X) com imediato(%08X): ", opcode, variavel);

                switch (variavel){
                    case 0x00001000:
                        CPU.reg.X=8;
                        printf("X=%i.\n", CPU.reg.X);
                        break;
                }//switch variavel

                IC++;
                break;
        //STA-------------------------------------------------------------------
            case 0x10000101:
                variavel=ram[IC];
                printf("\tSTA(%08X) com endereco zero page(%08X): ", opcode, variavel);

                switch (variavel){
                    case 0x01010000:
                        c1_resul=CPU.reg.A;
                        printf("%s=%i.\n", RESUL, c1_resul);
                        break;
                    case 0x01010001:
                        c2_n1=CPU.reg.A;
                        printf("%s=%i.\n", N1, c1_resul);
                        break;
                    case 0x01010010:
                        c2_n2=CPU.reg.A;
                        printf("%s=%i.\n", N2, c1_resul);
                        break;
                    case 0x01010011:
                        c2_temp=CPU.reg.A;
                        printf("%s=%i.\n", TEMP, c1_resul);
                        break;
                    case 0x01010100:
                        c2_resul1=CPU.reg.A;
                        printf("%s=%i.\n", RESUL1, c1_resul);
                        break;
                    case 0x01010101:
                        c2_resul2=CPU.reg.A;
                        printf("%s=%i.\n", RESUL2, c1_resul);
                        break;
                }//switch variavel

                IC++;
                break;
        //SBC-------------------------------------------------------------------
            case 0x11101001:
                variavel=ram[IC];
                printf("\tSBC(%08X) com imediato(%08X): ", opcode, variavel);

                switch (variavel){
                    case 0x00000011:
                        CPU.reg.A=CPU.reg.A-3;
                        printf("A=A-3 -> A=%i.\n", CPU.reg.A);
                        break;
                }//switch variavel

                IC++;
                break;
        //ADC-------------------------------------------------------------------
            case 0x01101001:
                variavel=ram[IC];
                printf("\tADC(%08X) com imediato(%08X): ", opcode, variavel);

                switch (variavel){
                    case 0x00001010:
                        CPU.reg.A=CPU.reg.A+10;
                        printf("A=A+10 -> A=%i.\n", CPU.reg.A);
                        break;
                }//switch variavel

                IC++;
                break;

            case 0x01100101:
                variavel=ram[IC];
                printf("\tADC(%08X) com endereco zero page(%08X): ", opcode, variavel);

                switch (variavel){
                    case 0x01010001:
                        CPU.reg.A=CPU.reg.A+c2_n1;
                        printf("A=A+%i(%s) -> A=%i.\n", CPU.reg.A, c2_n1, N1);
                        break;
                    case 0x01010011:
                        CPU.reg.A=CPU.reg.A+c2_n1;
                        printf("A=A+%i(%s) -> A=%i.\n", CPU.reg.A, c2_n1, N1);
                        break;
                }//switch variavel

                IC++;
                break;
        //LSR-------------------------------------------------------------------
            case 0x01000110:
                variavel=ram[IC];
                printf("\tLSR(%08X) com endereco zero page(%08X): ", opcode, variavel);

                switch (variavel){
                    case 0x01010010:
                        c2_n2="ARRUMA AKI Ó";---------------------------------------------------
                        printf("%s=%i.\n", N2, c2_n2);
                        break;
                }//switch variavel

                IC++;
                break;
        //BCC-------------------------------------------------------------------
            case 0x10010000:
                int novo_end;
                variavel=ram[IC];
                printf("\tBCC(%08X) com para o endereco relativo(%08X): ", opcode, variavel);

                switch (variavel){
                    case 0x01010010:
                        novo_end=35;
                        printf("%i=%i.\n", PC, novo_end);
                        break;
                }//switch variavel

                CPU.reg.PC=novo_end;
                IC=novo_end;
                break;
        //CLC-------------------------------------------------------------------
            case 0x00011000:
                CPU.flags.C=0;
                printf("\tCLC(%08X): C=%i.\n", opcode, CPU.flags.C);
                break;
        //BRK-------------------------------------------------------------------
            case 0x00000000:
                printf("\tBRK(%08X).\n", opcode);
                break;
            }//switch opcode
        //----------------------------------------------------------------------
        getchar();
        
    }while(opcode!=0x00000000);

//Enceramento=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    fclose(arq);
    return 0;
}//main
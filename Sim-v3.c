#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 64000//8kb
#define RESUL "$50"
#define N1 "$51"
#define N2 "$52"
#define TEMP "$53"
#define RESUL1 "$54"
#define RESUL2 "$55"
#define NUM "$56"
#define AUX "$57"
#define RESULA "$58"

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

//ROL============================================================================================================================
int RotateLeft(int num, EstadoCPU *CPU){
	int vet_bin[8];
	int i=0, k=6;

	while(num > 0){
		vet_bin[i] = num % 2;
		i++;
		num = num / 2;
	}//while pega o resto da divisão de num por 2

    if(i!=7){
        for(int j=i; j<=7; j++){
            vet_bin[j]=0;
        }//for
    }//if para preencher o resto do vetor com 0

    if(vet_bin[7]==1){
        CPU->flags.C=1;
    }else{
        CPU->flags.C=0;
    }//seta o carry

    for(i=7; i>=0; i--){
        if(i==0){ k=7; }
        vet_bin[i]=vet_bin[k];
        k=i-2;
    }//faz a rotacao dos bits

    vet_bin[0]=CPU->flags.C;

    int decimal = 0;

    for (int i=7; i>=0; i--) {
        decimal = (decimal << 1) + vet_bin[i];
    }//volta pra decimal

    return decimal;
}//RotateLeft

//main===========================================================================================================================
int main(){
    int inst_lidas=0;//Qtde de instrucoes lidas
    char *nome_arq, *msg;//Nome do arquivo e mensagem de explicacao sobre o codigo
    unsigned int opcode, variavel;
    int menu;//Controla qual codigo foi escolhido
    EstadoCPU CPU;//Estado da CPU
    unsigned char temp[4];

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
        msg="No codigo escolhido, o valor 5 e subtraido por 3, resultando em 2. Em seguida, 10 e adicionado a 2, resultando em 12. O valor final de 12 e armazenado na variavel RESUL, no endereco $50.\n";
    }else if(menu==2){
        nome_arq="Code2.dat";
        msg="No codigo escolhido, faremos uma multiplicacao de dois numeros, 11(N1) e 14(14), o resultado sera 154 e ficara salvo em RESUL1($54) e RESUL2($55), lembrando que nao existe uma intrucao direta para multiplicacao nessa arquitetura.\n";
    }else if(menu==3){
        nome_arq="Code3.dat";
        msg="No codigo escolhido, faremos o calculo fatorial de um numero e suporta um resultado de ate 8 bits, nesse exemplo vamos calcular 5!=120.\n";
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
    int c3_num, c3_aux, c3_resula;//Variaveis do codigo 2

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
                    case 0x00000001:
                        CPU.reg.A=1;
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
                    case 0x01010101:
                        CPU.reg.A=c2_resul2;
                        printf("A=%i(%s).\n", CPU.reg.A, RESUL2);
                        break;
                    case 0x01010111:
                        CPU.reg.A=c3_aux;
                        printf("A=%i(%s).\n", CPU.reg.A, AUX);
                        break;
                    case 0x01011000:
                        CPU.reg.A=c3_resula;
                        printf("A=%i(%s).\n", CPU.reg.A, RESULA);
                        break;
                }//switch variavel

                IC++;
                break;
        //LDX-------------------------------------------------------------------
            case 0x10100010:
                variavel=ram[IC];
                printf("\tLDX(%08X) com imediato(%08X): ", opcode, variavel);

                switch (variavel){
                    case 0x00000001:
                        CPU.reg.X=1;
                        printf("X=%i.\n", CPU.reg.X);
                        break;
                    case 0x00001000:
                        CPU.reg.X=8;
                        printf("X=%i.\n", CPU.reg.X);
                        break;
                }//switch variavel

                IC++;
                break;
        //LDY-------------------------------------------------------------------
            case 0x10100000:
                variavel=ram[IC];
                printf("\tLDY(%08X) com imediato(%08X): ", opcode, variavel);

                switch (variavel){
                    case 0x00000000:
                        CPU.reg.Y=0;
                        printf("Y=%i.\n", CPU.reg.Y);
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
                        printf("%s=%i.\n", N1, c2_n1);
                        break;
                    case 0x01010010:
                        c2_n2=CPU.reg.A;
                        printf("%s=%i.\n", N2, c2_n2);
                        break;
                    case 0x01010011:
                        c2_temp=CPU.reg.A;
                        printf("%s=%i.\n", TEMP, c2_temp);
                        break;
                    case 0x01010100:
                        c2_resul1=CPU.reg.A;
                        printf("%s=%i.\n", RESUL1, c2_resul1);
                        break;
                    case 0x01010101:
                        c2_resul2=CPU.reg.A;
                        printf("%s=%i.\n", RESUL2, c2_resul2);
                        break;
                    case 0x01010110:
                        c3_num=CPU.reg.A;
                        printf("%s=%i.\n", NUM, c3_num);
                        break;
                    case 0x01010111:
                        c3_aux=CPU.reg.A;
                        printf("%s=%i.\n", AUX, c3_aux);
                        break;
                    case 0x01011000:
                        c3_resula=CPU.reg.A;
                        printf("%s=%i.\n", RESULA, c3_resula);
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
                        printf("A=A+%i(%s) -> A=%i.\n", CPU.reg.A, N1, c2_n1);
                        break;
                    case 0x01010011:
                        CPU.reg.A=CPU.reg.A+c2_temp;
                        printf("A=A+%i(%s) -> A=%i.\n", CPU.reg.A, c2_temp, TEMP);
                        break;
                    case 0x01011000:
                        CPU.reg.A=CPU.reg.A+c3_resula;
                        printf("A=A+%i(%s) -> A=%i.\n", c3_resula, RESULA, CPU.reg.A);
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
        //INX-------------------------------------------------------------------
            case 0x11101000:
                printf("\tINX(%08X): ", opcode);
                CPU.reg.X++;
                printf("X=%i.\n", CPU.reg.X);
                break;
        //INY-------------------------------------------------------------------
            case 0x11001000:
                printf("\tINY(%08X): ", opcode);
                CPU.reg.Y++;
                printf("Y=%i.\n", CPU.reg.Y);
                break;
        //DEX-------------------------------------------------------------------
            case 0x11001010:
                printf("\tDEX(%08X): ", opcode);
                CPU.reg.X--;
                printf("X=%i.\n", CPU.reg.X);
                if(CPU.reg.X==0){ CPU.flags.Z=1; }
                break;
        //TXA-------------------------------------------------------------------
            case 0x10001010:
                printf("\tTXA: ");
                CPU.reg.A=CPU.reg.X;
                printf("A=%i(X).\n", CPU.reg.X);
                break;
        //ROL-------------------------------------------------------------------
            case 0x00100110:
                variavel=ram[IC];
                printf("\tROL(%08X) com endereco zero page(%08X): ", opcode, variavel);

                switch (variavel){
                    case 0x01010011:
                        c2_temp=RotateLeft(c2_temp, &CPU);
                        printf("%s=%i\n", TEMP, c2_temp);
                        break;
                }//switch variavel

                IC++;
                break;
        //ASL-------------------------------------------------------------------
            case 0x00000110:
                variavel=ram[IC];
                printf("\tALS(%08X) com endereco zero page(%08X): ", opcode, variavel);

                switch (variavel){
                    case 0x01010001:
                        c2_n1=c2_n1*2;
                        printf("%s=%i.\n", N1, c2_n1);
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
                        if(c2_n2%2==1){
                            CPU.flags.C=1;
                        }else{
                            CPU.flags.C=0;
                        }//if carry "perdido"
                        c2_n2=c2_n2/2;
                        printf("%s=%i.\n", N2, c2_n2);
                        break;
                }//switch variavel

                IC++;
                break;
        //CMP-------------------------------------------------------------------
            case 0x11000101:
                variavel=ram[IC];
                printf("\tCMP(%08X) com endereco zero page(%08X): ", opcode, variavel);

                switch (variavel){
                    case 0x01010110:
                        if(CPU.reg.A==c3_num){
                            CPU.flags.Z=1;
                        }else{ 
                            CPU.flags.Z=0;
                        }//if seta/limpa Z
                        printf("%i(A)=%i(%s)? -> Z=%i.\n", CPU.reg.A, c3_num, NUM, CPU.flags.Z);
                        break;
                }//switch variavel

                IC++;
                break;
        //CPX-------------------------------------------------------------------
            case 0x11100100:
                variavel=ram[IC];
                printf("\tCMP(%08X) com endereco zero page(%08X): ", opcode, variavel);

                switch (variavel){
                    case 0x00000001:
                        if(CPU.reg.X==CPU.reg.Y){
                            CPU.flags.Z=1;
                        }else{ 
                            CPU.flags.Z=0;
                        }//if seta/limpa Z
                        printf("%i(X)=%i(Y)? -> Z=%i.\n", CPU.reg.X, CPU.reg.Y, CPU.flags.Z);
                        break;
                }//switch variavel

                IC++;
                break;
        //BEQ-------------------------------------------------------------------
            case 0x11110000:
                variavel=ram[IC];
                printf("\tBEQ(%08X) com para o endereco relativo(%08X): ", opcode, variavel);

                switch (variavel){
                    case 0x00010010:
                    case 0x00000100:
                        if(CPU.flags.Z==1){
                            CPU.reg.PC--;
                            printf("Branch realizado - [PC]%i->%i.\n", CPU.reg.PC, 24);
                            CPU.reg.PC=24;
                            IC=44;
                        }else{
                            printf("Brach nao realizado.\n");
                            IC++;
                        }//if
                        break;
                    case 0x00000011:
                        if(CPU.flags.Z==1){
                            CPU.reg.PC--;
                            printf("Branch realizado - [PC]%i->%i.\n", CPU.reg.PC, 17);
                            CPU.reg.PC=17;
                            IC=32;
                        }else{
                            printf("Branch nao realizado.\n");
                            IC++;
                        }//if
                        break;
                }//switch variavel

                break;
        //BNE-------------------------------------------------------------------
            case 0x11010000:
                variavel=ram[IC];
                printf("\tBNE(%08X) com para o endereco relativo(%08X): ", opcode, variavel);

                switch (variavel){
                    case 0x00001100:
                        if(CPU.flags.Z==0){
                            CPU.reg.PC--;
                            printf("Branch realizado - [PC]%i->%i.\n", CPU.reg.PC, 9);
                            CPU.reg.PC=9;
                            IC=18;
                        }else{
                            printf("Branch nao realizado.\n");
                            IC++;
                        }//if
                        break;
                }//switch variavel

                break;
        //BCC-------------------------------------------------------------------
            case 0x10010000:
                variavel=ram[IC];
                printf("\tBCC(%08X) com para o endereco relativo(%08X): ", opcode, variavel);

                switch (variavel){
                    case 0x00001000:
                        if(CPU.flags.C==0){
                            CPU.reg.PC--;
                            printf("Branch realizado - [PC]%i->%i.\n", CPU.reg.PC, 18);
                            CPU.reg.PC=18;
                            IC=35;
                        }else{
                            printf("Branch nao realizado.\n");
                            IC++;
                        }//if
                        break;
                }//switch variavel

                break;
        //JMP-------------------------------------------------------------------
            case 0x01001100:
                variavel=ram[IC];
                printf("\tJMP(%08X) com para o endereco absoluto(%08X).\n", opcode, variavel);
                CPU.reg.PC=8;
                IC=16;
                break;
        //CLC-------------------------------------------------------------------
            case 0x00011000:
                CPU.flags.C=0;
                printf("\tCLC(%08X): C=%i.\n", opcode, CPU.flags.C);
                break;
        //BRK-------------------------------------------------------------------
            case 0x00000000:
                printf("\tBRK(%08X).\n", opcode);
                if(strcasecmp(nome_arq, "Code1.dat")==0){
                    printf("\tRESUL(%s)=%i.\n", RESUL, c1_resul);
                }else if(strcasecmp(nome_arq, "Code2.dat")==0){
                    printf("\tRESUL1(%s)=%i; RESUL2(%s)=%i.\n", RESUL1, c2_resul1, RESUL2, c2_resul2);
                }else if(strcasecmp(nome_arq, "Code3.dat")==0){
                    printf("\tRESUL(%s)=%i.\n", RESULA, c3_resula);
                }//if print resultado
                break;
            }//switch opcode
        //----------------------------------------------------------------------
        getchar();
        
    }while(opcode!=0x00000000);

//Enceramento=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    fclose(arq);
    return 0;
}//main
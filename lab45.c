#include"lab45.h"

#define CMAX 32
#define MAX_intr 32
#define stdfname "Soma.txt\0"

void readBin(int *X, int n, char *text)
{
    int aux;

    printf("Novo valor de %s: ", text);
    scanf("%d", &aux);
    
    while(Bin_Array(aux, X, n) == 0)
    {
        printf("Erro: valor invalido! \n");
        printf("Novo valor de %s: ", text);
        scanf("%d", &aux);
    }
}

int print_Array(int *v, int n)
{
    if(v == NULL)
    {
        fprintf(stderr, "Uninitialised value: v\n");
        return -1;
    }
    if(!v || n < 1) return -1;
    
    for (int i = 0; i < n; i++)printf ("%d", v[i]);
    printf("    ");

    return 1;
}

void printComandos(int OPR[3], int SEL[3], int CE[2], int X[8],int FLAGS[4])
{
    printf("\n\n\t\t\t\t      FLAGS ");
    printf("\nOPR    SEL    CE       X\t     O C S Z\n");

    print_Array(OPR, 3);
    print_Array(SEL, 3);
    print_Array(CE, 2);
    print_Array(X, 8);
    printf("     %d %d %d %d \n", FLAGS[3], FLAGS[2], FLAGS[1], FLAGS[0] );
}

void PrintNexys4(int MOD, int OPR[3], int SEL[3], int CE[2], int X[8], int E[5], int A[8], int R1[8], int R2[8], int R3[8], int FLAGS[4], int R[8], int *B)
{
    int v1[8] = {0,0,0,0,0,0,0,1};
    int v15[8] = {1,1,1,1,1,1,1,1};
    
    int C[8] = {0, 0, 0, 0, 0, 0, 0, FLAGS[1]};
    int *out_SEL[8] = {A, R1, R2, R3, X, v1, v15, C};
    B = muxN_1(SEL, out_SEL, 8);

    char *ME = BinHex(E, 5);
    char *MR = BinHex(R, 8);
    char *MA = BinHex(A, 8);
    char *MB = BinHex(B, 8);
    char *MR1 = BinHex(R1, 8);
    char *MR2 = BinHex(R2, 8);
    char *MR3 = BinHex(R3, 8);
    
    puts("\n\n");
    if(MOD == 0)
    {
    printf("  \tR \t A \t B \n");
    printf("00\t%c%c\t %c%c\t %c%c", MR[0], MR[1], MA[0], MA[1], MB[0], MB[1] );
    printComandos(OPR, SEL, CE, X, FLAGS);
    }

    if(MOD == 1)
    {
    printf("E \tR \t A \t B \n");
    printf("%c%c\t%c%c\t %c%c\t %c%c", ME[0], ME[1], MR[0], MR[1], MA[0], MA[1], MB[0], MB[1] );
    printf("\n\n\t\t\t\t      FLAGS ");
    printf("\n                       X\t     O C S Z\n\t            ");
    print_Array(X, 8);
    printf("     %d %d %d %d \n", FLAGS[3], FLAGS[2], FLAGS[1], FLAGS[0] );
    }

    if(MOD == 2)
    {
    printf("A \tR1 \t R2 \t R3 \n");
    printf("%c%c\t%c%c\t %c%c\t %c%c", MA[0], MA[1], MR1[0], MR1[1], MR2[0], MR2[1], MR3[0], MR3[1] );
    printf("\n\n\t\t\t\t      FLAGS ");
    printf("\n                       X\t     O C S Z\n\t            ");
    print_Array(X, 8);
    printf("     %d %d %d %d \n", FLAGS[3], FLAGS[2], FLAGS[1], FLAGS[0] );
    }

    free(ME);
    free(MA);
    free(MB);
    free(MR);
    free(MR1);
    free(MR2);
    free(MR3);
}

int printROM(int **ROM, int tam)
{
    if(!ROM)
    {
        fprintf( stderr, "Uninitialised value: ROM");
        return -1;
    } 
    
    for (int i = 0; i < tam; i++)
    {
        if(!ROM[i])
        {
            fprintf(stderr, "Uninitialised value: ROM[%d]\n", i);
            return -1;
        }
        
        if (!print_Array(ROM[i], 8) )
        {
            fprintf(stderr, "print_Array returned -1\n");
            return -1;
        }
        puts("");
    }
    return 1;
}

void circuit(int clock[2], int reset, int OPR[3], int SEL[3], int CE[2], int X[8], int A[8], int R1[8], int R2[8], int R3[8], int *R_ALU, int regFLAGS[4], int B[8], int jump)
{
    int v1[8] = {0,0,0,0,0,0,0,1};
    int v15[8] = {1,1,1,1,1,1,1,1};
    int C[8] = {0, 0, 0, 0, 0, 0, 0, regFLAGS[1]};
    int *out_SEL[8] = {A, R1, R2, R3, X, v1, v15, C};
    int FlagsALU[4] = {};

    B = muxN_1(SEL, out_SEL, 8);
    ALUcompleta(A, B, OPR, regFLAGS[2], R_ALU, FlagsALU);

    Reg(clock, 0, 1, reset, FlagsALU, regFLAGS, 4);
    
    int w0, w1, w2, w3;
    dec2_4(CE, &w0, &w1, &w2, &w3, !jump);
    
    Reg(clock, 0, w0, reset, R_ALU, A, 8);
    Reg(clock, 0, w1, reset, R_ALU, R1, 8);
    Reg(clock, 0, w2, reset, R_ALU, R2, 8);
    Reg(clock, 0, w3, reset, R_ALU, R3, 8);
    
    ALUcompleta(A, B, OPR, regFLAGS[2], R_ALU, FlagsALU);
}

void Lab4(int A[8], int *B,  int R1[8], int R2[8],  int R3[8], int regFLAGS[4], int R_ALU[8], int X[8], int *OPR,int *SEL, int *CE, int clock[2], int reset[2])
{
    char menuLab4;
    
    int userInput[8] = {};

    OPR = userInput;
    SEL = userInput + 3;
    CE = userInput + 6;

    printf("\t\t   LAB 4 \n");
    
    do
    {
    circuit(clock, reset[1], OPR, SEL, CE, X, A, R1, R2, R3, R_ALU, regFLAGS, B, 0);
    PrintNexys4(0, OPR, SEL, CE, X, A, A, R1, R2, R3, regFLAGS, R_ALU, B);
    
    printf("\n\n1 - Mudar valor de OPR \t\t   8 - CLOCK \n");
    printf("2 - Mudar valor de SEL \t\t   9 - RESET \n");
    printf("3 - Mudar valor de CE \n");
    printf("\n4 - Mudar valor de X \n");
    printf("\nP - Escolher atividade \n\n");
    
    printf("SELECIONAR: ");
    scanf("%c", &menuLab4);
    printf("\n\n ");


    switch(menuLab4)
    {
        case '8': //CLOCK
        {
            press(1, clock);
            circuit(clock, reset[1], OPR, SEL, CE, X, A, R1, R2, R3, R_ALU, regFLAGS, B, 0);
            press(0, clock);
            clock[0] = 0; clock[1] = 0;
            break;
        }
        case '9': // RESET
        {
            press(1, reset);
            press(1, clock);
            circuit(clock, reset[1], OPR, SEL, CE, X, A, R1, R2, R3, R_ALU, regFLAGS, B, 0);
            press(0, clock);
            press(0, reset);
            break;
        }
        case '1': readBin(OPR, 3, "OPR\0"); break;  // Mudar valor de OPR
        case '2': readBin(SEL, 3, "SEL\0"); break; // Mudar valor de SEL
        case '3': readBin(CE, 2, "CE\0"); break; // Mudar valor de CE
        case '4': // Mudar valor de X
        case 'X':
        case 'x':
        {
            readBin(X, 8, "X\0"); break; 
        }
            
    }

    } while(menuLab4 != 'P' && menuLab4 != 'p' && menuLab4 != 'E' && menuLab4 != 'e');
}

void Lab5(int A[8], int *B,  int R1[8], int R2[8],  int R3[8], int regFLAGS[4], int R_ALU[8], int X[8], int *OPR,int *SEL, int *CE, int clock[2], int reset[2])
{
    int bPI = 8;
    int nbitsPC = 5;
    char menuLab5;
    char fname[CMAX];
    
    int nIntr = 0;
    int *intr = NULL;

    int PC[5] = {};
    int *A2jump; //guarda o endereço do jump
    int wLOAD = 0; // guarda o valor lógico que define o salto
    
    printf("\t\t  LAB 5 \n");

    int **data = rMEM(stdfname, bPI, MAX_intr, &nIntr);
    while(data == NULL)
    {
        printf("Nome do ficheiro :");
        scanf("%s", fname);
        data = rMEM(fname, bPI, MAX_intr, &nIntr);
    }

    printROM(data, nIntr);

    do
    {
        intr = ROM(PC, data, 5);
        
        OPR = intr;
        SEL = OPR + 3;
        CE = SEL + 3;
        A2jump = SEL;
                
        wLOAD = jump(OPR) * !regFLAGS[0]; // jump not zero
        
        circuit(clock, reset[1], OPR, SEL, CE, X, A, R1, R2, R3, R_ALU, regFLAGS, B, jump(OPR)); 
        PrintNexys4(1, OPR, SEL, CE, X, PC, A, R1, R2, R3, regFLAGS, R_ALU, B);

        if( BinDec(PC, 5) < nIntr ) //controla quando terminam os dados; se os dados terminarem força o reset
        {
            printf("\n\n \t\t   8 - CLOCK \n");
            printf(" \t\t   9 - RESET \n");
            printf(" \t\t   0 - REGS \n");
            printf("\n4 - Mudar valor de X \n");
            printf("\nR - Alterar  ficheiro da ROM \n");
            printf("P - Escolher atividade\n\n");
        
            printf("SELECIONAR: ");
            scanf(" %c", &menuLab5);
            printf("\n\n ");
        }
        else menuLab5 = '9';
        
        switch(menuLab5)
        {
            case '8': //CLOCK
            {
                press(1, clock);
                circuit(clock, reset[1], OPR, SEL, CE, X, A, R1, R2, R3, R_ALU, regFLAGS, B, jump(OPR));
                press(0, clock);
                Counter5b(clock, 1, 1, reset[1], wLOAD, A2jump, PC);
                clock[0] = 0; clock[1] = 0;
                break;
            }
            case '9': // RESET
            {
                press(1, reset);
                press(1, clock);
                circuit(clock, reset[1], OPR, SEL, CE, X, A, R1, R2, R3, R_ALU, regFLAGS, B, jump(OPR));
                press(0, clock);
                press(0, reset);
                Counter5b(clock, 1, 1, 1, wLOAD, A2jump, PC);
                clock[0] = 0; clock[1] = 0;
                reset[0] = 0; reset[1] = 0;
                break;
            }
            case '0' : // Ver REGS
            {
                PrintNexys4(2, OPR, SEL, CE, X, PC, A, R1, R2, R3, regFLAGS, R_ALU, B);
                getchar();
                getchar();
                break;
            }
            case '4': // Mudar valor de X
            case 'X':
            case 'x':
            {
                readBin(X, 8, "X\0");
                break; 
            }
            case 'R' : // Alterar  ficheiro da ROM
            case 'r' :
            {
                do
                {
                    printf("Nome do ficheiro :");
                    scanf("%s", fname);
                    data = rMEM(fname, bPI, MAX_intr, &nIntr);
                } while(data == NULL);
                break;
            }
        }   
    } while(menuLab5 != 'P' && menuLab5 != 'p' && menuLab5 != 'E' && menuLab5 != 'e');

    for(int i = 0; i < nIntr; i++) free(data[i]);
    free(data);

}

int lab45exe()
{
    int X[8] = {};
    int A[8] = {};
    int *B = NULL;
    int R1[8] = {};
    int R2[8] = {};
    int R3[8] = {};
    int regFLAGS[4] = {1,0,0,0};
    int R_ALU[8] = {};
    int *OPR;
    int *SEL;
    int *CE;
    int clock[2] = {};
    int reset[2] = {};
    
    char escolha;
    
    do
    {
        printf("LSDI - TL \n\n");
        
        printf("1 - Lab 4 \n");
        printf("2 - Lab 5 \n\n");

        printf("E - Sair\n\n");
        
        printf("SELECIONAR: ");
        scanf(" %c", &escolha);
        printf("\n\n ");
        
            switch(escolha)
            {
            case '1':
            case '4': Lab4(A, B, R1, R2, R3, regFLAGS, R_ALU, X, OPR, SEL, CE, clock, reset); break;
            case '2':
            case '5': Lab5(A, B, R1, R2, R3, regFLAGS, R_ALU, X, OPR, SEL, CE, clock, reset); break;
            } 
    } while(escolha != 'E' && escolha != 'e'); 
    
    return 0;
}
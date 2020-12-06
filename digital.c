#include"digital.h"

/*------------------------------conversão---------------------------------*/

int BinDec(int *Bin, int n)
{
    int dec = 0;
    int aux = 1;
    int last = n - 1;
    
    for(int i = last; i > -1; i--)
    {
        aux = aux <<1;
        dec += Bin[i]*aux;
    }
    return dec >> 1;
}

char *DecHex(int dec)
{
    char *Hex = (char*)malloc(sizeof(Hex)*2 + 1);
    
    char hexAUX[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    Hex[0] = hexAUX[dec / 16];
    Hex[1] = hexAUX[dec % 16];

    return Hex;
}

char *BinHex(int *bin, int n)
{
    int dec = BinDec(bin, n);
    return DecHex(dec);
}

int Bin_Array(int bin, int *array, int n)
{
    int aux;
    
    for (int i = 0; i < n ; i++)
    {
        aux =  bin % 10;
        
        if (aux != 0 && aux != 1) return 0;

        array[n -i - 1] = bin % 10;
        bin /= 10;
    }
    return 1;
}
/*--------------------------------circuitos combinacionais-----------------------------------*/

void dec2_4(int CE[2], int *w0, int *w1, int *w2, int *w3, int EN)
{
    *w0 = !CE[0] * !CE[1] * EN;
    *w1 = !CE[0] * CE[1] * EN;
    *w2 = CE[0] * !CE[1] * EN;
    *w3 = CE[0] * CE[1] * EN;
}

int BIT_mux2_1(int S, int Y0, int Y1)
{
   return Y1*S + Y0*(!S);
}

int *muxN_1(int *S, int **Y, int N)
{
    if(N == 2)
    {
        if(S[0]) return Y[1];
        
        return Y[0];
    }
    
    const int h = N/2;
    
    if(S[0])return muxN_1(&S[1], &Y[h], h); 
    
    return muxN_1(&S[1], Y, h);
}

int fulladder_8b(int A[8], int  B[8], int Ci, int S[8])
{   
    if (Ci)
    for(int i = 0; i < 8; i++) B[i]= B[i] ^ 1;

    for(int i = 7; i > -1; i--)
    {
        S[i] = A[i] ^ B[i] ^ Ci;
        Ci = (A[i]*B[i] + A[i]*Ci + B[i]*Ci)%2;
    }
    
    return Ci;
}
/*--------------------------------circuitos sequencias-----------------------------------*/
void press(int n, int butao[2])
{
    if (n == 1)
    {
        butao[0] = 0;
        butao[1] = 1;
    }
    else if (n == 0)
    {
        butao[1] = 0;
        butao[0] = 1;
    }
}

int clockIN(int anterior, int atual, int neg)
{
    if(anterior == neg && atual == !neg) return 1;
    else return 0;
}

void FlipFlop_D(int D, int clock[2], int negClock, int *Q)
{
    if(clockIN(clock[0], clock[1], negClock) == 1 ) *Q = D;
}

void Reg(int clock[2],int negClock, int EN, int reset, int Input[8], int Registo[8], int n)
{
    if(EN == 1)
    {
        if(reset == 1 )
        for(int i = 0; i < n ; i++) FlipFlop_D( 0, clock, negClock, Registo + i);
        
        else  for(int i = 0; i < n ; i++) 
        {
        FlipFlop_D( Input[i], clock, negClock, Registo + i);
        }
    }
}

void Counter5b(int clock[2], int negClock, int EN, int reset, int LOAD, int ABC[5], int Q[5])
{
    if(EN == 1)
    {
        if(reset == 1 && clockIN(clock[0], clock[1], negClock) == 1 )
        for(int i = 0; i < 8 ; i++) FlipFlop_D( 0, clock, negClock, Q + i);
        
        else
        {
            int w0, w1, w2, w3, w4;
            int D0, D1, D2, D3, D4;
            
            w0 = Q[4] ^ EN;
            w1 = Q[3] ^ (Q[4] * EN);
            w2 = Q[2] ^ (Q[3] * Q[4] * EN);
            w3 = Q[1] ^ (Q[2] * Q[3] * Q[4] * EN);
            w4 = Q[0] ^ (Q[1] * Q[2] * Q[3] * Q[4] * EN);

            D0 = BIT_mux2_1(LOAD, w0, ABC[4]);
            D1 = BIT_mux2_1(LOAD, w1, ABC[3]);
            D2 = BIT_mux2_1(LOAD, w2, ABC[2]);
            D3 = BIT_mux2_1(LOAD, w3, ABC[1]);
            D4 = BIT_mux2_1(LOAD, w4, ABC[0]);

            FlipFlop_D( D0, clock, negClock, &Q[4]);
            FlipFlop_D( D1, clock, negClock, &Q[3]);
            FlipFlop_D( D2, clock, negClock, &Q[2]);
            FlipFlop_D( D3, clock, negClock, &Q[1]);
            FlipFlop_D( D4, clock, negClock, &Q[0]);
        }   
    }
}

/*------------------------------ALU---------------------------------
OPR:
000 // R recebe o valor de A
001 // R recebe A - B
010 // R recebe A + B
011 // R recebe XOR(^) de A e B
100 // R recebe A >> 1 (A/2)
101 // R recebe A << 1 (A*2)
110 // R recebe A & B
111 // R recebe A | B
-------------------------------------------------------------------*/

void R_ALU(int A[8], int B[8],int OPR[3], int *Co, int *R)
{
    // OPR2 OPR1 OPR0
    _Bool OPR0= OPR[2];
    _Bool OPR1 = OPR[1];
    _Bool OPR2= OPR[0];
    
    if(OPR2)/*-----------------------------------------------*/ //1XX
    {
        if(OPR1)/*-----------------------------------------------*/ //11X
        {
            if(OPR2)/*-----------------------------------------------*/ //111
            {
                for (int i = 0; i < 8 ;i++) R[i] = A[i] | B[i];
                return;       
            } 
            /*-------------------------------------------------------*/ //110
                for (int i = 0; i < 8 ;i++) R[i] = A[i] & B[i];
                return;   
        }
        /*-------------------------------------------------------*/ //10X
        {
            if(OPR2)/*-----------------------------------------------*/ //101
            {
                for (int i = 0; i < 8 ;i++) R[i] = A[i + 1];R[7]= 0;
                return;           
            } 
            /*-------------------------------------------------------*/ //100
                for (int i = 1; i < 8 ;i++) R[i] = A[i - 1]; R[0]= 0;
                return;
        }
    }
    /*-------------------------------------------------------*/ //0XX
    {
        if(OPR1)/*-----------------------------------------------*/ //01X
        {
            if(OPR2)/*-----------------------------------------------*/ //011
            {
                for (int i = 0; i < 8 ;i++) R[i] = A[i] ^ B[i];
                return;           
            } 
            /*-------------------------------------------------------*/ //010
                *Co = fulladder_8b(A, B, 0, R);
                return; 
        }
        /*-------------------------------------------------------*/ //00X
        {
            if(OPR2)/*-----------------------------------------------*/ //001
            {
                *Co = fulladder_8b(A, B, 1, R);
                return;                            
            } 
            /*-------------------------------------------------------*/ //000
                for (int i = 0; i < 8 ;i++) R[i] = B[i];
                return;
        }
    }
}

void FLAGS_ALU(int R[8], int OPR[3], int Co, int sA, int sB, int FLAGS[4]) //flag overflow erro
{
    int flagZero = FLAGS[0];
    int flagSinal = FLAGS[1];
    int flagCarry = FLAGS[2];
    int flagOverflow = FLAGS[3];

    int AS = 0;

    AS = (OPR[1] ^ OPR[2]) * !OPR[0];

    if(R[0] == 0 && R[1] == 0 && R[2] == 0 && R[3] == 0 && R[4] == 0 && R[5] == 0 && R[6] == 0 && R[7] == 0 )
    flagZero = 1;
    
    else flagZero = 0; 

    if(R[0] == 1) flagSinal = 1;
    else flagSinal = 0;

    if(AS == 1)
    {
        if(Co == 1) flagCarry = 1;

        int XOR1 = OPR[2] ^ sB;
        int w1 = XOR1 * sA * (!R[0]);
        int w2 = (!XOR1) *(!sA) * R[0];
        if( w1 + w2 ==1 ) flagOverflow = 1;
    }

    FLAGS[0] = flagZero;
    FLAGS[1] = flagSinal;
    FLAGS[2] = flagCarry;
    FLAGS[3] = flagOverflow;
}

void ALUcompleta(int A[8], int B[8],int OPR[3], int Co, int *R, int FLAGS[4])
{
    R_ALU(A, B, OPR, &Co, R);
    FLAGS_ALU(R, OPR, Co, A[0], B[0], FLAGS);
}
/*------------------------------ROM---------------------------------*/

int *ROM(int *PC, int **data, int nbitsPC)
{   
    int N = 1;
    for(int i = 0; i < nbitsPC; i++) 
    N <<=1;

    //printf("\n\n%d\n\n", N);
    
    return muxN_1(PC, data, N);
}

int **rMEM(char *nomef, int bPI, int MAX_intr, int *nIntr)
{
    FILE *fp = fopen(nomef,"r");

    if (fp == NULL) return NULL;
    
    int **data = (int**)malloc(sizeof(int*) * MAX_intr);
    for (int i = 0; i <MAX_intr; i++) data[i] = (int*)malloc(sizeof(int) * bPI);

  
    *nIntr = 0;
    int A, B, C;
    while (fscanf(fp, "%d %d %d", &A, &B, &C) == 3)
    {
        Bin_Array(A, &data[*nIntr][0], 3);   //copia o OPR
        Bin_Array(B, &data[*nIntr][3], 3);   //copia o SEL
        Bin_Array(C, &data[*nIntr][6], 2);   //copia o CE
        *nIntr += 1;
    }

    for (int i = *nIntr; i < MAX_intr; i++) free(data[i]);

    data = realloc(data, sizeof(data)* (*nIntr));

    fclose(fp);
    return data;
}

#ifndef DIGITAL_H
#define DIGITAL_H

#include<stdio.h>
#include<stdlib.h>

/*------------------------------conversão---------------------------------*/

int BinDec(int *Bin, int n);

char *DecHex(int dec);

char *BinHex(int *bin, int n);

int Bin_Array(int bin, int *array, int n);
/*--------------------------------circuitos combinacionais-----------------------------------*/

void dec2_4(int CE[2], int *w0, int *w1, int *w2, int *w3, int EN);

int BIT_mux2_1(int S, int Y0, int Y1);

int *muxN_1(int *S, int **Y, int N);

int fulladder_8b(int A[8], int  B[8], int Ci, int S[8]);

#define jump(OPR) OPR[2]*!OPR[1]*OPR[0]
/*--------------------------------circuitos sequencias-----------------------------------*/
void press(int n, int butao[2]);

int clockIN(int anterior, int atual, int neg);

void FlipFlop_D(int D, int clock[2], int negClock, int *Q);

void Reg(int clock[2],int negClock, int EN, int reset, int Input[8], int Registo[8], int n);

void Counter5b(int clock[2], int negClock, int EN, int reset, int LOAD, int ABC[5], int Q[5]);

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

void R_ALU(int A[8], int B[8],int OPR[3], int *Co, int *R);

void FLAGS_ALU(int R[8], int OPR[3], int Co, int sA, int sB, int FLAGS[4]); //flag overflow erro

void ALUcompleta(int A[8], int B[8],int OPR[3], int Co, int *R, int FLAGS[4]);
/*------------------------------ROM---------------------------------*/

int *ROM(int *PC, int **data, int nbitsPC);

int **rMEM(char *nomef, int bPI, int MAX_intr, int *nIntr);

#endif
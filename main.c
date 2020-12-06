#include"lab45.h"
#include"digital.h"

/*--------------------Instruções-------------------------- 

OPR:
000 // R recebe o valor de A
001 // R recebe A - B
010 // R recebe A + B
011 // R recebe XOR(^) de A e B
100 // R recebe A >> 1 (A/2)
101 // R recebe A << 1 (A*2)
110 // R recebe A & B
111 // R recebe A | B

SEL
000 // O operando B é A
001 // O operando B é R1
010 // O operando B é R2
011 // O operando B é R3
100 // O operando B é X
101 // O operando B é 1
110 // O operando B é 15
111 // O operando B é o Carry

CE
00 // Guardar em A
01 Guardar em R1
10 //Guardar em R2
11 //Guardar em R3

*/


int main()
{
    lab45exe();
    
    return 0;
}
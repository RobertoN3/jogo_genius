// teste

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int operacao(int a); //função que gera a operação, vai retornar de 1 a 4 (+-*/)

int numerosomasub(int b); //função que gera o numero de 1 a 99 para a + e -

int numeromultB20(int d); //função que gera numeros faceis de multiplicar (1 a 20)

int numeromultB50(int d); //função que gera numeros faceis de multiplicar (20 a 50)

int numeromultB100(int d); //função que gera numeros faceis de multiplicar (50 a 100)

int numeromultA(int nivel, int rand); //função que retorna um numero de 1 a 10, depende do nivel




int main(){

    int a;
    scanf("%d", &a);
}



int operacao(int a){
if (a%4 == 1) return 1; // 1 vai ser convertido em 'if' para soma
if (a%4 == 2) return 2; // 2 para -
if (a%4 == 3) return 3; // 3 para *
if (a%4 == 0) return 4; // 4 para /
}


int numerosomasub(int b){

int c; //numero de 0 a 99
c = b%99 + 1; //0 a 98 --> 1 a 99
return c;
}


int numeromultB20(int d){

int e;
e = d%20 + 1; // numeros de 0 a 19 --> 1 a 20 

return e;
}


int numeromultB50(int d){

int e;
e = d%30 + 21; // numeros de 0 a 29 --> 21 a 50

return e;
}


int numeromultB100(int d){

int e;
e = d%50 + 51; // numeros de 0 a 49 --> 51 a 100

return e;
}

int numeromultA(int nivel, int rand){ //nivel de 1 a 4 (+ 5) 5 = aleatorio
if (nivel == 1){
    if (rand%3 == 0) return 1;
    if (rand%3 == 1) return 2;
    if (rand%3 == 2) return 10;
}

if (nivel == 2){
    if (rand%2 == 0) return 4;
    if (rand%2 == 1) return 5;
}

if (nivel == 3){
    if (rand%4 == 0) return 3;
    if (rand%4 == 1) return 6;
    if (rand%4 == 2) return 8;
    if (rand%4 == 3) return 9;
}

if (nivel == 4) return 7;

int x;

if (nivel == 5){
x = rand%10 + 1; //0 a 9 --> 1 a 10
return x;
}
}
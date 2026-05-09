#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int operacao(int a); //função que gera a operação, vai retornar de 1 a 4 (+-*/)

int numerosomasub99(int b); //função que gera o numero de 1 a 99 para a + e -

int numerosomasub999(int b); //função que gera o numero de 100 a 999 para a + e -

int numeromultB11_20(int d); //função que gera numeros faceis de multiplicar (11 a 20)

int numeromultB21_50(int d); //função que gera numeros medios de multiplicar (21 a 50)

int numeromultB51_100(int d); //função que gera numeros dificeis de multiplicar (50 a 100)

int numeromultA(int nivel, int rand); //função que retorna um numero de 1 a 10, depende do nivel




int main(){

    bool jogo = true;
    int dificuldade = 0;
    int nivel = 1;

    int seed;
    scanf("%d", &seed);

    srand(seed);

    int opera;

    while (jogo){

    opera = operacao(rand());

    if (opera == 1){
        if (dificuldade < 5){
            int a = numerosomasub99(rand());
            int b = numerosomasub99(rand());
            printf("%d + %d = ?\n", a, b);
            int resposta;
            scanf("%d", &resposta);
            if (resposta == a + b){
                printf("acertou\n");
                dificuldade++;
            }
            else{
                printf("game over\n%d + %d = %d\n", a, b, a + b);
                jogo = false;
            }
        }

        else if (5 <= dificuldade && dificuldade < 10){
            int a = numerosomasub999(rand());
            int b = numerosomasub99(rand());
            printf("%d + %d = ?\n", a, b);
            int resposta;
            scanf("%d", &resposta);
            if (resposta == a + b){
                printf("acertou\n");
                dificuldade++;
            }
            else{
                printf("game over\n%d + %d = %d\n", a, b, a + b);
                jogo = false;
            }
        }

        else if (dificuldade >= 10){
            int a = numerosomasub999(rand());
            int b = numerosomasub999(rand());
            printf("%d + %d = ?\n", a, b);
            int resposta;
            scanf("%d", &resposta);
            if (resposta == a + b){
                printf("acertou\n");
                dificuldade++;
            }
            else{
                printf("game over\n%d + %d = %d\n", a, b, a + b);
                jogo = false;
            }
        }
    }

    if (opera == 2){ // !!!tem q deixar sempre o maior numero sendo subtraido// n fiz -_-
                    // pq se n vai ter q adicionar um botao de negativo no bagulho
        if (dificuldade < 5){
            int a = numerosomasub99(rand());
            int b = numerosomasub99(rand());
            printf("%d - %d = ?\n", a, b);
            int resposta;
            scanf("%d", &resposta);
            if (resposta == a - b){
                printf("acertou\n");
                dificuldade++;
            }
            else{
                printf("game over\n%d - %d = %d\n", a, b, a - b);
                jogo = false;
            }
        }

        else if (5 <= dificuldade && dificuldade < 10){
            int a = numerosomasub999(rand());
            int b = numerosomasub99(rand());
            printf("%d - %d = ?\n", a, b);
            int resposta;
            scanf("%d", &resposta);
            if (resposta == a - b){
                printf("acertou\n");
                dificuldade++;
            }
            else{
                printf("game over\n%d - %d = %d\n", a, b, a - b);
                jogo = false;
            }
        }

        else if (dificuldade >= 10){
            int a = numerosomasub999(rand());
            int b = numerosomasub999(rand());
            printf("%d - %d = ?\n", a, b);
            int resposta;
            scanf("%d", &resposta);
            if (resposta == a - b){
                printf("acertou\n");
                dificuldade++;
            }
            else{
                printf("game over\n%d - %d = %d\n", a, b, a - b);
                jogo = false;
            }
        }
    }
        
    
    if (opera == 3){ //da pra resumir esse if com uma funcao, mas faz qm quiser

        if (nivel == 1){
            int a = numeromultA(1, rand()); //multiplicacao de 1,2 ou 10 por 11-20
            int b = numeromult11_20(rand());
            printf("%d * %d = ?\n", a, b);
            int resposta;
            scanf("%d", &resposta);
            if (resposta == a * b){
                printf("acertou\n");
                nivel++;
            }
            else{
                printf("game over\n%d * %d = %d\n", a, b, a * b);
                jogo = false;
            }
        }

        else if (nivel == 2 || nivel == 3){
            int a = numeromultA(1, rand()); //multiplicacao de 1,2 ou 10 por 21-50
            int b = numeromultB21_50(rand());
            printf("%d * %d = ?\n", a, b);
            int resposta;
            scanf("%d", &resposta);
            if (resposta == a * b){
                printf("acertou\n");
                nivel++;
            }
            else{
                printf("game over\n%d * %d = %d\n", a, b, a * b);
                jogo = false;
            }
        }

        else if (nivel == 4 || nivel == 5){
            int a = numeromultA(1, rand()); //multiplicacao de 1,2 ou 10 por 51-100
            int b = numeromult51_100(rand());
            printf("%d * %d = ?\n", a, b);
            int resposta;
            scanf("%d", &resposta);
            if (resposta == a * b){
                printf("acertou\n");
                nivel++;
            }
            else{
                printf("game over\n%d * %d = %d\n", a, b, a * b);
                jogo = false;
            }
        }

        else if (nivel == 6 || nivel == 7){
            int a = numeromultA(2, rand()); //multiplicacao de 4 ou 5 por 11-20
            int b = numeromultB11_20(rand());
            printf("%d * %d = ?\n", a, b);
            int resposta;
            scanf("%d", &resposta);
            if (resposta == a * b){
                printf("acertou\n");
                nivel++;
            }
            else{
                printf("game over\n%d * %d = %d\n", a, b, a * b);
                jogo = false;
            }
        }

        else if (nivel == 8){
            int a = numeromultA(2, rand()); //multiplicacao de 4 ou 5 por 21-50
            int b = numeromultB21_50(rand());
            printf("%d * %d = ?\n", a, b);
            int resposta;
            scanf("%d", &resposta);
            if (resposta == a * b){
                printf("acertou\n");
                nivel++;
            }
            else{
                printf("game over\n%d * %d = %d\n", a, b, a * b);
                jogo = false;
            }
        }

        else if (nivel == 9 || nivel == 10){
            int a = numeromultA(3, rand()); //multiplicacao de 3,6,8,9 por 11-20
            int b = numeromultB11_20(rand());
            printf("%d * %d = ?\n", a, b);
            int resposta;
            scanf("%d", &resposta);
            if (resposta == a * b){
                printf("acertou\n");
                nivel++;
            }
            else{
                printf("game over\n%d * %d = %d\n", a, b, a * b);
                jogo = false;
            }
        }

        else if (nivel == 11 || nivel == 12){
            int a = numeromultA(2, rand()); //multiplicacao de 4 ou 5 por 51-100
            int b = numeromult51_100(rand());
            printf("%d * %d = ?\n", a, b);
            int resposta;
            scanf("%d", &resposta);
            if (resposta == a * b){
                printf("acertou\n");
                nivel++;
            }
            else{
                printf("game over\n%d * %d = %d\n", a, b, a * b);
                jogo = false;
            }
        }

        else if (nivel == 13){
            int a = numeromultA(4, rand()); //multiplicacao de 7 por 11-20
            int b = numeromultB11_20(rand());
            printf("%d * %d = ?\n", a, b);
            int resposta;
            scanf("%d", &resposta);
            if (resposta == a * b){
                printf("acertou\n");
                nivel++;
            }
            else{
                printf("game over\n%d * %d = %d\n", a, b, a * b);
                jogo = false;
            }
        }        

        else if (nivel == 14){
            int a = numeromultA(3, rand()); //multiplicacao de 3,6,8 ou 9 por 21-50
            int b = numeromultB21_50(rand());
            printf("%d * %d = ?\n", a, b);
            int resposta;
            scanf("%d", &resposta);
            if (resposta == a * b){
                printf("acertou\n");
                nivel++;
            }
            else{
                printf("game over\n%d * %d = %d\n", a, b, a * b);
                jogo = false;
            }
        }

        else if (nivel == 15){
            int a = numeromultA(4, rand()); //multiplicacao de 7 por 21-50
            int b = numeromultB21_50(rand());
            printf("%d * %d = ?\n", a, b);
            int resposta;
            scanf("%d", &resposta);
            if (resposta == a * b){
                printf("acertou\n");
                nivel++;
            }
            else{
                printf("game over\n%d * %d = %d\n", a, b, a * b);
                jogo = false;
            }
        }       

        else if (nivel == 16 || nivel == 17){
            int a = numeromultA(3, rand()); //multiplicacao de 3,6,8 ou 9 por 50-100
            int b = numeromultB100(rand());
            printf("%d * %d = ?\n", a, b);
            int resposta;
            scanf("%d", &resposta);
            if (resposta == a * b){
                printf("acertou\n");
                nivel++;
            }
            else{
                printf("game over\n%d * %d = %d\n", a, b, a * b);
                jogo = false;
            }
        }

        else if (nivel == 18){
            int a = numeromultA(4, rand()); //multiplicacao de 7 por 50-100
            int b = numeromultB100(rand());
            printf("%d * %d = ?\n", a, b);
            int resposta;
            scanf("%d", &resposta);
            if (resposta == a * b){
                printf("acertou\n");
                nivel++;
            }
            else{
                printf("game over\n%d * %d = %d\n", a, b, a * b);
                jogo = false;
            }
        }
    } // <-- esse é o fim do if da multiplicacao


    } 
    }




int operacao(int a){
if (a%4 == 1) return 1; // 1 vai ser convertido em 'if' para soma
if (a%4 == 2) return 2; // 2 para -
if (a%4 == 3) return 3; // 3 para *
if (a%4 == 0) return 4; // 4 para /
}


int numerosomasub99(int b){

int c; //numero de 0 a 99
c = b%99 + 1; //0 a 98 --> 1 a 99
return c;
}

int numerosomasub999(int b){

int c;
c = b%900 + 100; //0 a 899 --> 100 a 999
return c;
}

int numeromultB11_20(int d){

int e;
e = d%10 + 11; // numeros de 0 a 9 --> 11 a 20

return e;
}


int numeromultB21_50(int d){

int e;
e = d%30 + 21; // numeros de 0 a 29 --> 21 a 50

return e;
}


int numeromultB51_100(int d){

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
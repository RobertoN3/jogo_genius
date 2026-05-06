#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


int main(){

bool jogo = true;
int seed;
int botão, sequência=0;
int leitura, pontos =0;

printf("digite seed");
scanf("%d", seed);
srand(seed);

while(jogo){

    botão = (rand()%4)+1;

    sequência = sequência*10 + botão;

    printf("%d", sequência);
    scanf("%d", leitura);
    
    if (leitura == sequência){
        printf("acertou");
        pontos ++;
    }

    else{
    jogo = false;
    printf("game over\n pontuacao = %d", pontos);
    }
    
    
}


}
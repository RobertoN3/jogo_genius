// digitada uma seed, vai-se gerando uma sequencia com digitos de 1 a 4 ex: 134213
// um digito de cada vez, por ex: 134213 --> 1342134 
// e o jogador no caso precisa digitar a série inteira, porém só é mostrado o ultimo digito
// adicionado na sequencia, no caso anterior, o numero 4

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

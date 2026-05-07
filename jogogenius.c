// digitada uma seed, vai-se gerando uma sequencia com digitos de 1 a 4 ex: 134213
// um digito de cada vez, por ex: 134213 --> 1342134 
// e o jogador no caso precisa digitar a série inteira, porém só é mostrado o ultimo digito
// adicionado na sequencia, no caso anterior, o numero 4

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define NUMERO_DE_ENTRADAS 4

int main(){

bool jogo = true;
int seed; //a seed pode ser gerada no arduino com randomSeed
int botão; // botão é um unico digito que será adicionado na sequencia
int sequência=0; //sequencia salvará o que ja foi mostrado para comparar com o input do jogador
int leitura; // leitura é o que o player insere, e q é comparada com sequencia
int pontos = 0;

printf("digite a seed: ");
scanf("%d", &seed);
srand(seed);

while(jogo){

    botão = (rand()%NUMERO_DE_ENTRADAS)+1;

    sequência = sequência*10 + botão;

    printf("%d\n", botão);
    scanf("%d", &leitura);
    
    if (leitura == sequência){
        printf("acertou\n");
        pontos ++;
    }

    else{
    jogo = false;
    printf("game over\npontuacao = %d", pontos);
    }
    
    
}


}

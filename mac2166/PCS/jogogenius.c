// digitada uma seed, vai-se gerando uma sequencia com digitos de 1 a 4 ex: 134213
// um digito de cada vez, por ex: 134213 --> 1342134 
// e o jogador no caso precisa digitar a série inteira, porém só é mostrado o ultimo digito
// adicionado na sequencia, no caso anterior, o numero 4

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define NUMERO_DE_ENTRADAS 9 //deve ser entre 1 e 9, pois a sequencia é salva como um numero inteiro, e o numero de digitos é limitado a 10 (int tem 10 digitos), e o ultimo digito é reservado para o numero do botão, ou seja, o numero de entradas é limitado a 9

int main(){

bool jogo = true;
int seed; //a seed pode ser gerada no arduino com randomSeed
int botão; // botão é um unico digito que será adicionado na sequencia
int sequência[100]; //sequencia salvará o que ja foi mostrado para comparar com o input do jogador
int ordem = 0; // ordem é o numero de digitos da sequencia, ou seja, a quantidade de vezes que o jogador acertou, e é usado para comparar com a leitura do jogador, pois a leitura deve ter o mesmo numero de digitos da sequencia para ser comparada
int leitura; // leitura é o que o player insere, e q é comparada com sequencia
int pontos = 0;

printf("digite a seed: ");
scanf("%d", &seed);
srand(seed);

while(jogo){

    botão = (rand()%NUMERO_DE_ENTRADAS)+1;

    sequência[ordem] = botão;

    ordem ++;

    for (int i = 0; i < ordem; i++){
        printf("%d ", sequência[i]);
    }

    int j = 0;

    do{
        for (int i = 0; i < ordem; i++){
            scanf("%d", &leitura);

            if (leitura != sequência[j]){
                printf("game over\npontuacao = %d\n", pontos);
                jogo = false;
                break;
            }

            j++;
        }
        pontos++;
    }while(leitura == sequência[j] && j < ordem && jogo);
    
    
}
}
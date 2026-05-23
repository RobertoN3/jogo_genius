// Genius Game - Versão Arduino com 4 Pushbuttons e 4 LEDs
// Conecte os botões aos pinos digitais: 2, 3, 4, 5
// Conecte os LEDs aos pinos: 6, 7, 8, 9

// Definição dos pinos
const int botoes[] = {2, 3, 4, 5};
const int leds[] = {6, 7, 8, 9};
const int numPinos = 4;

// Variáveis de controle do jogo
int sequencia[100]; // Armazena a sequência do jogo
int nivel = 0;
int passo = 0;
bool jogoAtivo = false;

void setup() {
  for (int i = 0; i < numPinos; i++) {
    pinMode(botoes[i], INPUT_PULLUP); // Botões com resistor interno
    pinMode(leds[i], OUTPUT);
  }
  randomSeed(analogRead(0)); // Gera uma semente aleatória
}

void loop() {
  if (!jogoAtivo) {
    aguardarInicio();
  } else {
    proximaRodada();
    reproduzirSequencia();
    verificarEntradaUsuario();
  }
}

// Pisca todos os LEDs 3 vezes em caso de erro
void sinalizarErro() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < numPinos; j++) digitalWrite(leds[j], HIGH);
    delay(300);
    for (int j = 0; j < numPinos; j++) digitalWrite(leds[j], LOW);
    delay(300);
  }
  nivel = 0;
  jogoAtivo = false;
}

// Aguarda o usuário pressionar qualquer botão para começar
void aguardarInicio() {
  for (int i = 0; i < numPinos; i++) {
    if (digitalRead(botoes[i]) == LOW) {
      delay(1000); // Debounce
      jogoAtivo = true;
      nivel = 0;
      break;
    }
  }
}

// Adiciona um novo termo (1 a 4) à sequência
void proximaRodada() {
  sequencia[nivel] = random(0, 4); // Sorteia índice de 0 a 3
  nivel++;
}

// Mostra a sequência atual para o jogador
void reproduzirSequencia() {
  for (int i = 0; i < nivel; i++) {
    int ledPino = leds[sequencia[i]];
    digitalWrite(ledPino, HIGH);
    delay(500);
    digitalWrite(ledPino, LOW);
    delay(200);
  }
}

// Lógica para capturar e validar os botões apertados
void verificarEntradaUsuario() {
  for (int i = 0; i < nivel; i++) {
    bool jogadaCorreta = false;
    
    // Aguarda o jogador pressionar um botão
    while (!jogadaCorreta) {
      for (int b = 0; b < numPinos; b++) {
        if (digitalRead(botoes[b]) == LOW) {
          digitalWrite(leds[b], HIGH); // Feedback visual
          delay(300);
          digitalWrite(leds[b], LOW);
          
          if (b == sequencia[i]) {
            jogadaCorreta = true;
          } else {
            sinalizarErro();
            return; // Sai da função para reiniciar o jogo
          }
        }
      }
    }
  }
  delay(500); // Pequena pausa entre níveis
}

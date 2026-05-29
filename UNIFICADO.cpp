#include <LiquidCrystal.h>

// --- Configuração do LCD ---
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// --- Configuração dos Botões Gerais ---
const int pinosBotoes[11] = {A0, A1, A2, A3, A4, A5, 6, 7, 8, 9, 10};

// --- Configuração dos LEDs do Genius (Jogo 2) ---
const int ledsGenius[] = {13, A6, 0, 1}; 
const int numLedsGenius = 4;

// --- Variáveis de Controle do Menu Piscante ---
int telaAtual = 0; // 0 = Menu Principal, 1 = Jogo Mat, 2 = Jogo Genius, 3 = Teste Reação
bool jogoAtivo = true;

unsigned long tempoFaseMenu = 0;
int estadoAbaMenu = 0; // 0 = Mostra Jogo 1e2, 1 = Apagado 1, 2 = Mostra Jogo 3, 3 = Apagado 2

// --- Variáveis Jogo 1 (Matemática) ---
int dificuldade = 0; 
int nivelMat = 1;       
int totalPerguntas = 0;
int totalAcertos = 0;

// --- Variáveis Jogo 2 (Genius) ---
int sequenciaGenius[100]; 
int nivelGenius = 0;
// Mapeamento pedido: Botões 1, 3, 7 e 9
const int botoesValidosGenius[] = {1, 3, 7, 9}; 

// --- Protótipos das Funções ---
int lerBotao();
int obterRespostaUsuario();
void gerenciarMenuPiscante();
void rodarJogoMatematica();
void rodarJogoGenius();
void rodarJogoReacao();
bool processa_pergunta(int a, int b, char operador, int resultado_correto, int *contador_progresso);
void telaFimDeJogoMatematica();

// Auxiliares do Genius
void proximaRodadaGenius();
void reproduzirSequenciaGenius();
bool verificarEntradaUsuarioGenius();
void sinalizarErroGenius();

// Auxiliares da Matemática
int operacao(long a);
int numerosomasub99(long b);
int numerosomasub999(long b);
int numeromultB11_20(long d);
int numeromultB21_50(long d);
int numeromultB51_100(long d);
int numeromultA(int nivel, long rand_val);
void troca_se_menor(int *a, int *b);

void setup() {
  lcd.begin(16, 2);
  
  for (int i = 0; i < 11; i++) {
    pinMode(pinosBotoes[i], INPUT_PULLUP);
  }

  for (int i = 0; i < numLedsGenius; i++) {
    pinMode(ledsGenius[i], OUTPUT);
    digitalWrite(ledsGenius[i], LOW);
  }

  randomSeed(analogRead(A7)); 

  lcd.print("Multijogos");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");
  delay(1500);
  lcd.clear();
  tempoFaseMenu = millis();
}

void loop() {
  if (telaAtual == 0) {
    gerenciarMenuPiscante();
  } 
  else if (telaAtual == 1) {
    rodarJogoMatematica();
  } 
  else if (telaAtual == 2) {
    rodarJogoGenius();
  }
  else if (telaAtual == 3) {
    rodarJogoReacao();
  }
}

// --- GERENCIADOR DO MENU PRINCIPAL (NÃO-BLOQUEANTE) ---
void gerenciarMenuPiscante() {
  unsigned long tempoAtual = millis();

  // Máquina de estados baseada no tempo para alternar as telas do menu
  switch (estadoAbaMenu) {
    case 0: // Mostra Jogo 1 e 2 por 2000ms (2s)
      lcd.setCursor(0, 0);
      lcd.print("1-Matematica    ");
      lcd.setCursor(0, 1);
      lcd.print("2-Genius Game   ");
      if (tempoAtual - tempoFaseMenu >= 2000) {
        lcd.clear();
        estadoAbaMenu = 1;
        tempoFaseMenu = tempoAtual;
      }
      break;

    case 1: // Apagado por 300ms (0.3s) antes do jogo 3
      if (tempoAtual - tempoFaseMenu >= 300) {
        estadoAbaMenu = 2;
        tempoFaseMenu = tempoAtual;
      }
      break;

    case 2: // Mostra Jogo 3 por 2000ms (2s)
      lcd.setCursor(0, 0);
      lcd.print("3-Teste Reacao  ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      if (tempoAtual - tempoFaseMenu >= 2000) {
        lcd.clear();
        estadoAbaMenu = 3;
        tempoFaseMenu = tempoAtual;
      }
      break;

    case 3: // Apagado por 300ms (0.3s) antes de voltar pro 1 e 2
      if (tempoAtual - tempoFaseMenu >= 300) {
        estadoAbaMenu = 0;
        tempoFaseMenu = tempoAtual;
      }
      break;
  }

  // Verifica continuamente se o usuário escolheu algum jogo
  int selecao = lerBotao();

  if (selecao == 1) {
    dificuldade = 0; nivelMat = 1; totalPerguntas = 0; totalAcertos = 0; juegoAtivo = true;
    lcd.clear(); lcd.print("Jogo Matematica"); delay(1000);
    telaAtual = 1;
  } 
  else if (selecao == 2) {
    nivelGenius = 0; jogoAtivo = true;
    lcd.clear(); lcd.print("Jogo Genius"); lcd.setCursor(0, 1); lcd.print("Use: 1, 3, 7 e 9"); delay(2000);
    telaAtual = 2;
  }
  else if (selecao == 3) {
    jogoAtivo = true;
    lcd.clear(); lcd.print("Teste Reacao"); lcd.setCursor(0, 1); lcd.print("Use o botao [5]"); delay(2000);
    telaAtual = 3;
  }
}

// --- JOGO 1: MATEMÁTICA ---
void rodarJogoMatematica() {
  if (jogoAtivo) {
    long r = random(0, 2147483647);
    int opera = operacao(r);
    int a, b;

    if (opera == 1) {
      if (dificuldade < 5) { a = numerosomasub99(random(0, 10000)); b = numerosomasub99(random(0, 10000)); }
      else if (dificuldade < 10) { a = numerosomasub999(random(0, 10000)); b = numerosomasub99(random(0, 10000)); }
      else { a = numerosomasub999(random(0, 10000)); b = numerosomasub999(random(0, 10000)); }
      jogoAtivo = processa_pergunta(a, b, '+', a + b, &dificuldade);
    }
    else if (opera == 2) {
      if (dificuldade < 5) { a = numerosomasub99(random(0, 10000)); b = numerosomasub99(random(0, 10000)); }
      else if (dificuldade < 10) { a = numerosomasub999(random(0, 10000)); b = numerosomasub99(random(0, 10000)); }
      else { a = numerosomasub999(random(0, 10000)); b = numerosomasub999(random(0, 10000)); }
      troca_se_menor(&a, &b);
      jogoAtivo = processa_pergunta(a, b, '-', a - b, &dificuldade);
    }
    else if (opera == 3) {
      switch (nivelMat) {
        case 1:           a = numeromultA(1, random(0, 10000)); b = numeromultB11_20(random(0, 10000)); break;
        case 2: case 3:   a = numeromultA(1, random(0, 10000)); b = numeromultB21_50(random(0, 10000)); break;
        case 4: case 5:   a = numeromultA(1, random(0, 10000)); b = numeromultB51_100(random(0, 10000)); break;
        case 6: case 7:   a = numeromultA(2, random(0, 10000)); b = numeromultB11_20(random(0, 10000)); break;
        case 8:           a = numeromultA(2, random(0, 10000)); b = numeromultB21_50(random(0, 10000)); break;
        case 9: case 10:  a = numeromultA(3, random(0, 10000)); b = numeromultB11_20(random(0, 10000)); break;
        case 11: case 12: a = numeromultA(2, random(0, 10000)); b = numeromultB51_100(random(0, 10000)); break;
        case 13:          a = numeromultA(4, random(0, 10000)); b = numeromultB11_20(random(0, 10000)); break;
        case 14:          a = numeromultA(3, random(0, 10000)); b = numeromultB21_50(random(0, 10000)); break;
        case 15:          a = numeromultA(4, random(0, 10000)); b = numeromultB21_50(random(0, 10000)); break;
        case 16: case 17: a = numeromultA(3, random(0, 10000)); b = numeromultB51_100(random(0, 10000)); break;
        case 18:          a = numeromultA(4, random(0, 10000)); b = numeromultB51_100(random(0, 10000)); break;
        default:          a = numeromultA(5, random(0, 10000)); b = numeromultB21_50(random(0, 10000)); break;
      }
      jogoAtivo = processa_pergunta(a, b, '*', a * b, &nivelMat);
    }
    else if (opera == 4) {
      switch (nivelMat) {
        case 1:           b = numeromultA(1, random(0, 10000)); a = b * numeromultB11_20(random(0, 10000)); break;
        case 2: case 3:   b = numeromultA(1, random(0, 10000)); a = b * numeromultB21_50(random(0, 10000)); break;
        case 4: case 5:   b = numeromultA(1, random(0, 10000)); a = b * numeromultB51_100(random(0, 10000)); break;
        case 6: case 7:   b = numeromultA(2, random(0, 10000)); a = b * numeromultB11_20(random(0, 10000)); break;
        case 8:           b = numeromultA(2, random(0, 10000)); a = b * numeromultB21_50(random(0, 10000)); break;
        case 9: case 10:  b = numeromultA(3, random(0, 10000)); a = b * numeromultB11_20(random(0, 10000)); break;
        case 11: case 12: b = numeromultA(2, random(0, 10000)); a = b * numeromultB51_100(random(0, 10000)); break;
        case 13:          b = numeromultA(4, random(0, 10000)); a = b * numeromultB11_20(random(0, 10000)); break;
        case 14:          b = numeromultA(3, random(0, 10000)); a = b * numeromultB21_50(random(0, 10000)); break;
        case 15:          b = numeromultA(4, random(0, 10000)); a = b * numeromultB21_50(random(0, 10000)); break;
        case 16: case 17: b = numeromultA(3, random(0, 10000)); a = b * numeromultB51_100(random(0, 10000)); break;
        case 18:          b = numeromultA(4, random(0, 10000)); a = b * numeromultB51_100(random(0, 10000)); break;
        default:          b = numeromultA(5, random(0, 10000)); a = b * numeromultB21_50(random(0, 10000)); break;
      }
      jogoAtivo = processa_pergunta(a, b, '/', a / b, &nivelMat);
    }
  } 
  else {
    lcd.clear();
    lcd.print("0:Sair");
    lcd.setCursor(0, 1);
    lcd.print("1-9:Continuar");

    int escolha = -1;
    while (escolha == -1 || escolha == 10) { escolha = lerBotao(); }

    if (escolha == 0) {
      telaFimDeJogoMatematica(); 
      telaAtual = 0; 
      estadoAbaMenu = 0; tempoFaseMenu = millis(); // Reseta relógio do menu
    } else {
      lcd.clear(); lcd.print("Continuando..."); delay(1500);
      jogoAtivo = true;
    }
  }
}

// --- JOGO 2: GENIUS GAME (COM BOTÕES 1, 3, 7 e 9) ---
void rodarJogoGenius() {
  if (jogoAtivo) {
    lcd.clear();
    lcd.print("Nivel Genius: "); lcd.print(nivelGenius + 1);
    delay(1000);

    proximaRodadaGenius();
    reproduzirSequenciaGenius();
    
    jogoAtivo = verificarEntradaUsuarioGenius();
    delay(500);
  } 
  else {
    sinalizarErroGenius();
    
    lcd.clear();
    lcd.print("0:Sair");
    lcd.setCursor(0, 1);
    lcd.print("1-9:Tentar Novam.");

    int escolha = -1;
    while (escolha == -1 || escolha == 10) { escolha = lerBotao(); }

    if (escolha == 0) {
      telaAtual = 0; 
      estadoAbaMenu = 0; tempoFaseMenu = millis();
    } else {
      nivelGenius = 0; jogoAtivo = true;
      lcd.clear(); lcd.print("Reiniciando..."); delay(1500);
    }
  }
}

void proximaRodadaGenius() {
  sequenciaGenius[nivelGenius] = random(0, 4); // Sorteia índice de 0 a 3
  nivelGenius++;
}

void reproduzirSequenciaGenius() {
  for (int i = 0; i < nivelGenius; i++) {
    int ledPino = ledsGenius[sequenciaGenius[i]];
    digitalWrite(ledPino, HIGH);
    delay(400);
    digitalWrite(ledPino, LOW);
    delay(150);
  }
}

bool verificarEntradaUsuarioGenius() {
  lcd.setCursor(0, 1);
  lcd.print("Sua vez!       ");

  for (int i = 0; i < nivelGenius; i++) {
    int botaoApertado = -1;
    int indiceSelecao = -1;
    
    // Aguarda o clique de um dos botões reconfigurados (1, 3, 7 ou 9)
    while (indiceSelecao == -1) {
      botaoApertado = lerBotao();
      for (int k = 0; k < 4; k++) {
        if (botaoApertado == botoesValidosGenius[k]) {
          indiceSelecao = k; // Descobre qual pino (0 a 3) corresponde ao botão físico clicado
        }
      }
    }
    
    digitalWrite(ledsGenius[indiceSelecao], HIGH);
    delay(250);
    digitalWrite(ledsGenius[indiceSelecao], LOW);

    if (indiceSelecao != sequenciaGenius[i]) {
      return false; 
    }
  }
  return true; 
}

void sinalizarErroGenius() {
  lcd.clear();
  lcd.print("Sequencia Errada!");
  lcd.setCursor(0, 1);
  lcd.print("Pontos: "); lcd.print(nivelGenius - 1);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < numLedsGenius; j++) digitalWrite(ledsGenius[j], HIGH);
    delay(250);
    for (int j = 0; j < numLedsGenius; j++) digitalWrite(ledsGenius[j], LOW);
    delay(250);
  }
}

// --- JOGO 3: TESTE DE REAÇÃO (ADAPTADO) ---
void rodarJogoReacao() {
  lcd.clear();
  lcd.print("[5]: Iniciar");
  lcd.setCursor(0, 1);
  lcd.print("[0]: Voltar");
  
  int pronto = -1;
  while(pronto != 5 && pronto != 0) {
    pronto = lerBotao();
  }
  
  if(pronto == 0) {
    telaAtual = 0;
    estadoAbaMenu = 0; tempoFaseMenu = millis();
    return;
  }
  
  lcd.clear();
  lcd.print("Aguarde o sinal...");
  
  // Limpeza preventiva: garante que cliques ansiosos anteriores não estraguem o teste
  while(lerBotao() != -1); 
  
  // Gera o tempo aleatório pedido pelo código original (entre 3 e 10 segundos)
  unsigned long tempoEsperaRandom = random(3000, 10001);
  unsigned long inicioEspera = millis();
  bool trapaça = false;
  
  // Dorme pelo tempo aleatório, mas vigia se o usuário tentou clicar antes da hora!
  while(millis() - inicioEspera < tempoEsperaRandom) {
    if(digitalRead(pinosBotoes[5]) == LOW) { // Se detectou clique no pino do botão 5 antes
      trapaça = true;
    }
  }
  
  if(trapaça) {
    lcd.clear();
    lcd.print("TRAPACA DETECTADA");
    lcd.setCursor(0, 1);
    lcd.print("Clicou cedo demais");
    delay(3000);
    return;
  }
  
  lcd.clear();
  lcd.print("!!! AGORA !!!");
  
  // Dispara o cronômetro de microssegundos de alta precisão do Arduino
  unsigned long tempoInicioCronometro = micros();
  
  // Aguarda até que o botão 5 seja pressionado
  while(digitalRead(pinosBotoes[5]) == HIGH);
  
  unsigned long tempoFimCronometro = micros();
  
  // Calcula o tempo de reação em segundos (com precisão de floats)
  double tempo_reacao = (double)(tempoFimCronometro - tempoInicioCronometro) / 1000000.0;
  
  // Remove o ruído do debounce do final do clique para não bugar leituras seguintes
  delay(50);
  while(digitalRead(pinosBotoes[5]) == LOW);
  delay(50);
  
  lcd.clear();
  lcd.print("Tempo Reacao:");
  lcd.setCursor(0, 1);
  lcd.print(tempo_reacao, 4); // Exibe com 4 casas decimais na tela
  lcd.print(" s");
  
  delay(4000); // Exibe o resultado por 4 segundos e volta para o início do jogo 3
}

// --- LEITURA DE BOTÕES ---
int lerBotao() {
  for (int i = 0; i < 11; i++) {
    if (digitalRead(pinosBotoes[i]) == LOW) { 
      delay(50); 
      while(digitalRead(pinosBotoes[i]) == LOW) { delay(10); }
      delay(50); 
      return i; 
    }
  }
  return -1;
}

int obterRespostaUsuario() {
  int valorTotal = 0;
  bool digitouAlgo = false;
  unsigned long tempoUltimoDigito = millis();
  
  while (true) {
    int botaoPressionado = lerBotao();
    
    if (botaoPressionado == 10) {
      if (digitouAlgo && valorTotal > 0) {
        valorTotal = valorTotal / 10; 
        if (valorTotal == 0) {
          digitouAlgo = false;
          lcd.setCursor(0, 1);
          lcd.print("Digite...       ");
        } else {
          lcd.setCursor(0, 1);
          lcd.print("                "); 
          lcd.setCursor(0, 1);
          lcd.print(valorTotal);
        }
        tempoUltimoDigito = millis(); 
      }
    }
    else if (botaoPressionado != -1) {
      if (!digitouAlgo) {
        digitouAlgo = true;
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
      }
      valorTotal = (valorTotal * 10) + botaoPressionado;
      lcd.print(botaoPressionado);
      tempoUltimoDigito = millis(); 
    }
    
    if (digitouAlgo && (millis() - tempoUltimoDigito > 2000)) {
      break;
    }
  }
  return valorTotal;
}

bool processa_pergunta(int a, int b, char operador, int resultado_correto, int *contador_progresso) {
  totalPerguntas++;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(a); lcd.print(" "); lcd.print(operador); lcd.print(" "); lcd.print(b); lcd.print(" = ?");
  lcd.setCursor(0, 1);
  lcd.print("Digite...");

  int resposta = obterRespostaUsuario();

  lcd.clear();
  if (resposta == resultado_correto) {
    lcd.setCursor(0, 0);
    lcd.print("Acertou!");
    totalAcertos++;
    (*contador_progresso)++;
    delay(1500);
    return true;
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Errouu!");
    lcd.setCursor(0, 1);
    lcd.print(a); lcd.print(operador); lcd.print(b); lcd.print("="); lcd.print(resultado_correto);
    delay(3000); 
    return false;
  }
}

void telaFimDeJogoMatematica() {
  lcd.clear();
  lcd.print("Jogo Encerrado!");
  int porcentagem = 0;
  if (totalPerguntas > 0) {
    porcentagem = (totalAcertos * 100) / totalPerguntas; 
  }
  lcd.setCursor(0, 1);
  lcd.print("Aproveit.: "); lcd.print(porcentagem); lcd.print("%");
  delay(4000); 
}

// --- Funções Matemáticas do Jogo 1 ---
int operacao(long a) { if (a % 4 == 1) return 1; if (a % 4 == 2) return 2; if (a % 4 == 3) return 3; return 4; }
int numerosomasub99(long b) { return (b % 99) + 1; }
int numerosomasub999(long b) { return (b % 900) + 100; }
int numeromultB11_20(long d) { return (d % 10) + 11; }
int numeromultB21_50(long d) { return (d % 30) + 21; }
int numeromultB51_100(long d) { return (d % 50) + 51; }
int numeromultA(int nivel, long rand_val) {
  if (nivel == 1) { if (rand_val % 3 == 0) return 1; if (rand_val % 3 == 1) return 2; return 10; }
  if (nivel == 2) { if (rand_val % 2 == 0) return 4; return 5; }
  if (nivel == 3) { if (rand_val % 4 == 0) return 3; if (rand_val % 4 == 1) return 6; if (rand_val % 4 == 2) return 8; return 9; }
  if (nivel == 4) return 7;
  return (rand_val % 8) + 2;
}
void troca_se_menor(int *a, int *b) { if (*a < *b) { int temp = *a; *a = *b; *b = temp; } }
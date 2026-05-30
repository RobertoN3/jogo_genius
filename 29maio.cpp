#include <LiquidCrystal.h>

// --- Configuração do LCD (Totalmente nas portas analógicas) ---
// RS=A0, E=A1, D4=A2, D5=A3, D6=A4, D7=A5
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

// --- Configuração da Matriz de Botões (Pinos 6 a 12) ---
const int pinosLinhas[4] = {6, 7, 8, 9};
const int pinosColunas[3] = {10, 11, 12};

// Mapeamento da matriz para os números lógicos do jogo
const int mapaBotoes[4][3] = {
  {0, 1, 2},
  {3, 4, 5},
  {6, 7, 8},
  {9, 10, -1} // -1 indica espaço vazio na matriz
};

// --- Configuração dos LEDs do Genius ---
// Usando as portas de 0 a 3 como solicitado
const int ledsGenius[] = {0, 1, 2, 3}; 
const int numLedsGenius = 4;
const int botoesValidosGenius[] = {1, 3, 7, 9}; 

// PINOS 4, 5 E 13 ESTÃO TOTALMENTE LIVRES E SOBRANDO!

// --- Variáveis de Controle do Menu Piscante ---
int telaAtual = 0; // 0 = Menu Principal, 1 = Jogo Mat, 2 = Jogo Genius, 3 = Teste Reação
bool jogoAtivo = true;

unsigned long tempoFaseMenu = 0;
int estadoAbaMenu = 0; 

// --- Variáveis Jogo 1 (Matemática) ---
int dificuldade = 0; 
int nivelMat = 1;       
int totalPerguntas = 0;
int totalAcertos = 0;

// --- Variáveis Jogo 2 (Genius) ---
int sequenciaGenius[100]; 
int nivelGenius = 0;

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
  
  // Configura as linhas da matriz como entrada com Pull-up
  for (int i = 0; i < 4; i++) {
    pinMode(pinosLinhas[i], INPUT_PULLUP);
  }
  // Configura as colunas da matriz como saídas em nível ALTO
  for (int i = 0; i < 3; i++) {
    pinMode(pinosColunas[i], OUTPUT);
    digitalWrite(pinosColunas[i], HIGH);
  }

  // Configura os pinos dos LEDs do Genius como Saída
  for (int i = 0; i < numLedsGenius; i++) {
    pinMode(ledsGenius[i], OUTPUT);
    digitalWrite(ledsGenius[i], LOW);
  }

  // Semente aleatória usando uma leitura analógica fantasma para gerar ruído real
  randomSeed(analogRead(A5)); 

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

// --- GERENCIADOR DO MENU PRINCIPAL ---
void gerenciarMenuPiscante() {
  unsigned long tempoAtual = millis();

  switch (estadoAbaMenu) {
    case 0: 
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

    case 1: 
      if (tempoAtual - tempoFaseMenu >= 300) {
        estadoAbaMenu = 2;
        tempoFaseMenu = tempoAtual;
      }
      break;

    case 2: 
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

    case 3: 
      if (tempoAtual - tempoFaseMenu >= 300) {
        estadoAbaMenu = 0;
        tempoFaseMenu = tempoAtual;
      }
      break;
  }

  int selecao = lerBotao();

  if (selecao == 1) {
    dificuldade = 0; nivelMat = 1; totalPerguntas = 0; totalAcertos = 0; jogoAtivo = true;
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
        case 16: case 17: b = numeromultA(3, random(0, 10000)); b = numeromultB51_100(random(0, 10000)); break;
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
    while (escolha == -1) { escolha = lerBotao(); }

    if (escolha == 0) {
      telaFimDeJogoMatematica(); 
      telaAtual = 0; 
      estadoAbaMenu = 0; tempoFaseMenu = millis(); 
    } else {
      lcd.clear(); lcd.print("Continuando..."); delay(1500);
      jogoAtivo = true;
    }
  }
}

// --- JOGO 2: GENIUS GAME ---
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
    while (escolha == -1) { escolha = lerBotao(); }

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
  sequenciaGenius[nivelGenius] = random(0, 4); 
  nivelGenius++;
}

void reproduzirSequenciaGenius() {
  for (int i = 0; i < nivelGenius; i++) {
    int pinoLed = ledsGenius[sequenciaGenius[i]];
    
    digitalWrite(pinoLed, HIGH);
    delay(500); 
    digitalWrite(pinoLed, LOW);
    delay(200);
  }
}

bool verificarEntradaUsuarioGenius() {
  lcd.setCursor(0, 1);
  lcd.print("Sua vez!       ");

  for (int i = 0; i < nivelGenius; i++) {
    int botaoApertado = -1;
    int indiceSelecao = -1;
    
    while (indiceSelecao == -1) {
      botaoApertado = lerBotao();
      for (int k = 0; k < 4; k++) {
        if (botaoApertado == botoesValidosGenius[k]) {
          indiceSelecao = k; 
        }
      }
    }
    
    int pinoLed = ledsGenius[indiceSelecao];
    digitalWrite(pinoLed, HIGH);
    delay(250);
    digitalWrite(pinoLed, LOW);

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

// --- JOGO 3: TESTE DE REAÇÃO ---
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
  
  // Limpa o buffer de cliques anteriores
  while(lerBotao() != -1); 
  
  unsigned long tempoEsperaRandom = random(3000, 10001);
  unsigned long inicioEspera = millis();
  bool trapaca = false; 
  
  while(millis() - inicioEspera < tempoEsperaRandom) {
    // Se ler o botão 5 durante a espera, aciona a trapaça
    if(lerBotao() == 5) { 
      trapaca = true;
    }
  }
  
  if(trapaca) {
    lcd.clear();
    lcd.print("TRAPACA DETECTADA");
    lcd.setCursor(0, 1);
    lcd.print("Clicou cedo demais");
    delay(3000);
    return;
  }
  
  lcd.clear();
  lcd.print("!!! AGORA !!!");
  
  unsigned long tempoInicioCronometro = micros();
  
  // Aguarda até o botão 5 ser pressionado na matriz
  while(lerBotao() != 5);
  
  unsigned long tempoFimCronometro = micros();
  
  double tempo_reacao = (double)(tempoFimCronometro - tempoInicioCronometro) / 1000000.0;
  
  lcd.clear();
  lcd.print("Tempo Reacao:");
  lcd.setCursor(0, 1);
  lcd.print(tempo_reacao, 4); 
  lcd.print(" s");
  
  delay(4000); 
}

// --- LEITURA DA MATRIZ DE BOTÕES ---
int lerBotao() {
  for (int col = 0; col < 3; col++) {
    // Ativa a coluna atual colocando ela em LOW
    digitalWrite(pinosColunas[col], LOW);
    
    for (int lin = 0; lin < 4; lin++) {
      if (digitalRead(pinosLinhas[lin]) == LOW) {
        int botaoDetectado = mapaBotoes[lin][col];
        
        if (botaoDetectado != -1) {
          delay(20); // Debounce rápido para simulação estável
          while(digitalRead(pinosLinhas[lin]) == LOW); // Aguarda o usuário soltar
          delay(20);
          
          // Desativa a coluna antes de retornar o valor
          digitalWrite(pinosColunas[col], HIGH);
          return botaoDetectado;
        }
      }
    }
    // Desativa a coluna atual voltando ela para HIGH
    digitalWrite(pinosColunas[col], HIGH);
  }
  return -1; // Retorna -1 se nenhum botão foi apertado
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
    else if (botaoPressionado != -1 && botaoPressionado != 10) {
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
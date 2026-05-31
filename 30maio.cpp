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
// Usando as portas de 2 a 5
const int ledsGenius[] = {4, 5, 2, 3}; 
const int numLedsGenius = 4;
const int botoesValidosGenius[] = {1, 3, 7, 9}; 
// LED porta 4 botao 1, LED porta 5 botao 3, etc

// --- Configuração do Buzzer (Porta 13) ---
const int pinoBuzzer = 13;

// Frequências para cada um dos 4 LEDs (Notas: Dó, Ré, Mi, Sol)
const int sonsGenius[] = {262, 294, 330, 392}; 

// PINOS 0 E 1 ESTÃO TOTALMENTE LIVRES E SOBRANDO PRO ESP32

// --- Variáveis de Controle do Menu Piscante ---
int telaAtual = 0; // 0=Menu, 1=Mat Simples, 2=Matematica 2.0, 3=Genius, 4=Reacao
bool jogoAtivo = true;

unsigned long tempoFaseMenu = 0;
int estadoAbaMenu = 0; 

// --- Variáveis Jogo 1 (Matemática Simples) ---
int dificuldade = 0; 
int nivelMat = 1;       
int totalPerguntas = 0;
int totalAcertos = 0;

// --- Variáveis Jogo 2 (Matemática 2.0 / Avançado) ---
int nivelCalculo = 1;
int totalPerguntasCalc = 0;
int totalAcertosCalc = 0;

// --- Variáveis Jogo 3 (Genius) ---
int sequenciaGenius[100]; 
int nivelGenius = 0;

// --- Protótipos das Funções ---
int lerBotao();
int obterRespostaUsuario();
void gerenciarMenuPiscante();
void rodarJogoMatematica();
void rodarJogoCalculo();
void rodarJogoGenius();
void rodarJogoReacao();
bool processa_pergunta(int a, int b, char operador, int resultado_correto, int *contador_progresso);
void telaFimDeJogoMatematica();
long potenciaInteira(int base, int exp);
void telaFimDeJogoCalculo();

// Auxiliares do Genius
void proximaRodadaGenius();
void reproduzirSequenciaGenius();
bool verificarEntradaUsuarioGenius();
void sinalizarErroGenius();

// Auxiliares da Matemática Simples
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
  
  // Configura o pino do Buzzer como Saída
  pinMode(pinoBuzzer, OUTPUT);

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

  randomSeed(analogRead(A5)); 

  // --- ANIMAÇÃO DE INICIALIZAÇÃO ---
  lcd.clear();
  lcd.print("Iniciando. ");
  delay(1000);
  
  lcd.clear();
  lcd.print("Iniciando.. ");
  delay(1000);
  
  lcd.clear();
  lcd.print("Iniciando... ");
  delay(1000);
  
  lcd.clear();
  lcd.print("Sinapse!");
  delay(1000);
  
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
    rodarJogoCalculo();
  }
  else if (telaAtual == 3) {
    rodarJogoGenius();
  }
  else if (telaAtual == 4) {
    rodarJogoReacao();
  }
}

// --- GERENCIADOR DO MENU PRINCIPAL (4 OPÇÕES) ---
void gerenciarMenuPiscante() {
  unsigned long tempoAtual = millis();

  switch (estadoAbaMenu) {
    case 0: 
      lcd.setCursor(0, 0);
      lcd.print("1-Matematica    ");
      lcd.setCursor(0, 1);
      lcd.print("2-Matematica 2.0");
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
      lcd.print("3-Genius Game   ");
      lcd.setCursor(0, 1);
      lcd.print("4-Teste Reacao  ");
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
    lcd.clear(); lcd.print("Matematica"); delay(1000);
    telaAtual = 1;
  } 
  else if (selecao == 2) {
    nivelCalculo = 1; totalPerguntasCalc = 0; totalAcertosCalc = 0; jogoAtivo = true;
    lcd.clear(); lcd.print("Matematica 2.0"); lcd.setCursor(0, 1); lcd.print("Boa Sorte!"); delay(1500);
    telaAtual = 2;
  }
  else if (selecao == 3) {
    nivelGenius = 0; jogoAtivo = true;
    lcd.clear(); lcd.print("Jogo Genius"); lcd.setCursor(0, 1); lcd.print("Use: 1, 3, 7 e 9"); delay(2000);
    telaAtual = 3;
  }
  else if (selecao == 4) {
    jogoAtivo = true;
    lcd.clear(); lcd.print("Teste Reacao"); lcd.setCursor(0, 1); lcd.print("Use o botao [5]"); delay(2000);
    telaAtual = 4;
  }
}

// --- JOGO 1: MATEMÁTICA SIMPLES ---
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

// --- JOGO 2: MATEMATICA 2.0 (AVANÇADA) ---
void rodarJogoCalculo() {
  if (jogoAtivo) {
    totalPerguntasCalc++;
    lcd.clear();
    int sorteio = random(1, 5);
    long respostaEsperada = 0;

    int tetoRaizLog = 100 * nivelCalculo;
    if (tetoRaizLog > 1000) tetoRaizLog = 1000;

    if (sorteio == 1) { // --- RAIZ QUADRADA ---
      int nraiz = random(1, tetoRaizLog);
      double raiz = sqrt(nraiz);
      while ((int)raiz * (int)raiz != nraiz) { 
        nraiz = random(1, tetoRaizLog); 
        raiz = sqrt(nraiz); 
      }
      lcd.setCursor(0, 0); lcd.print("Raiz de "); lcd.print(nraiz);
      lcd.setCursor(0, 1); lcd.print("Digite: ");
      respostaEsperada = (long)raiz;
    }
    else if (sorteio == 2) { // --- POTENCIAÇÃO ---
      int npotencia = random(1, 10 + (nivelCalculo * 2));
      if (npotencia > 30) npotencia = 30;
      int expoente  = (npotencia <= 10) ? random(1, 4) : random(1, 3);
      
      lcd.setCursor(0, 0); lcd.print(npotencia); lcd.print("^"); lcd.print(expoente); lcd.print(" = ?");
      lcd.setCursor(0, 1); lcd.print("Digite: ");
      respostaEsperada = potenciaInteira(npotencia, expoente);
    }
    else if (sorteio == 3) { // --- LOGARITMO ---
      int opcoes[4] = {2, 3, 5, 10};
      int base = opcoes[random(0, 4)];
      int logaritimando = random(1, tetoRaizLog);
      double logaritimo = log((double)logaritimando) / log((double)base);
      while (abs(logaritimo - round(logaritimo)) > 0.0001) {
        logaritimando = random(1, tetoRaizLog);
        logaritimo    = log((double)logaritimando) / log((double)base);
      }
      lcd.setCursor(0, 0); lcd.print("Log"); lcd.print(base); lcd.print("("); lcd.print(logaritimando); lcd.print(") = ?");
      lcd.setCursor(0, 1); lcd.print("Digite: ");
      respostaEsperada = (long)round(logaritimo);
    }
    else { // --- FATORIAL ---
      int nfatorial = random(1, 7); 
      long fatorial = 1;
      for (int i = 2; i <= nfatorial; i++) fatorial *= i;
      lcd.setCursor(0, 0); lcd.print(nfatorial); lcd.print("! = ?");
      lcd.setCursor(0, 1); lcd.print("Digite: ");
      respostaEsperada = fatorial;
    }

    int respostaUsuario = obterRespostaUsuario();

    lcd.clear();
    if (respostaUsuario == respostaEsperada) {
      lcd.print("Acertou!");
      totalAcertosCalc++;
      nivelCalculo++; 
      delay(1500);
    } else {
      lcd.print("Errouu!");
      lcd.setCursor(0, 1);
      lcd.print("Resp correta: "); lcd.print(respostaEsperada);
      delay(3000);
      jogoAtivo = false; 
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
      telaFimDeJogoCalculo(); 
      telaAtual = 0; 
      estadoAbaMenu = 0; tempoFaseMenu = millis(); 
    } else {
      lcd.clear(); lcd.print("Continuando..."); delay(1500);
      jogoAtivo = true;
    }
  }
}

// --- TELA FIM DE JOGO: MATEMATICA 2.0 ---
void telaFimDeJogoCalculo() {
  lcd.clear();
  double porcentagem = (totalPerguntasCalc > 0) ? (100.0 * totalAcertosCalc) / totalPerguntasCalc : 0;
  
  lcd.setCursor(0, 0);
  if      (porcentagem >= 70) lcd.print("  Parabens!    ");
  else if (porcentagem >= 50) lcd.print("  Quase la!    ");
  else                        lcd.print("   Melhore!    ");
  
  lcd.setCursor(0, 1);
  lcd.print("Aproveit.: "); lcd.print((int)porcentagem); lcd.print("%");
  delay(4000);
}

long potenciaInteira(int base, int exp) {
  long resultado = 1;
  for (int i = 0; i < exp; i++) resultado *= base;
  return resultado;
}

// --- JOGO 3: GENIUS GAME ---
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
    int indiceLed = sequenciaGenius[i];
    int pinoLed = ledsGenius[indiceLed];
    int frequenciaSom = sonsGenius[indiceLed];
    
    digitalWrite(pinoLed, HIGH);
    tone(pinoBuzzer, frequenciaSom); // Toca o som respectivo do LED
    delay(500); 
    
    digitalWrite(pinoLed, LOW);
    noTone(pinoBuzzer);             // Desliga o som
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
    int frequenciaSom = sonsGenius[indiceSelecao];
    
    digitalWrite(pinoLed, HIGH);
    tone(pinoBuzzer, frequenciaSom); // Toca o som quando o usuario pressiona o botao
    delay(250);
    
    digitalWrite(pinoLed, LOW);
    noTone(pinoBuzzer);

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

  // Som grave de erro prolongado combinado com os LEDs piscando
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < numLedsGenius; j++) digitalWrite(ledsGenius[j], HIGH);
    tone(pinoBuzzer, 131); // Nota bem grave de erro
    delay(250);
    
    for (int j = 0; j < numLedsGenius; j++) digitalWrite(ledsGenius[j], LOW);
    noTone(pinoBuzzer);
    delay(250);
  }
}

// --- JOGO 4: TESTE DE REAÇÃO ---
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
  
  while(lerBotao() != -1); 
  
  unsigned long tempoEsperaRandom = random(3000, 10001);
  unsigned long inicioEspera = millis();
  bool trapaca = false; 
  
  while(millis() - inicioEspera < tempoEsperaRandom) {
    if(lerBotao() == 5) { 
      trapaca = true;
    }
  }
  
  if(trapaca) {
    lcd.clear();
    lcd.print("Queimou a");
    lcd.setCursor(0, 1);
    lcd.print("largada!");
    delay(3000);
    return;
  }
  
  lcd.clear();
  lcd.print("!!! AGORA !!!");
  
  unsigned long tempoInicioCronometro = micros();
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
    digitalWrite(pinosColunas[col], LOW);
    for (int lin = 0; lin < 4; lin++) {
      if (digitalRead(pinosLinhas[lin]) == LOW) {
        int botaoDetectado = mapaBotoes[lin][col];
        if (botaoDetectado != -1) {
          delay(20); 
          while(digitalRead(pinosLinhas[lin]) == LOW); 
          delay(20);
          digitalWrite(pinosColunas[col], HIGH);
          return botaoDetectado;
        }
      }
    }
    digitalWrite(pinosColunas[col], HIGH);
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
          lcd.setCursor(8, 1);
          lcd.print("        "); 
          lcd.setCursor(8, 1);
          lcd.print(valorTotal);
        }
        tempoUltimoDigito = millis(); 
      }
    }
    else if (botaoPressionado != -1 && botaoPressionado != 10) {
      if (!digitouAlgo) {
        digitouAlgo = true;
        lcd.setCursor(8, 1);
        lcd.print("        ");
        lcd.setCursor(8, 1);
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
  lcd.print("Digite: ");

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

// --- TELA FIM DE JOGO: MATEMATICA SIMPLES ---
void telaFimDeJogoMatematica() {
  lcd.clear();
  int porcentagem = 0;
  if (totalPerguntas > 0) {
    porcentagem = (totalAcertos * 100) / totalPerguntas; 
  }
  
  lcd.setCursor(0, 0);
  if      (porcentagem >= 70) lcd.print("  Parabens!    ");
  else if (porcentagem >= 50) lcd.print("  Quase la!    ");
  else                        lcd.print("   Melhore!    ");

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
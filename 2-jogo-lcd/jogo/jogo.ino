/*
   ATENCAO: Este codigo eh feio e cabeludo, mas ele ja sabe disso.
   Um dia vamos arrumar o coitado.
*/

#include <LiquidCrystal.h>
#define FUNDO 0 #define MEIO 1 #define FRENTE 2
#define UNICO 3

#define PLAYER 14

#define BLEFT 6
#define BRIGHT 7

#define min(a,b) ((a<b)?(a):(b))


byte player[8] = {
  B10000,
  B11011,
  B11100,
  B11110,
  B11100,
  B11011,
  B10000,
};

// CAMINHOES:

byte fundo[8] = {
  B01100,
  B11111,
  B00001,
  B00001,
  B00001,
  B11111,
  B01100,
};
byte meio[8] = {
  B00000,
  B11111,
  B00000,
  B00000,
  B00000,
  B11111,
  B00000,
};
byte frente[8] = {
  B00110,
  B11111,
  B10000,
  B10000,
  B10000,
  B11111,
  B00110,
};
byte unico[8] = {
  B00001,
  B00010,
  B00100,
  B01000,
  B00100,
  B00010,
  B00001,
};

//Inicialização do display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//Declaração da matriz
char matrix[128][2];

char vazio = ' ';

//Função que gera a matriz do jogo 
void gerarMatriz() {
	//Guarda o char <espaço> em todos os espaços da matriz
  for (int i = 0; i < 128; i++) {
    for (int j = 0; j < 2; j++) {
      matrix[i][j] = vazio;
    }
  }
  // Nesta parte os "carros" sao gerados na matriz.
  // O for comeca apartir do 20 para deixar um espaco vazio
  // no inicio do jogo, para dar tempo para o jogador se acostumar
  // com os controles.
  for (int i = 20; i < 127; i++) {

    // Este if tem a funcionalidade de checar a validez da posicao "i",
    // para sempre deixar vazio o espaco entre um carro e o outro
    // deixando assim, sempre a possibilidade para o jogador nao perder.
    if (matrix[i][0] != vazio || matrix[i][1] != vazio ||
        matrix[i - 1][0] != vazio || matrix[i - 1][1] != vazio ||
        matrix[i - 2][0] != vazio || matrix[i - 2][1] != vazio) {
    } else {
      // Checando se vamos construir um obstaculo ou nao.
      if ((rand() % 10) > 3) {
        // Coordenada y do novo obstaculo a ser gerado.
        int y = rand() % 2;

        // Tamanho do obstaculo, que varia entre 2-6
        // E que nunca passará do final da matriz para 
        // o inicio, por isso o min.
        int tamanho = min((rand() % 5) + 2, 128 - i);

        // A primeira posicao do obstaculo sera a frente
        matrix[i][y] = byte(FRENTE);

        // As do meio serao do meio
        for (int j = i + 1; j < i + tamanho - 1; j++) {
          matrix[j][y] = byte(MEIO);
        }

        // A ultima posicao do obustaculo sera o fundo
        matrix[i + tamanho - 1][y] = byte(FUNDO);

      }
    }
  }
}

int passado = 0;

void setup() {
	//Inicializações	
  Serial.begin(9600);
  pinMode(BLEFT, INPUT);
  pinMode(BRIGHT, INPUT);

	//Mensagem de boas vindas
  lcd.begin(16, 2);
  lcd.print("Put On a");
  lcd.setCursor(0, 1);
  lcd.print("HappyFace :)");
  delay(2000);

	//Gera a matriz do jogo
  gerarMatriz();
	//Cria os chars baseados nos bytecodes definidos no topo do código
  lcd.createChar(UNICO, unico);
  lcd.createChar(FRENTE, frente);
  lcd.createChar(FUNDO, fundo);
  lcd.createChar(MEIO, meio);
  lcd.createChar(PLAYER, player);

  passado = millis();
}

int offset = 0;

int past_l = 0, past_r = 0;
int carrinho = 0;

// Funcao que sera chamada quando o jogador pressionar
// um botao de acao esquerdo (a ou btn_esq)
void l_callback(int l) {
  //Serial.println("PUTONA HAPPYFACE2");
  carrinho = 0;
}

// Funcao que sera chamada quando o jogador pressionar
// um botao de acao direito (s ou btn_dir)
void r_callback(int r) {
  //Serial.println("PUTONA HAPPYFACE");
  carrinho = 1;
}

// Funcao para receber input do player
int input() {

  // Ler os botoes
  int l = digitalRead(BLEFT);
  int r = digitalRead(BRIGHT);
  int pressed = 0;

  // Ler input da serial para a comunicacao com o python no computador
  String s = "";
  if (Serial.available() > 0) {
    s = Serial.readString();
  }

  // Acao esquerdo
  if (s == "l") {
    l_callback(1);
    pressed = 1;
  }

  // Acao direita
  if (s == "r") {
    r_callback(1);
    pressed = 1;
  }

  // Acao btn_esq
  // Acao sera executada apenas na borda ascendente do sinal
  if (l != past_l) {
    if (l == HIGH) {
      l_callback(l);
      pressed = 1;
    }

  }

  past_l = l;

  // Acao btn_dir
  // Acao sera executada apenas na borda ascendente do sinal
  if (r != past_r) {
    if (r == HIGH) {
      r_callback(r);
      pressed = 1;
    }
  }

  past_r = r;

  return pressed;

}

// Funcao para abstrair o processo de desenhar no lcd
void print_raw(int x, int y, char c) {
  lcd.setCursor(x, y);
  lcd.write(c);
}

// wrapper para print_raw que nao faz mais sentido
void print(int x, int y, char c) {
  //if(c != vazio) {
  print_raw(x, y, c);
  //}
}


// Preencer a matriz com vazio
void clear_screen() {
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 2; j++) {
      print_raw(i, j, vazio);
    }
  }
}

// Checar colisao com o jogador
int checar_perda() {
  if (matrix[offset % 128][carrinho] != vazio) {
    return 1;
  }

  return 0;
}

int perdeu = 0;
int score = 0;

void loop() {
  // Loop principal do jogo (game loop)
  if (!perdeu) {
    // Ler o input dos butoes e teclado
    input();

    // Desenhar o jogador.
    // Foi observado que o quanto antes o jogador for desenhado
    // por mais tempo ele sera visivel na tela, logo eh melhor
    // que o jogador seja desenhado antes de tudo.
    print(0, carrinho, PLAYER);

    for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 2; j++) {
        // Gambiarra para nao desenhar os obstaculos sobre o player
        if (j == carrinho && i == 0) continue; 
        print(i, j, matrix[(i + offset) % 128][j]);

      }
    }

    // Checando a condicao de perda
    if (checar_perda() == 1) {
      perdeu = 1;
      clear_screen();
      score = offset;
    }

    offset += (millis() - passado) / 30;
    score += (millis() - passado);
    passado = millis();

    // Tela de game over
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Put On a SadFace");
    lcd.setCursor(0, 1);
    lcd.print(":(");
    lcd.setCursor(4, 1);
    // Calculo tosco do score
    // Foi testado e aprovado empiricamente
    lcd.print((score/10)*(score/10)-100);

    // Reinicializando o estado do jogo assim que o jogador mandar uma entrada (teclado ou botoes)
    if (input()) {
      perdeu = 0;
      offset = 0;
      score = 0;
      srand(millis());
      gerarMatriz();
    }

  }
}

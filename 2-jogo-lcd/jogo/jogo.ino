/*
   ATENÇÂO: Este código é feio e cabeludo, mas ele já sabe disso.
   Um dia vamos arrumar o coitado.

*/

#include <LiquidCrystal.h>

#define FUNDO 0
#define MEIO 1
#define FRENTE 2
#define UNICO 3

#define BLEFT 6
#define BRIGHT 7

#define min(a,b) ((a<b)?(a):(b))

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

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
char matrix[128][2];
char vazio = ' ';

void setup() {

  Serial.begin(9600);
  pinMode(BLEFT, INPUT);
  pinMode(BRIGHT, INPUT);

  lcd.begin(16, 2);
  lcd.print("PUTONA HAPPYFACE");
  for (int i = 0; i < 128; i++) {
    for (int j = 0; j < 2; j++) {
      matrix[i][j] = vazio;
    }
  }

  for (int i = 5; i < 128; i++) {
    if (matrix[i][0] != vazio || matrix[i][1] != vazio ||
        matrix[i - 1][0] != vazio || matrix[i - 1][1] != vazio ||
        matrix[i - 2][0] != vazio || matrix[i - 2][1] != vazio) {
    } else {
      int y = rand() % 2;
      if (rand() % 10) {
        int tamanho = min((rand() % 5) + 2, 128 - i);
        matrix[i][y] = byte(FRENTE);
        for (int j = i + 1; j < i + tamanho - 1; j++) {

          matrix[j][y] = byte(MEIO);
        }
        matrix[i + tamanho - 1][y] = byte(FUNDO);
        if (tamanho == 1) {
          matrix[i][y] = byte(UNICO);
        }
      }
    }
  }

  lcd.createChar(UNICO, unico);
  lcd.createChar(FRENTE, frente);
  lcd.createChar(FUNDO, fundo);
  lcd.createChar(MEIO, meio);
}

int offset = 0;

int p = 1;

void MoveLeft() {
  p = 1 - p;
}

int past_l = 0, past_r = 0;
int carrinho=0;

void l_callback(int l) {
  Serial.println("PUTONA HAPPYFACE2");
  carrinho = 0;
}

void r_callback(int r) {
  Serial.println("PUTONA HAPPYFACE");
  carrinho = 2;
}

void input() {
  int l = digitalRead(BLEFT);
  int r = digitalRead(BRIGHT);

  if (l != past_l) {
    if (l == HIGH) {
      l_callback(l);
    }

  }

  past_l = l;


  if (r != past_r) {
    if (r == HIGH) {
      r_callback(r);
    }
  }

  past_r = r;

}

void print_raw(int x, int y, char c) {
  lcd.setCursor(x, y);
  lcd.write(c);
}

void print(int x, int y, char c) {
  if(c != vazio) {
    print_raw(x,y,c);
  }
}


void clear_screen() {
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 2; j++) {
      print_raw(i,j, vazio);
    }
  }
}

int checar_perda() {
  if(matrix[offset%128][carrinho] != vazio) {
    return 1;
  }

  return 0;
}

void loop() {
  lcd.setCursor(0, 1);

  input();
  clear_screen();
  

  print(0,carrinho, UNICO);

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 2; j++) {
      print(i, j, matrix[(i + offset) % 128][j]);
      
    }
  }

  if(checar_perda() == 1) {
    lcd.setCursor(0,0);
    lcd.print("SE FUDEU");
    return;
  }

  offset = millis() / 150;
}

/*
 * ATENÇÂO: Este código é feio e cabeludo, mas ele já sabe disso.
 * Um dia vamos arrumar o coitado.
 *
 */

#define FUNDO 0
#define MEIO 1
#define FRENTE 2
#define UNICO 3

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

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
char matrix[128][2];

void setup() {
  lcd.begin(16, 2);
  lcd.print("PUTONA HAPPYFACE");
  char vazio = '-';
  for (int i = 0; i < 128; i++) {
    for (int j = 0; j < 2; j++) {
      matrix[i][j] = vazio;
    }
  }


  for (int i = 5; i < 128; i++) {
    if (matrix[i][0] != vazio || matrix[i][1] != vazio || matrix[i - 1][0] != vazio || matrix[i - 1][1] != vazio || matrix[i - 2][0] != vazio || matrix[i - 2][1] != vazio) {
    } else {
      int y = rand() % 2;
      if (rand() % 10) {
        int tamanho = min((rand() % 5) + 2, 128-i);
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

void loop() {
  lcd.setCursor(0, 1);
  //lcd.print("PUTONA happy face");
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 2; j++) {
      lcd.setCursor(i, j);
      lcd.write(matrix[(i + offset) % 128][j]);
    }
  }
  offset = millis() / 150;
}

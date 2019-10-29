#define RODADAS 19
#define BTNS 3
#define RESET 2
//int jogo[19][3]={0};
// auto-explicativo
int buttons[BTNS]={A0, A1, A2},estagio=1,pos=0;
int leds[BTNS]={8,9,10};

// estado do botão
int pressionado=0;

void gerarJogo(int** mat){
    for(int i=0;i<RODADAS;i++){
        int led = random(3);
        *(*(mat+i)+led) = 1; 
    }
}

void desenharJogo(int** mat){
    for(int i=0;i<RODADAS;i++){
        for(int j=0;j<3;j++){
            Serial.print(*(*(mat+i)+j));
        }

        Serial.println();
    }
}

int** alocarJogo(){
    int** mat = (int**) calloc(RODADAS, sizeof(int*));

    for(int i=0;i<RODADAS;i++){
        *(mat+i)=(int*) calloc(BTNS, sizeof(int));
    }

    return mat;
}

void liberarJogo(int** p){
    for(int i=0;i<RODADAS;i++){
        free(*(p+1));
    }

    free(p);
}

// retorna uma bitmask com o estado dos botões
int lerInput(){
    int mask=0;
    for(int i=0;i<BTNS;i++){
        if(digitalRead(buttons[i])==HIGH){
            mask|=(1<<i);
        }
    }

    return mask;
}

int checarJogada(int** mat, int jogada){

    int target=0;
    for(int i=0;i<BTNS;i++){
        if((*(*(mat+pos)+i))) target|= (1<<i);
    }
    Serial.print("Target: ");
    Serial.println(target);
    return (jogada==target);
}

void proximaPosicao(int** jogo){
    if(++pos>=estagio) {
        if(++estagio==RODADAS) Serial.println("ACABOU-se");
        printCres(jogo);
        pos=0;
        
    }
}

void printCres(int** jogo){
    //Serial.println("Printing");
    delay(200);
    for(int i=0;i<estagio;i++){
        for(int j=0;j<BTNS;j++){
            if(*(*(jogo+i)+j)) {
                //Serial.print("led: ");
                //Serial.println(j);
                led(j, 1);
                delay(1000);
                led(j, 0);
                if(!(i==estagio-1&&j==BTNS-1))delay(500);

            }
        }
    }
}

void printJogo(int** jogo){
    for(int i=0;i<BTNS;i++){
        //led(i, *(*(jogo+pos)+i));
        led(i, 0);
    }
}

void led(int idx, int state){
    //TODO: implementar
    digitalWrite(leds[idx], state?HIGH:LOW);
}

void reiniciarJogo(int** jogo){
    for(int i=0;i<BTNS;i++){
        digitalWrite(leds[i], HIGH);
    }

    delay(2000);
    digitalWrite(RESET, LOW);
    //iniciarJogo(jogo);
}

void iniciarJogo(int** jogo){
    gerarJogo(jogo);
    estagio=1;
    pos=0;
    printJogo(jogo);
    desenharJogo(jogo);
    printCres(jogo);
}

void printDebug() {

    Serial.print("Estagio: ");
    Serial.print(estagio);
    Serial.print(" pos ");
    Serial.println(pos);
}

int** jogo=NULL;

void setup() {

    randomSeed(analogRead(A4));
    digitalWrite(RESET, HIGH);
    delay(200);

    Serial.begin(9600);
    for(int i=0;i<BTNS;i++){
        pinMode(buttons[i], INPUT);
        pinMode(leds[i], OUTPUT);
    }

    pinMode(RESET, OUTPUT);

    jogo = alocarJogo();
    iniciarJogo(jogo);
    //desenharJogo(jogo);
    //printCres(jogo);
}

void loop() { 
    printJogo(jogo);
    //digitalWrite(9, HIGH);
    int in = lerInput();
    if(in){
        //Serial.println(in);
        if(!pressionado){
            Serial.println("Pressionado");
            if(checarJogada(jogo, in)){
                printDebug();
                proximaPosicao(jogo);
                delay(250);
            } else {
                Serial.println("Errou");
                reiniciarJogo(jogo);
            }
        }
        pressionado=1;
    } else {
        pressionado=0;
    }
}
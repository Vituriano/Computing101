#define RODADAS 19
#define BTNS 3

//int jogo[19][3]={0};

// auto-explicativo
int buttons[BTNS]={A0, A1, A2},estagio=0,pos=0;
int leds[BTNS]={8,9,10};

/* ---- Define as cores e a ordem em que aparecem no jogo ----
  
  void gerarJogo recebe a matriz como parâmetro
  e, para cada linha, atribui 1 para UMA das colunas
  
  ex: 0 1 0
	  1 0 1
	  0 0 1
	   ...													*/
void gerarJogo(int** mat){
    for(int i=0;i<RODADAS;i++){ //para cada linha,
        int led = rand()%3; //escolha uma coluna aleatoriamente
        *(*(mat+i)+led) = 1; //atribua 1 a este espaço na matriz. 
    }
}

/* void desenharJogo desenha a matriz no serial */
void desenharJogo(int** mat){
    for(int i=0;i<RODADAS;i++){
        for(int j=0;j<3;j++){
            Serial.print(*(*(mat+i)+j));
        }

        Serial.println();
    }
}

/* ---- Declaração dinâmica de matriz ----
  Aloca espaço para os ponteiros que definem a matriz
  e para o próprio conteúdo da matriz 
  A função retorna um **int --> vetor de *int */
int** alocarJogo(){
	/*Aloca espaço para <RODADAS> *int
	  mat --> *int (linhas)*/
    int** mat = (int**) calloc(RODADAS, sizeof(int*));
	/*Aloca espaço para <BTNS> int
	  *mat --> int (colunas)*/
    for(int i=0;i<RODADAS;i++){
        *(mat+i)=(int*) calloc(BTNS, sizeof(int));
    }
	//Retorna **int (vetor de *int)
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
    return !(jogada^target);
}

void proximaPosicao(int** jogo){
    if(++pos>=estagio) {
        if(++estagio==RODADAS) Serial.println("ACABOU-se");
        printCres(jogo);
        pos=0;
    }
}

void printCres(int** jogo){
    for(int i=0;i<estagio;i++){
        for(int j=0;j<BTNS;j++){
            if(*(*(jogo+i)+j)) {
                led(j, 1);
                delay(1000);
                led(j, 0);
                delay(500);

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

int** jogo=NULL;

void setup() {
    Serial.begin(9600);

	//Declara as entradas e saídas no arduino
    for(int i=0;i<BTNS;i++){
        pinMode(buttons[i], INPUT);
        pinMode(leds[i], OUTPUT);
    }

	//int** jogo aponta para cada linha da matriz
    jogo = alocarJogo();

    gerarJogo(jogo);
    desenharJogo(jogo);
    printCres(jogo);
}

void loop() { 
    printJogo(jogo);
    //digitalWrite(9, HIGH);
    int in = lerInput();
    if(in){
        Serial.println(in);
        if(checarJogada(jogo, in)){
            proximaPosicao(jogo);
        }
    }

    delay(10);
}

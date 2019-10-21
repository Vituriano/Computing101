byte LED[10][7] = { 
    {1, 1, 1, 1, 1, 1, 0}, //DIGITO 0
    {0, 1, 1, 0, 0, 0, 0}, //DIGITO 1
    {1, 1, 0, 1, 1, 0, 1}, //DIGITO 2
    {1, 1, 1, 1, 0, 0, 1}, //DIGITO 3
    {0, 1, 1, 0, 0, 1, 1}, //DIGITO 4
    {1, 0, 1, 1, 0, 1, 1}, //DIGITO 5
    {1, 0, 1, 1, 1, 1, 1}, //DIGITO 6
    {1, 1, 1, 0, 0, 0, 0}, //DIGITO 7
    {1, 1, 1, 1, 1, 1, 1}, //DIGITO 8
    {1, 1, 1, 0, 0, 1, 1}  //DIGITO 9
};
int valor_i = 0, SenhaInput[4], Senha = 1234;
int flag = 0;
void setup()
{
  pinMode(2, OUTPUT); //PINO 2 -> SEGMENTO A  
  pinMode(3, OUTPUT); //PINO 3 -> SEGMENTO B
  pinMode(4, OUTPUT); //PINO 4 -> SEGMENTO C
  pinMode(5, OUTPUT); //PINO 5 -> SEGMENTO D
  pinMode(6, OUTPUT); //PINO 6 -> SEGMENTO E
  pinMode(7, OUTPUT); //PINO 7 -> SEGMENTO F
  pinMode(8, OUTPUT); //PINO 8 -> SEGMENTO G
  pinMode(9, INPUT);  //Botão Esquerda
  pinMode(10, INPUT); //Botão Direita
  printa(0);
}

void printa(int digit){
  for (int i = 2; i <= 8; i++)
  {
    digitalWrite(i, 0);
  }
  for(int i = 0; i <= 7; i++){
  	digitalWrite(i + 2, LED[digit][i]);
  }
}

void loop()
{
	int a = digitalRead(9);
    if(a == 1){
      if(flag == 0){	
      	valor_i++;
    	printa(valor_i%10);
    	flag = 1;
      }
    }else{
      flag = 0;
    }
}
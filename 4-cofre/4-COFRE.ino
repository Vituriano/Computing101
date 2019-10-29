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

int senhaInput[4], senha = 1234, senhaIn = 1;
int flag = 0, flag_B = 0, cont = 0, validacao = 0, valor_i = 0;

void setup()
{
  pinMode(2, OUTPUT);  //PINO 2 -> SEGMENTO A
  pinMode(3, OUTPUT);  //PINO 3 -> SEGMENTO B
  pinMode(4, OUTPUT);  //PINO 4 -> SEGMENTO C
  pinMode(5, OUTPUT);  //PINO 5 -> SEGMENTO D
  pinMode(6, OUTPUT);  //PINO 6 -> SEGMENTO E
  pinMode(7, OUTPUT);  //PINO 7 -> SEGMENTO F
  pinMode(8, OUTPUT);  //PINO 8 -> SEGMENTO G
  pinMode(13, OUTPUT); //LED Verde
  pinMode(12, OUTPUT); //LED Vermelho
  pinMode(9, INPUT);   //Botão Esquerda -> Muda o valor do LED.
  pinMode(10, INPUT);  //Botão Direita -> Salva os numeros no vetor.
  printa(0);
}

// Função que printa o valor do numero no LED.
void printa(int digit)
{
  for (int i = 2; i <= 8; i++)
  {
    digitalWrite(i, 0);
  }
  for (int i = 0; i <= 7; i++)
  {
    digitalWrite(i + 2, LED[digit][i]);
  }
}

// Função recursiva que compara as duas senhas.
int comparaSenha(int senhaIn, int senha, int tam)
{
  if (senha % ((int)pow(10, tam)) == senhaIn % ((int)pow(10, tam)) && tam == 1)
    return 1;
  else if (senha % ((int)pow(10, tam)) == senhaIn % ((int)pow(10, tam)))
  {
    return comparaSenha(senhaIn, senha, tam - 1);
  }
  else
    return 0;
}

void loop()
{
  // Incrementando o numero no LED.
  int a = digitalRead(9);
  if (a == 1)
  {
    if (flag == 0)
    {
      valor_i++;
      printa(valor_i % 10);
      flag = 1;
    }
  }
  else
  {
    flag = 0;
  }

  // Salvando os numeros no vetor.
  int b = digitalRead(10);
  if (b == 1)
  {
    if (flag_B == 0 && cont < 4)
    {
      senhaInput[cont] = valor_i % 10;
      cont++;
      flag_B = 1;
    }
  }
  else
  {
    flag_B = 0;
  }
  
  // Verifica após o usuário escolher os 4 digitos
  if (cont == 4)
  {
    // tranforma o vetor em inteiro
    for (int i = 0; i < 4; i++)
    {
      senhaIn += senhaInput[i] * pow(10, 3 - i);
    }

    validacao = comparaSenha(senhaIn, senha, 4);
    if (validacao)
    {
      digitalWrite(13, 0);
      digitalWrite(12, 1);
    }
    else
    {
      digitalWrite(13, 1);
      digitalWrite(12, 0);
    }
    cont = 0;
    senhaIn = 1;
  }
}
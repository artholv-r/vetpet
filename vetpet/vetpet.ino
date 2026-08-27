//Registrador é o endereço físico de um byte na memória. O que tá dentro dos parênteses em Wire.() são os dados.
#include <Wire.h>

// Endereço I2C do ADXL345
const int endereco_ADXL345 = 0x1D; 

// Variáveis para armazenar os valores puros (16 bits)
int16_t eixo_X, eixo_Y, eixo_Z;

void setup() {
  delay(10); // Pequeno atraso para o sensor estabilizar
  Serial.begin(115200);
  Wire.begin(); // Inicializa o barramento I2C
  
  //Tirando o sensor do standby
  Wire.beginTransmission(endereco_ADXL345);
  Wire.write(0x2D); // Registrador POWER_CTL
  Wire.write(0x08); // Escreve 8 (ativa o bit de "Measure")
  Wire.endTransmission();
  //atribuindo precisão
  Wire.beginTransmission(endereco_ADXL345);
  Wire.write(0x31);
  Wire.write(0x01); //  +/- 2*9.8m/s²
  Wire.endTransmission();
  //Corrigindo offset
  Wire.beginTransmission(endereco_ADXL345);
  Wire.write(0x1E);
  Wire.write(0x01);
  Wire.write(0x06);
  Wire.write(0x01);
  Wire.endTransmission();
}

void loop() {
  //iniciando o processo de leitura 
  Wire.beginTransmission(endereco_ADXL345);
  Wire.write(0x32); // Avisamos que queremos começar a ler a partir do registrador 0x32 (DATAX0)
  Wire.endTransmission(false); // false mantém a conexão ativa para leitura
  
  // Lendo os bits do sensor
  // (X0, X1, Y0, Y1, Z0, Z1)
  Wire.requestFrom(endereco_ADXL345, 6, true); 
  
  // Como a leitura retorna 8 bits, precisamos juntar 2 leituras para formar um número de 16 bits.
  
  if (Wire.available() <= 6) {
    eixo_X = Wire.read() | (Wire.read() << 8); 
    eixo_Y = Wire.read() | (Wire.read() << 8);
    eixo_Z = Wire.read() | (Wire.read() << 8); 
  }
  eixo_X=eixo_X*0.0813;
  eixo_Y=eixo_Y*0.0813;
  eixo_Z=eixo_Z*0.0813;
  //Exibindo as leituras
  Serial.print("X: ");
  Serial.print(eixo_X);
  Serial.print(" \tY: ");
  Serial.print(eixo_Y);
  Serial.print(" \tZ: ");
  Serial.println(eixo_Z);
  Serial.println();
  if (eixo_X>2) {
    Serial.println("");
  }
  delay(250); // Aguarda 250 milissegundos antes da próxima leitura
}
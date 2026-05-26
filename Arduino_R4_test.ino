/************************************
* Parque Tecnológico de Santo André *
* Rede Ciência Cidadã               *
* Estação Meteorológica Maker       *
* Autor: Matheus Valadares Teixeira *
* ***********************************/

// Bibliotecas Obrigatórias
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <BH1750.h>

// Configurações da Tela OLED
#define SCREEN_WIDTH 128 // Largura do display em pixels
#define SCREEN_HEIGHT 64 // Altura do display em pixels
#define OLED_RESET    -1 // Reset compartilhado com o Arduino
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Configurações do DHT11
#define DHTPIN 2       // Pino digital conectado ao DHT11
#define DHTTYPE DHT11   // Definindo o modelo do sensor
DHT dht(DHTPIN, DHTTYPE);

// Configurações do GY-30 (BH1750)
BH1750 lightMeter;

// Configurações do Sensor de Umidade do Solo Capacitivo
const int SOIL_PIN = A0; 

// VALORES DE CALIBRAÇÃO DO SOLO (Ajuste conforme o seu sensor)
// Deixe o sensor no ar seco e anote o valor -> coloque em VALOR_SECO
// Coloque o sensor em um copo d'água e anote o valor -> coloque em VALOR_UMIDO
const int VALOR_SECO = 1020;  
const int VALOR_UMIDO = 410; 

void setup() {
  Serial.begin(9600);
  
  // Inicializa a comunicação I2C
  Wire.begin();

  // Inicializa o Display OLED (Endereço padrão costuma ser 0x3C)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao inicializar a tela OLED. Verifique as conexões!"));
    for(;;); // Trava o código aqui se não achar a tela
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE); //O modelo escolhido foi de visor branco
  display.setTextSize(1);
  
  // Inicializa o DHT11
  dht.begin();

  // Inicializa o GY-30
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("GY-30 inicializado com sucesso."));
  } else {
    Serial.println(F("Erro ao inicializar o GY-30."));
  }
}

void loop() {
  // --- Leitura do DHT11 ---
  float umidadeAr = dht.readHumidity();
  float tempAr = dht.readTemperature();

  // --- Leitura da Luminosidade (GY-30) ---
  float lux = lightMeter.readLightLevel();

  // --- Leitura do Sensor de Solo ---
  int soloRaw = analogRead(SOIL_PIN);
  // Converte a leitura analógica para porcentagem invertida (pois sensores capacitivos diminuem o valor analógico quanto mais úmido o solo está)
  int umidadeSolo = map(soloRaw, VALOR_SECO, VALOR_UMIDO, 0, 100);
  // Garante que o valor fique estritamente entre 0% e 100%
  umidadeSolo = constrain(umidadeSolo, 0, 100);

  // --- Verificação de erro no DHT ---
  if (isnan(umidadeAr) || isnan(tempAr)) {
    Serial.println(F("Falha na leitura do sensor DHT11!"));
    tempAr = 0;
    umidadeAr = 0;
  }

  // --- Atualização da Tela OLED ---
  display.clearDisplay();
  
  // Título / Cabeçalho
  display.setTextSize(1);
  display.setCursor(15, 0);
  display.print("MONITOR DE SENSORES");
  display.drawFastHLine(0, 10, 128, SSD1306_WHITE); // Linha divisória

  // Exibição dos dados
  display.setCursor(0, 16);
  display.print("Temp. Ar: ");
  display.print(tempAr, 1);
  display.println(" C");

  display.print("Umid. Ar: ");
  display.print(umidadeAr, 1);
  display.println(" %");

  display.print("Umid. Solo: ");
  display.print(umidadeSolo);
  display.println(" %");

  display.print("Luminos.: ");
  display.print(lux, 0);
  display.println(" lx");

  // Envia tudo configurado para a tela física
  display.display();

  // Delay de 2 segundos antes da próxima atualização (o DHT11 precisa de pelo menos 2s entre leituras)
  delay(2000);
}
#include <UrlEncode.h>
#include <HTTPClient.h>
#include <WiFi.h>

// Definição dos pinos
#define LED_AZUL 2
#define LED_VERMELHO 18
#define LED_VERDE 5
#define BUZZER 16
#define MQ135_PIN 36 //A0

// Configurações da rede WiFi
const char* ssid = "Limas_2G";
const char* password = "q1w2e3r4@2G";

// Função para acender o LED Azul
void acenderLedAzul() {
  digitalWrite(LED_AZUL, HIGH);
}

// Função para apagar o LED Azul
void apagarLedAzul() {
  digitalWrite(LED_AZUL, LOW);
}

// Função para acender o LED Vermelho e apagar o LED Verde
void acenderLedVermelho() {
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_VERDE, LOW);
}

// Função para apagar o LED Vermelho e acender o LED Verde
void apagarLedVermelho() {
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_VERDE, HIGH);
}

void setup() {
  // Configuração dos pinos como saídas
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Conexão à rede WiFi
  WiFi.begin(ssid, password);
  Serial.begin(9200);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando à rede WiFi...");
  }

  Serial.println("Conexão WiFi estabelecida!");
  Serial.println("IP obtido: ");
  Serial.println(WiFi.localIP());

  // Ligando o LED Azul quando conectado à rede WiFi
  acenderLedAzul();
}

void loop() {
  // Leitura do valor do sensor MQ-135
  int gasValue = analogRead(MQ135_PIN);

  
  // Verificando se o sensor detectou gás
  if (gasValue > 500) {
    // Acende o LED Vermelho e apaga o LED Verde
    acenderLedVermelho();
    digitalWrite(BUZZER, HIGH); // Ativa o buzzer
    digitalWrite(BUZZER, LOW);    
    
    Serial.print("Concentraçao de Gás Detectado (ppm): ");
    Serial.println(gasValue);    
  } else {
    // Apaga o LED Vermelho e acende o LED Verde
    apagarLedVermelho();
    digitalWrite(BUZZER, LOW); // Desativa o buzzer
    Serial.println("Zerada concentraçao de Gás! / Muito abaixo do risco.");
    Serial.println(gasValue);
  }

  delay(1000); // Aguarda 1 segundo antes de ler novamente o sensor
}

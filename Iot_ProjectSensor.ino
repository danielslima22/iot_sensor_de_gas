#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <HTTPClient.h>
#include <WiFi.h>


// Definição dos pinos
#define LED_AZUL 2
#define LED_VERMELHO 18
#define LED_VERDE 5
#define BUZZER 16
#define MQ135_PIN 36 //A0

// Configurações do Telegram
#define bot_token "numero do token"
#define chat_id "numero do id"


WiFiClientSecure clientSecure;
UniversalTelegramBot bot(bot_token, clientSecure);
// Intervalor de 1 minuto para envio de nova notificação.
int botRequestDelay = 60000;
unsigned long lastTimeBotRan;


// Configurações da rede WiFi
const char* ssid = "nome da rede"; 
const char* password = "senha de acesso a rede";


// Envia notificação ao Telegram
void SendTelegramNotification(String message) {
bot.sendMessage(chat_id, message, "");
Serial.println(message);
}


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

  // Add root certificate for api.telegram.org
  clientSecure.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  // Testa notificação pelo telegram
  //SendTelegramNotification("Testando envio de notificação.");
}

void loop() {
  // Leitura do valor do sensor MQ-135
  int gasValue = analogRead(MQ135_PIN);
  
  
  String send_msg = "";
  String aux = "";
  
  // Verificando se o sensor detectou gás
  if (gasValue > 700) {
    // Acende o LED Vermelho e apaga o LED Verde
    acenderLedVermelho();
    digitalWrite(BUZZER, HIGH); // Ativa o buzzer
    delay(500);
    digitalWrite(BUZZER, LOW);
    delay(500);    
    aux = String(gasValue);
    Serial.print("Concentraçao de Gás Detectado (ppm): " + aux);
    Serial.println(gasValue);  
    send_msg = "ALERTA! Alta concentraçao de Gás Detectado (ppm): " + aux;

    if (millis() > lastTimeBotRan + botRequestDelay)  {
    SendTelegramNotification(send_msg); 
    lastTimeBotRan = millis();
    }
       
  } else {
    // Apaga o LED Vermelho e acende o LED Verde
    apagarLedVermelho();
    digitalWrite(BUZZER, LOW); // Desativa o buzzer
    Serial.println("Zerada concentraçao de Gás! / Muito abaixo do risco.");    
    
  }
  
  delay(2000); // Aguarda 2 segundo antes de ler novamente o sensor
}
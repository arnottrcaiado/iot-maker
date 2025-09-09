// ===================================================================================
// Projeto: Monitor de Clima com ESP32-C3, DHT11 e ThingSpeak
// Autor: Arnott Ramos Caiado - Documentação com apoio da IAG Gemini
// Versão: 1.0 - Data: 12/8/2025
// Descrição: Este código lê a temperatura e umidade de um sensor DHT11 e envia os
//            dados para um canal no ThingSpeak. Utiliza WiFiManager e constantes
//            definidas via #define para máxima configurabilidade e clareza.
// ===================================================================================

// --- Inclusão de Bibliotecas ---
#include <WiFi.h>
#include <WiFiManager.h>
#include <DHT.h>
#include <ThingSpeak.h>
#include "chaves.h"

// ===================================================================================
// --- CONFIGURAÇÕES GLOBAIS E CONSTANTES ---
// Usar #define permite centralizar todas as configurações em um único local.
// ===================================================================================

// --- Configurações de Comunicação ---
#define SERIAL_BAUD_RATE 115200          // Velocidade da comunicação com o Monitor Serial.

// --- Configurações do Wi-Fi ---
#define WIFI_MANAGER_AP_NAME "ESP32-IoT-Setup" // Nome do Ponto de Acesso (AP) para configuração.
#define RESTART_DELAY_MS 5000            // Tempo (em ms) para reiniciar o ESP em caso de falha de conexão.

// --- Configurações do Sensor DHT11 ---
#define DHT_PIN 4                        // Pino GPIO onde o sensor DHT11 está conectado.
#define DHT_TYPE DHT11                   // Tipo de sensor DHT (DHT11 ou DHT22).
#define TEMP_MIN_VALID -10               // Temperatura mínima considerada válida para envio.
#define TEMP_MAX_VALID 50                // Temperatura máxima considerada válida para envio.
#define HUMI_MIN_VALID 0                 // Umidade mínima considerada válida para envio.
#define HUMI_MAX_VALID 100               // Umidade máxima considerada válida para envio.

// --- Configurações do ThingSpeak ---
#define POSTING_INTERVAL_MS 30000        // Intervalo (em ms) entre envios de dados (30 segundos).
#define THINGSPEAK_FIELD_TEMP 2          // Campo do canal ThingSpeak para enviar a temperatura.
#define THINGSPEAK_FIELD_HUMI 1          // Campo do canal ThingSpeak para enviar a umidade.
#define HTTP_STATUS_OK 200               // Código de status HTTP para uma requisição bem-sucedida.

// --- Objetos Globais ---
DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient client;
unsigned long lastPostTime = 0;

// ===================================================================================
// FUNÇÃO SETUP: Executada uma vez na inicialização do ESP32.
// ===================================================================================
void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println("\n\n[SISTEMA] Iniciando o dispositivo...");

  WiFiManager wifiManager;
  Serial.println("[WIFI] Tentando conectar via WiFiManager...");

  if (!wifiManager.autoConnect(WIFI_MANAGER_AP_NAME)) {
    Serial.println("[ERRO] Falha ao conectar ao Wi-Fi. O dispositivo será reiniciado.");
    delay(RESTART_DELAY_MS);
    ESP.restart();
  }

  Serial.println("\n[WIFI] Conexão Wi-Fi estabelecida com sucesso!");
  Serial.print("[WIFI] Endereço IP: ");
  Serial.println(WiFi.localIP());

  dht.begin();
  Serial.println("[SENSOR] Sensor DHT inicializado.");

  ThingSpeak.begin(client);
  Serial.println("[THINGSPEAK] Cliente ThingSpeak iniciado.");
}

// ===================================================================================
// FUNÇÃO LOOP: Executada repetidamente após o setup.
// ===================================================================================
void loop() {
  if (millis() - lastPostTime >= POSTING_INTERVAL_MS) {
    Serial.println("\n-----------------------------------------");
    Serial.println("[SISTEMA] Ciclo de leitura e envio iniciado.");

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("[WIFI] Conexão perdida. Tentando reconectar...");
      WiFi.reconnect();
      return;
    }

    // --- 1. Ler os dados do sensor DHT11 ---
    Serial.print("[SENSOR] Lendo dados do pino ");
    Serial.println(DHT_PIN);
    float temperatura = dht.readTemperature();
    float umidade = dht.readHumidity();

    // --- 2. Validar as leituras do sensor ---
    if (isnan(temperatura) || isnan(umidade)) {
      Serial.println("[ALERTA] Falha ao ler do sensor DHT. Verifique o circuito. Envio cancelado.");
      return;
    }
    
    // Verificação de faixas válidas
    if (temperatura < TEMP_MIN_VALID || temperatura > TEMP_MAX_VALID || umidade < HUMI_MIN_VALID || umidade > HUMI_MAX_VALID) {
      Serial.println("[ALERTA] Leitura do sensor fora da faixa esperada. Envio cancelado.");
      Serial.print("Leitura: Temp=");
      Serial.print(temperatura);
      Serial.print("C, Humi=");
      Serial.print(umidade);
      Serial.println("%");
      return;
    }

    Serial.print("[SENSOR] Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");
    Serial.print("[SENSOR] Umidade: ");
    Serial.print(umidade);
    Serial.println(" %");

    // --- 3. Enviar os dados para o ThingSpeak ---
    Serial.println("[THINGSPEAK] Preparando para enviar dados...");
    ThingSpeak.setField(THINGSPEAK_FIELD_TEMP, temperatura);
    ThingSpeak.setField(THINGSPEAK_FIELD_HUMI, umidade);
    int statusCode = ThingSpeak.writeFields(THINGSPEAK_CHANNEL_ID, thingspeakApiKey);

    // --- 4. Verificar o resultado do envio ---
    if (statusCode == HTTP_STATUS_OK) {
      Serial.println("[THINGSPEAK] Dados enviados com sucesso!");
    } else {
      Serial.print("[ERRO] Falha ao enviar para o ThingSpeak. Código de status: ");
      Serial.println(statusCode);
      Serial.println("[DICA] Verifique seu ID de canal, chave de API e conexão com a internet.");
    }

    lastPostTime = millis();
    Serial.println("[SISTEMA] Ciclo finalizado. Aguardando próximo intervalo.");
    Serial.println("-----------------------------------------");
  }
}
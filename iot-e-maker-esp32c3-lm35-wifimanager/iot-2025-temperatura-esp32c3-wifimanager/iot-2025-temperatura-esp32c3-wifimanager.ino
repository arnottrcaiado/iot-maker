// ===================================================================================
// Faculdade SENAC - Ads - IOT
// Projeto: Monitor de Temperatura com ESP32, LM35 e ThingSpeak
// Autor: Arnott Ramos Caiado - documentação com apoio da IAG Gemini
// Versão: 2.1 - Data: 12/8/2025
// Descrição: Este código lê a temperatura de um sensor LM35 e envia os dados
//            para um canal no ThingSpeak. Utiliza WiFiManager e constantes
//            definidas via #define para máxima configurabilidade.
// ===================================================================================

// --- Inclusão de Bibliotecas ---
#include <WiFi.h>
#include <WiFiManager.h>
#include "ThingSpeak.h"
#include "chaves.h"

// ===================================================================================
// --- CONFIGURAÇÕES GLOBAIS E CONSTANTES ---
// Usar #define permite centralizar todas as configurações em um único local.
// ===================================================================================

// --- Configurações de Comunicação ---
#define SERIAL_BAUD_RATE 115200         // Velocidade da comunicação com o Monitor Serial.

// --- Configurações do Wi-Fi ---
#define WIFI_MANAGER_AP_NAME "ESP32-IoT -Setup" // Nome do Ponto de Acesso (AP) para configuração.
#define RESTART_DELAY_MS 5000           // Tempo (em ms) para reiniciar o ESP em caso de falha de conexão.

// --- Configurações do Sensor LM35 ---
#define LM35_PIN 4                      // Pino GPIO onde o sensor LM35 está conectado.
#define CALIBRATION_FACTOR_A 21.0       // Fator de calibração 'A' (numerador). Ajuste se necessário.
#define CALIBRATION_FACTOR_B 2014.0      // Fator de calibração 'B' (denominador). Ajuste se necessário.
#define TEMP_MIN_VALID -10              // Temperatura mínima considerada válida para envio.
#define TEMP_MAX_VALID 100              // Temperatura máxima considerada válida para envio.

// --- Configurações do ThingSpeak ---
#define POSTING_INTERVAL_MS 30000       // Intervalo (em ms) entre envios de dados (30 segundos).
#define THINGSPEAK_FIELD_TEMPERATURE 3  // Campo do canal ThingSpeak para enviar a temperatura.
#define HTTP_STATUS_OK 200              // Código de status HTTP para uma requisição bem-sucedida.

// --- Objetos Globais ---
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

    // --- 1. Ler a temperatura do sensor LM35 ---
    Serial.print("[SENSOR] Lendo dados do pino ");
    Serial.println(LM35_PIN);
    int sensorValue = analogRead(LM35_PIN);

    // Conversão usando os fatores de calibração definidos.
    float temperatureC = (float)sensorValue * (CALIBRATION_FACTOR_A / CALIBRATION_FACTOR_B);

    Serial.print("[SENSOR] Leitura analógica bruta: ");
    Serial.println(sensorValue);
    Serial.print("[SENSOR] Temperatura calculada: ");
    Serial.print(temperatureC);
    Serial.println(" °C");

    if (temperatureC < TEMP_MIN_VALID || temperatureC > TEMP_MAX_VALID) {
      Serial.println("[ALERTA] Temperatura fora da faixa esperada. Verifique o sensor. Envio cancelado.");
      return;
    }

    // --- 2. Enviar a temperatura para o ThingSpeak ---
    Serial.println("[THINGSPEAK] Preparando para enviar dados...");
    ThingSpeak.setField(THINGSPEAK_FIELD_TEMPERATURE, temperatureC);
    int statusCode = ThingSpeak.writeFields(THINGSPEAK_CHANNEL_ID, thingspeakApiKey);

    // --- 3. Verificar o resultado do envio ---
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

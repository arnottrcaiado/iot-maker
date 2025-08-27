//==================================================================================
// Faculdade SENAC - Ads - IOT
// Projeto: Monitor de Servidor Web com ESP32-C3
// Autor: Arnott Ramos Caiado - documentação com apoio da IAG Gemini
// Versão: 1.0 - Data: 27/08/2025
// Descrição: Este código realiza requisições HTTP GET periódicas para monitorar
//            a disponibilidade de um servidor web. Utiliza WiFiManager para
//            configuração Wi-Fi e constantes para máxima flexibilidade.
// ===================================================================================

// --- Inclusão de Bibliotecas ---
#include <WiFi.h>
#include <WiFiManager.h>
#include <HTTPClient.h>

// ===================================================================================
// --- CONFIGURAÇÕES GLOBAIS E CONSTANTES ---
// Usar #define permite centralizar todas as configurações em um único local.
// ===================================================================================

// --- Configurações de Comunicação ---
#define SERIAL_BAUD_RATE 115200         // Velocidade da comunicação com o Monitor Serial.

// --- Configurações do Wi-Fi Manager ---
#define WIFI_MANAGER_AP_NAME "Monitor-Web-Setup" // Nome do Ponto de Acesso (AP) para a configuração.
#define RESTART_DELAY_MS 5000                   // Tempo (em ms) para reiniciar em caso de falha.

// --- Configurações da Requisição HTTP ---
#define HTTP_SERVER_URL "http://example.com"    // URL do servidor web a ser monitorado.
#define POSTING_INTERVAL_MS 30000               // Intervalo (em ms) entre as requisições (30 segundos).
#define HTTP_STATUS_OK 200                      // Código de status HTTP para uma requisição bem-sucedida.

// --- Objetos Globais ---
HTTPClient http;
unsigned long lastRequestTime = 0;

// ===================================================================================
// FUNÇÃO SETUP: Executada uma vez na inicialização do ESP32.
// ===================================================================================
void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println("\n\n[SISTEMA] Iniciando o dispositivo...");

  WiFiManager wifiManager;
  Serial.println("[WIFI] Tentando conectar via WiFiManager...");

  // wifiManager.resetSettings(); // Descomente esta linha para redefinir as credenciais Wi-Fi salvas.

  if (!wifiManager.autoConnect(WIFI_MANAGER_AP_NAME)) {
    Serial.println("[ERRO] Falha ao conectar ao Wi-Fi. O dispositivo será reiniciado.");
    delay(RESTART_DELAY_MS);
    ESP.restart();
  }

  Serial.println("\n[WIFI] Conexão Wi-Fi estabelecida com sucesso!");
  Serial.print("[WIFI] Endereço IP: ");
  Serial.println(WiFi.localIP());
}

// ===================================================================================
// FUNÇÃO LOOP: Executada repetidamente após o setup.
// ===================================================================================
void loop() {
  if (millis() - lastRequestTime >= POSTING_INTERVAL_MS) {
    Serial.println("\n-----------------------------------------");
    Serial.println("[SISTEMA] Ciclo de monitoramento iniciado.");

    // --- Verificação da Conexão Wi-Fi ---
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("[WIFI] Conexão perdida. Tentando reconectar...");
      WiFi.reconnect();
      return; // Pula para a próxima iteração.
    }

    // --- 1. Realizar a Requisição HTTP GET ---
    Serial.println("[HTTP] Fazendo requisição GET...");
    Serial.print("[HTTP] URL: ");
    Serial.println(HTTP_SERVER_URL);

    http.begin(HTTP_SERVER_URL);

    // --- 2. Obter o Código de Resposta ---
    int httpResponseCode = http.GET();

    // --- 3. Verificar o Resultado e Imprimir ---
    if (httpResponseCode > 0) {
      // A requisição foi bem-sucedida
      Serial.print("[HTTP] Código de Resposta: ");
      Serial.println(httpResponseCode);

      if (httpResponseCode == HTTP_STATUS_OK) {
        String payload = http.getString();
        Serial.println("[HTTP] Conteúdo recebido (somente os 255 primeiros caracteres):");
        Serial.println(payload.substring(0, min(payload.length(), 255))); // Limita a impressão para não encher o buffer
      }
    } else {
      // A requisição falhou. Imprime o código e a mensagem de erro.
      Serial.print("[ERRO] Falha na requisição. Código: ");
      Serial.println(httpResponseCode);
      Serial.print("[ERRO] Mensagem de erro: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }

    // Encerra a conexão para liberar recursos
    http.end();

    lastRequestTime = millis();
    Serial.println("[SISTEMA] Ciclo finalizado. Aguardando próximo intervalo.");
    Serial.println("-----------------------------------------");
  }
}
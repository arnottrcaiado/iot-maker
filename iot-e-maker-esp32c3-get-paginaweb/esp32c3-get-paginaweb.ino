/*
 * Projeto: Monitoramento de Página Web com ESP32-C3
 *
 * Descrição: Este sketch demonstra como usar a biblioteca HTTPClient no ESP32-C3
 * para realizar uma requisição HTTP GET para um URL específico. O objetivo é
 * monitorar o status de um servidor web e capturar seu conteúdo.
 *
 * Hardware: ESP32-C3
 * Software: Arduino IDE
 *
 * Autor: Desenvolvedor IoT
 * Data: 27/08/2025
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include "chaves.h"

// --- Configuração do Servidor Web ---
// Use http:// ou https://
const char* serverName = "http://example.com";

void setup() {
  // Inicializa o monitor serial com uma taxa de 115200 baud
  Serial.begin(115200);

  // --- Teste de Conexão Wi-Fi ---
  Serial.print("Iniciando conexao com a rede: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Loop de espera até que a conexão seja estabelecida
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); // Espera 1 segundo
    Serial.print("."); // Imprime um ponto para cada tentativa
  }

  // --- Saída de Depuração (Conexão bem-sucedida) ---
  Serial.println("\nWi-Fi conectado com sucesso!");
  Serial.print("Endereço IP local: ");
  Serial.println(WiFi.localIP());
  Serial.println("------------------------------------");
}

void loop() {
  // --- Verificação do Status da Conexão Wi-Fi ---
  if (WiFi.status() != WL_CONNECTED) {
    // Se a conexão for perdida, tenta reconectar
    Serial.println("Wi-Fi desconectado. Tentando reconectar...");
    WiFi.begin(ssid, password);
    // Para evitar um loop infinito de reconexão
    delay(5000); 
    return; // Pula para a próxima iteração do loop
  }

  // Cria um objeto HTTPClient para a requisição
  HTTPClient http;

  // --- Teste de Requisição GET ---
  Serial.println("Fazendo requisição HTTP GET...");
  Serial.print("URL: ");
  Serial.println(serverName);

  // Inicia a requisição para o servidor especificado
  http.begin(serverName);

  // Executa a requisição GET e armazena o código de resposta
  int httpResponseCode = http.GET();

  // --- Verificação e Depuração da Resposta ---
  if (httpResponseCode > 0) {
    // A requisição foi bem-sucedida
    Serial.print("Código de Resposta HTTP: ");
    Serial.println(httpResponseCode);

    // Se o código for 200 (OK), imprime o conteúdo da página
    if (httpResponseCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("--- Conteúdo da Página Recebido ---");
      Serial.println(payload);
    }
  } else {
    // A requisição falhou. Imprime o código de erro.
    Serial.print("Erro na requisição! Código: ");
    Serial.println(httpResponseCode);
    Serial.print("Mensagem de Erro: ");
    Serial.println(http.errorToString(httpResponseCode).c_str());
  }

  // Encerra a conexão HTTP para liberar recursos
  http.end();

  Serial.println("------------------------------------");
  Serial.println("Esperando 30 segundos para a próxima requisição...");
  delay(30000); // Espera 30 segundos
}
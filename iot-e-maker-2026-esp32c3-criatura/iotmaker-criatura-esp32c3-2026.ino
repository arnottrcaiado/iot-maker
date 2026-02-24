/**
 * PROJETO: Criaturas IoT - Oficina Maker 2026
 * LÓGICA: Botão de Alternância (Toggle)
 */

#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// ======================================================================
// 1. CREDENCIAIS E IDENTIFICAÇÃO (Mudar em cada mesa)
// ======================================================================
#include <chaves.h>

// Identificador único da equipe
#define EQUIPE_ID       "equipe2" 

// ======================================================================
// 2. ENDPOINTS (Caminhos dos Feeds)
// ======================================================================
#define FEED_BOTAO_PATH  AIO_USERNAME "/feeds/iot-maker-tarde." EQUIPE_ID "-botao1"
#define FEED_LED_PATH    AIO_USERNAME "/feeds/iot-maker-tarde." EQUIPE_ID "-led1"

// ======================================================================
// 3. CONFIGURAÇÃO DE HARDWARE
// ======================================================================
const int PIN_LED   = 8;
const int PIN_BOTAO = 9;

// ======================================================================
// 4. INSTÂNCIAS E VARIÁVEIS DE ESTADO
// ======================================================================
WiFiClient client;

// O EQUIPE_ID como segundo parâmetro garante que cada placa tenha um ID MQTT único
Adafruit_MQTT_Client mqtt(&client, "io.adafruit.com", 1883, EQUIPE_ID, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish   feedBotao = Adafruit_MQTT_Publish(&mqtt, FEED_BOTAO_PATH);
Adafruit_MQTT_Subscribe feedLed   = Adafruit_MQTT_Subscribe(&mqtt, FEED_LED_PATH);

// Variáveis para a lógica de Toggle
bool statusCriatura      = false; // Estado atual da criatura (ligada/desligada)
bool ultimoEstadoLeitura = HIGH;  // Para detectar quando o botão foi apertado

void conectarMQTT();

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BOTAO, INPUT_PULLUP);

  Serial.println("\n--- CRIATURA EM MODO TOGGLE ---");
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\n[OK] WiFi Conectado!");

  mqtt.subscribe(&feedLed);
}

void loop() {
  conectarMQTT();

  // --- PARTE 1: RECEBER COMANDO DO DASHBOARD ---
  Adafruit_MQTT_Subscribe *sub;
  while ((sub = mqtt.readSubscription(50))) {
    if (sub == &feedLed) {
      String msg = (char *)feedLed.lastread;
      Serial.printf("Dashboard enviou: %s\n", msg.c_str());
      
      // Sincroniza o LED físico com o Dashboard
      if (msg == "ON" || msg == "1") digitalWrite(PIN_LED, HIGH);
      else digitalWrite(PIN_LED, LOW);
    }
  }

  // --- PARTE 2: LÓGICA DO BOTÃO (TOGGLE) ---
  bool leituraAtual = digitalRead(PIN_BOTAO);

  // Detecta a "Borda de Descida" (Quando você aperta o botão)
  if (leituraAtual == LOW && ultimoEstadoLeitura == HIGH) {
    delay(50); // Anti-ruído (Debounce)
    
    // Inverte o estado da criatura
    statusCriatura = !statusCriatura;
    
    Serial.printf("Botão clicado! Novo status: %d\n", statusCriatura);

    // Envia o novo estado para o Dashboard
    if (statusCriatura) {
      feedBotao.publish("1");
      Serial.println(">>> Enviado: 1 (Ligado)");
    } else {
      feedBotao.publish("0");
      Serial.println(">>> Enviado: 0 (Desligado)");
    }
  }

  // Salva a leitura para comparar no próximo ciclo
  ultimoEstadoLeitura = leituraAtual;

  // Manter conexão viva
  if (!mqtt.ping()) mqtt.disconnect();
}

// ======================================================================
// FUNÇÃO DE CONEXÃO (Robusta para evitar quedas)
// ======================================================================
void conectarMQTT() {
  if (mqtt.connected()) return;

  Serial.print("Conectando ao Adafruit IO...");
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    Serial.printf(" Erro: %s. Tentando novamente...\n", mqtt.connectErrorString(ret));
    mqtt.disconnect();
    delay(5000);
  }
  Serial.println(" [SUCESSO]");
}

-----

## Projeto: Estação de Monitoramento de Clima com ESP32-C3 e ThingSpeak

Este projeto demonstra como construir uma estação de monitoramento de ambiente autônoma usando uma placa **ESP32-C3** e um sensor **DHT11**. Os dados de temperatura e umidade são coletados e enviados para a plataforma **ThingSpeak** a cada 30 segundos, permitindo a visualização e análise em tempo real. A conectividade Wi-Fi é gerenciada de forma inteligente pela biblioteca **WiFiManager**, que dispensa a necessidade de alterar o código para se conectar a novas redes.

### Componentes Necessários

  * 1 x Placa de desenvolvimento **ESP32-C3**
  * 1 x Sensor de Temperatura e Umidade **DHT11**
  * Cabos Jumper
  * Protoboard (opcional)

### 1\. Montagem do Circuito

O circuito é simples e direto. Conecte o sensor DHT11 ao seu ESP32-C3 da seguinte forma:

  * **VCC do DHT11** -\> **3.3V do ESP32-C3**
  * **GND do DHT11** -\> **GND do ESP32-C3**
  * **DATA do DHT11** -\> **GPIO 4 do ESP32-C3** (Você pode usar outro GPIO, mas lembre-se de ajustar o código).

**Observação:** Alguns módulos DHT11 já vêm com um resistor pull-up embutido no pino de dados. Se o seu não tiver, adicione um resistor de **10kΩ** ( marron, preto, laranja) entre o pino DATA e o VCC para garantir leituras estáveis.

### 2\. Configuração da Plataforma ThingSpeak

O ThingSpeak é a plataforma IoT que vamos usar para receber e exibir os dados do nosso sensor. Siga os passos abaixo para configurá-lo:

1.  Acesse o site do **[ThingSpeak](https://thingspeak.com/)** e crie uma conta.
2.  No painel principal, clique em **Channels** \> **My Channels** \> **New Channel**.
3.  Preencha os campos do novo canal:
      * **Name:** `Monitoramento de Clima` (ou um nome de sua escolha).
      * **Field 1:** `Umidade`.
      * **Field 2:** `Temperatura`.
4.  Clique em **Save Channel**.
5.  Vá para a aba **API Keys** e anote as chaves que precisaremos no código:
      * **Channel ID:** O número de identificação do seu canal.
      * **Write API Key:** A chave de escrita necessária para enviar dados.

### 3\. Preparação do Ambiente de Programação

Para que o código funcione, você precisa ter o **Arduino IDE** (ou Visual Studio Code com a extensão PlatformIO) configurado para a placa ESP32. Além disso, é necessário instalar as seguintes bibliotecas:

1.  **DHT sensor library:** Para ler os dados do sensor.
      * No Arduino IDE, vá em **Ferramentas** \> **Gerenciar Bibliotecas...**, busque por `DHT sensor library` e instale a versão de Adafruit.
2.  **Adafruit Unified Sensor:** Uma dependência da biblioteca DHT.
      * Vá em **Ferramentas** \> **Gerenciar Bibliotecas...**, busque por `Adafruit Unified Sensor` e instale.
3.  **WiFiManager:** Para a configuração fácil do Wi-Fi.
      * Vá em **Ferramentas** \> **Gerenciar Bibliotecas...**, busque por `WiFiManager by tzapu` e instale.
4.  **ThingSpeak:** A biblioteca oficial para comunicação com a plataforma.
      * Vá em **Ferramentas** \> **Gerenciar Bibliotecas...**, busque por `ThingSpeak` e instale a versão de MathWorks.

### 4\. Código do Projeto

Copie o código abaixo e cole no seu Arduino IDE. **Lembre-se de substituir `SEU_CANAL_ID` e `"SUA_WRITE_API_KEY"`** com as chaves que você anotou no passo 2.

```cpp
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

#define THINGSPEAK_FIELD_HUMI 1          // Campo do canal ThingSpeak para enviar a umidade.
#define THINGSPEAK_FIELD_TEMP 2          // Campo do canal ThingSpeak para enviar a temperatura.

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
```

### 5\. Como o Código Funciona

  * **`WiFiManager`**: A biblioteca simplifica a conexão à rede Wi-Fi. Na primeira vez que você ligar o ESP32, ele criará um ponto de acesso chamado **ESP32-C3-SETUP**. Basta se conectar a ele com seu celular ou computador e seguir as instruções na página para inserir as credenciais da sua rede. O ESP32 armazena essas informações e se conectará automaticamente nas próximas vezes.
  * **`setup()`**: Inicia a comunicação serial para debug, o sensor DHT e a biblioteca **ThingSpeak**. A mágica do **WiFiManager** acontece aqui, garantindo que o dispositivo esteja conectado antes de qualquer outra ação.
  * **`loop()`**: Esta função principal verifica se o intervalo de 30 segundos já passou. Se sim, ela lê os dados do sensor, imprime-os na Serial para que você possa monitorar, e então chama a função `sendDataToThingSpeak`.
  * **`sendDataToThingSpeak()`**: É a função que faz a comunicação com o ThingSpeak. Ela usa os comandos `ThingSpeak.setField()` para associar os valores de temperatura e umidade aos campos 1 e 2 do seu canal. Finalmente, `ThingSpeak.writeFields()` envia todos os dados de uma vez. O código de resposta é impresso na Serial para fácil diagnóstico.

Com este projeto, você terá uma base para criar outros projetos IoT, compreendendo o fluxo completo, desde a leitura de um sensor até o envio e visualização de dados em uma plataforma na nuvem.


# 📚 Guia Base: Projetos Maker com Arduino e ESP32-C3

Bem-vindo ao guia de tutoriais progressivos, elaborado para Makers, estudantes e entusiastas de IoT. Este documento serve como uma ponte de aprendizado, transitando do controle digital básico com o **Arduino UNO** para as avançadas funcionalidades de IoT da **ESP32-C3**.

A **ESP32-C3** é uma MCU (Microcontroller Unit) poderosa e econômica que se destaca por sua conectividade Wi-Fi e Bluetooth. Todos os códigos utilizam a estrutura C/C++ do Arduino Core, focando em código limpo, documentado e totalmente compatível com a formatação Markdown.

Os tutoriais estão organizados em níveis, com um novo projeto de **Entrada/Saída Digital** adicionado ao nível Iniciante/Básico.

-----

## 1️⃣ Nível Iniciante: Piscar um LED (Blink)

### **Funcionalidade do Projeto**

O "Blink" é o exercício fundamental para entender o **Controle Digital**, usando a função `digitalWrite()` para aplicar ou remover tensão em um pino de saída (`HIGH`/`LOW`) e a função `delay()` para gerenciar o tempo.

### **Fiação Comum (Ambos)**

  * LED anodo (perna longa) **liga em** Resistor 220 Ohm
  * Resistor **liga em** Pino Digital (Pino 11 no Arduino, GPIO 2 no C3)
  * LED catodo (perna curta) **liga em** GND (Terra)

### **1.1. Código para Arduino UNO (Pino 11)**

```c
// Tutorial 1.1: Piscar LED (Blink) com Arduino UNO

// Ligações: LED (+) -> Resistor 220 Ohm -> Pino 11
//           LED (-) -> GND

// Define o pino onde o LED está conectado
const int ledPin = 11; 

void setup() {
  Serial.begin(9600);
  // Configura o pino 11 como SAÍDA (OUTPUT) para que possa enviar tensão (5V).
  pinMode(ledPin, OUTPUT);
  Serial.println("Arduino UNO: Blink iniciado.");
}

void loop() {
  // 1. Liga o LED (HIGH = 5V)
  digitalWrite(ledPin, HIGH); 
  Serial.println("LED: LIGADO");
  
  // Aguarda 1000 milissegundos (1 segundo)
  delay(1000);                      
  
  // 2. Apaga o LED (LOW = 0V)
  digitalWrite(ledPin, LOW);    
  Serial.println("LED: DESLIGADO");
  
  // Aguarda 1000 milissegundos (1 segundo)
  delay(1000);                      
}
```

### **1.2. Adaptação para ESP32-C3 (GPIO 2)**

```c
// Tutorial 1.2: Piscar LED (Blink) com ESP32-C3

// Ligações: LED (+) -> Resistor 220 Ohm -> GPIO 2
//           LED (-) -> GND
// A ESP32-C3 usa lógica de 3.3V, mas a função do código é a mesma.

// Define o pino onde o LED está conectado
const int ledPin = 2; 

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200); // Taxa de comunicação padrão mais rápida para ESP32
  Serial.println("ESP32-C3: Blink iniciado.");
}

void loop() {
  // Liga o LED (HIGH = 3.3V na ESP32-C3)
  digitalWrite(ledPin, HIGH); 
  Serial.println("C3 LED: LIGADO");
  delay(500); 
  
  // Apaga o LED (LOW = 0V)
  digitalWrite(ledPin, LOW);    
  Serial.println("C3 LED: DESLIGADO");
  delay(500);
}
```

-----

## 2️⃣ Nível Iniciante/Básico: Botão (Pull-up) e LEDs

### **Funcionalidade do Projeto**

Este projeto essencial foca na **Leitura de Entrada Digital** usando um botão configurado com **Pull-up Interno**. Esta configuração simplifica a fiação na protoboard, usando apenas o botão para conectar o pino ao GND. Um LED é controlado pelo estado do botão.

### **Fiação (Ambos)**

| Componente | Arduino UNO | ESP32-C3 |
| :---: | :---: | :---: |
| **LED** | **D12** (com Resistor para GND) | **GPIO 2** (com Resistor para GND) |
| **Botão (Lado 1)** | **D2** (configurado como INPUT\_PULLUP) | **GPIO 5** (configurado como INPUT\_PULLUP) |
| **Botão (Lado 2)** | **GND** | **GND** |

### **2.1. Código para Arduino UNO (D2 e D12)**

```c
// Tutorial 2.1: Botao Pull-up com Arduino UNO

// Ligações:
// Botao: Pino D2 -> Botao Lado 1. Botao Lado 2 -> GND. (Usa PULLUP INTERNO)
// LED: Pino D12 -> Resistor 220 Ohm -> GND.

const int buttonPin = 2; // Pino para o botao
const int ledPin = 12;   // Pino para o LED

void setup() {
  Serial.begin(9600);
  // Define o pino do LED como SAIDA
  pinMode(ledPin, OUTPUT);
  
  // *** CONFIGURACAO PULL-UP ***
  // Define o pino do botao como ENTRADA, mas ativa o resistor PULL-UP INTERNO.
  // O pino sera lido como HIGH (ativo) por padrao.
  // Ao pressionar o botao, o pino e conectado ao GND, lendo LOW.
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.println("Arduino UNO: Teste de Botao Pull-up.");
}

void loop() {
  // A leitura do botao e INVERTIDA devido ao pull-up
  int buttonState = digitalRead(buttonPin);
  
  if (buttonState == LOW) {
    // Se o botao for pressionado (conectado ao GND), o estado e LOW.
    digitalWrite(ledPin, HIGH);
    Serial.println("Botao: PRESSIONADO. LED LIGADO.");
  } else {
    // Se o botao nao for pressionado, o resistor pull-up mantem o estado HIGH.
    digitalWrite(ledPin, LOW);
    Serial.println("Botao: SOLTO. LED DESLIGADO.");
  }
  
  delay(50); // Pequeno atraso para estabilizacao
}
```

### **2.2. Adaptação para ESP32-C3 (GPIO 5 e 2)**

```c
// Tutorial 2.2: Botao Pull-up com ESP32-C3

// Ligações:
// Botao: GPIO 5 -> Botao Lado 1. Botao Lado 2 -> GND. (Usa PULLUP INTERNO)
// LED: GPIO 2 -> Resistor 220 Ohm -> GND.

const int buttonPin = 5; // GPIO 5 para o botao
const int ledPin = 2;    // GPIO 2 para o LED

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  
  // *** CONFIGURACAO PULL-UP NA C3 ***
  // O ESP32-C3 tambem suporta INPUT_PULLUP. A logica de uso e a mesma.
  pinMode(buttonPin, INPUT_PULLUP); 
  Serial.println("ESP32-C3: Teste de Botao Pull-up.");
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  
  if (buttonState == LOW) {
    // Botao pressionado (LOW)
    digitalWrite(ledPin, HIGH);
    Serial.println("C3 Botao: PRESSIONADO. LED LIGADO.");
  } else {
    // Botao solto (HIGH)
    digitalWrite(ledPin, LOW);
    Serial.println("C3 Botao: SOLTO. LED DESLIGADO.");
  }
  
  delay(50);
}
```

-----

## 3️⃣ Nível Básico: Sensor Ultrassônico (HC-SR04)

### **Funcionalidade do Projeto**

Este projeto realiza a **Leitura de Sensores** para medir distâncias. O sensor HC-SR04 mede o tempo de um pulso sonoro de ida e volta, e o código converte esse tempo em centímetros utilizando a fórmula da velocidade do som.

### **Fiação HC-SR04**

| Pino HC-SR04 | Arduino UNO | ESP32-C3 |
| :---: | :---: | :---: |
| **VCC** | **5V** | **5V ou 3.3V** |
| **GND** | **GND** | **GND** |
| **Trig (Saída)** | **Pino Digital (ex: 9)** | **GPIO 5** |
| **Echo (Entrada)** | **Pino Digital (ex: 10)** | **GPIO 4 (COM DIVISOR DE TENSÃO)** |

> **IMPORTANTE para ESP32-C3:** O pino **Echo** do HC-SR04 emite **5V**. A C3 aceita apenas **3.3V**. É **OBRIGATÓRIO** usar um **divisor de tensão** no pino Echo para proteger a placa.

### **3.1. Arduino UNO (Pinos 9 e 10)**

```c
// Tutorial 3.1: Sensor Ultrassonico HC-SR04 com Arduino UNO

// Ligações:
// HC-SR04 VCC -> Arduino 5V
// HC-SR04 GND -> Arduino GND
// HC-SR04 Trig (OUTPUT) -> Arduino Pino 9
// HC-SR04 Echo (INPUT) -> Arduino Pino 10

const int trigPin = 9; 
const int echoPin = 10;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.println("Arduino UNO: Medindo distancia...");
}

void loop() {
  // 1. Geração do pulso de Trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // 2. Medir o tempo de retorno (Duração em microssegundos)
  long duration = pulseIn(echoPin, HIGH);
  
  // 3. Cálculo da Distância em centímetros (cm)
  // Distancia = (Tempo de ida e volta / 2) * Velocidade do Som (0.0343 cm/µs)
  int distanceCm = duration * 0.0343 / 2;
  
  // Monitoramento Serial
  Serial.print("Distancia: ");
  Serial.print(distanceCm);
  Serial.println(" cm");
  
  delay(1000); 
}
```

### **3.2. ESP32-C3 (GPIO 5 e 4)**

```c
// Tutorial 3.2: Sensor Ultrassonico HC-SR04 com ESP32-C3

// Ligações:
// HC-SR04 VCC -> C3 5V ou 3.3V
// HC-SR04 GND -> C3 GND
// HC-SR04 Trig -> C3 GPIO 5 
// HC-SR04 Echo -> C3 GPIO 4 (ATENCAO: Use Divisor de Tensao 5V->3.3V!)

const int trigPin = 5; 
const int echoPin = 4;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.println("ESP32-C3: Medindo distancia (Verifique o Divisor de Tensao no Echo!)");
}

void loop() {
  // A logica do codigo e identica, usando as funcoes universais.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  int distanceCm = duration * 0.0343 / 2;
  
  // Monitoramento Serial
  Serial.print("C3 Distancia: ");
  Serial.print(distanceCm);
  Serial.println(" cm");
  
  delay(1000); 
}
```

-----

## 4️⃣ Nível Intermediário: Leitura RFID (RC522)

### **Funcionalidade do Projeto**

Este projeto utiliza o protocolo **SPI (Serial Peripheral Interface)** para ler o **UID (Unique Identifier)** de uma tag RFID através do módulo RC522. A complexidade está em gerenciar a comunicação SPI, que exige a definição de múltiplos pinos. O código exibe o UID lido no Monitor Serial.

### **Fiação RC522 (Comunicação SPI)**

| Pino RC522 | Arduino UNO (SPI Fixo) | ESP32-C3 (Exemplo GPIOs) |
| :---: | :---: | :---: |
| **VCC** | **3.3V** | **3.3V** |
| **GND** | **GND** | **GND** |
| **RST** | **D9** | **GPIO 2** |
| **SDA (SS)** | **D10** | **GPIO 3** |
| **MOSI** | **D11** | **GPIO 6** |
| **MISO** | **D12** | **GPIO 7** |
| **SCK** | **D13** | **GPIO 5** |

### **Bibliotecas (Ambos):**

  * **SPI.h**
  * **MFRC522.h** (Instale via Gerenciador de Bibliotecas)

### **4.1. Arduino UNO**

```c
// Tutorial 4.1: Leitura RFID RC522 com Arduino UNO
// Ligas: RC522 VCC->3.3V, GND->GND. RST->D9, SDA(SS)->D10, MOSI->D11, MISO->D12, SCK->D13
#include <SPI.h>       
#include <MFRC522.h>   

// Define os pinos de controle
#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN); 

void setup() {
  Serial.begin(9600);   
  SPI.begin();          // Inicializa a comunicação SPI nos pinos padroes do UNO
  mfrc522.PCD_Init();   // Inicializa o leitor RC522
  
  Serial.println("Arduino UNO RFID: Aproxime a Tag/Cartao para ler o UID.");
}

void loop() {
  // Verifica se ha nova tag
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Seleciona a tag
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // --- Leitura e Exibição do UID ---
  Serial.print("UID Lido (HEX): ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    // Formatacao: imprime em hexadecimal com zero inicial
    if (mfrc522.uid.uidByte[i] < 0x10) {
        Serial.print(" 0"); 
    } else {
        Serial.print(" ");
    }
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  
  mfrc522.PICC_HaltA(); 
  delay(500);
}
```

### **4.2. ESP32-C3**

```c
// Tutorial 4.2: Leitura RFID RC522 com ESP32-C3 (SPI Personalizado)
// Ligas: RC522 VCC->3.3V, GND->GND. 
// PINOS C3: RST->GPIO 2, SDA(SS)->GPIO 3, MOSI->GPIO 6, MISO->GPIO 7, SCK->GPIO 5
#include <SPI.h>       
#include <MFRC522.h>   

// Define os pinos de controle
#define SS_PIN  3  // GPIO 3 (Slave Select)
#define RST_PIN 2  // GPIO 2 (Reset)

MFRC522 mfrc522(SS_PIN, RST_PIN); 

void setup() {
  Serial.begin(115200);   
  
  // *** Configuração SPI Específica para ESP32-C3 ***
  // SPI.begin(SCK, MISO, MOSI)
  // Define quais GPIOs serao usados para as linhas SPI.
  SPI.begin(5, 7, 6); // SCK=GPIO 5, MISO=GPIO 7, MOSI=GPIO 6
  
  mfrc522.PCD_Init();   
  
  Serial.println("ESP32-C3 RFID: Aproxime a Tag/Cartao. (Pinagem SPI customizada!)");
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // --- Leitura do UID (Logica identica ao Arduino) ---
  Serial.print("C3 UID Lido (HEX): ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
        Serial.print(" 0"); 
    } else {
        Serial.print(" ");
    }
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  
  mfrc522.PICC_HaltA(); 
  delay(1000);
}
```

-----

## 5️⃣ Nível Avançado (IoT): DHT11 para ThingSpeak

### **Funcionalidade do Projeto**

O projeto final integra **hardware, rede Wi-Fi e serviços de nuvem (IoT)**. Ele utiliza o ESP32-C3 para ler temperatura e umidade do DHT11 e enviar esses dados para a plataforma **ThingSpeak** através de uma API, permitindo a visualização e análise remota.

### **Fiação DHT11 (Apenas ESP32-C3)**

  * DHT11 VCC **liga em** **3.3V** do ESP32-C3
  * DHT11 GND **liga em** **GND** do ESP32-C3
  * DHT11 Data **liga em** **GPIO 4** do ESP32-C3

### **Pré-requisitos:**

1.  **Bibliotecas:** `WiFi.h`, `ThingSpeak.h` (MathWorks), `DHT.h` (Adafruit).
2.  **ThingSpeak:** Canal criado com Field 1 (Temperatura) e Field 2 (Umidade), e obtenção do **Channel ID** e da **Write API Key**.

### **5.1. Código para ESP32-C3 (IoT)**

```c
// Tutorial 5.1: Envio de DHT11 para ThingSpeak com ESP32-C3
// Ligações: DHT11 VCC->3.3V, GND->GND, Data->GPIO 4

#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

// --- Configurações de Rede e ThingSpeak (Atualize estas linhas!) ---
const char* ssid     = "SEU_WIFI_SSID";          
const char* password = "SUA_SENHA_WIFI";       

unsigned long myChannelNumber = 000000;         // Seu Channel ID
const char * myWriteAPIKey = "SUA_API_KEY";     // Sua Write API Key

// --- Configurações do Sensor DHT11 ---
#define DHTPIN 4         // Pino GPIO 4
#define DHTTYPE DHT11    // Tipo de sensor

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client; 

const long sendInterval = 25000; // Intervalo de envio (25 segundos)
unsigned long lastSendTime = 0;


void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // 1. Conexão Wi-Fi
  Serial.print("Conectando-se a: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // 2. Inicialização do ThingSpeak
  ThingSpeak.begin(client);
}


void loop() {
  // Executa o envio apenas quando o intervalo de tempo for atingido
  if(millis() - lastSendTime > sendInterval) {
    
    // Leitura da Umidade e Temperatura
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    // Verifica se a leitura falhou 
    if (isnan(h) || isnan(t)) {
      Serial.println("ERRO: Falha na leitura do sensor DHT!");
      return;
    }
    
    // 3. Preparação e Envio dos Dados para ThingSpeak
    
    // Configura o Campo 1 com a Temperatura
    ThingSpeak.setField(1, t);
    
    // Configura o Campo 2 com a Umidade
    ThingSpeak.setField(2, h);
    
    // Grava os dados no Canal ThingSpeak
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if(x == 200){
      Serial.print("ThingSpeak OK! Envio concluido. T: ");
      Serial.print(t);
      Serial.print(" C | H: ");
      Serial.print(h);
      Serial.println(" %");
    }
    else{
      Serial.print("ERRO ThingSpeak. Codigo HTTP: ");
      Serial.println(x);
    }
    
    lastSendTime = millis(); 
  }
}
```

-----

## 🔗 Links e Referências de Projetos

| Tutorial | Tema | Referência Válida (em Inglês/Espanhol) |
| :---: | :---: | :---: |
| **1 (LED)** | Blink Básico | [Arduino Blink Example (Official)](https://docs.arduino.cc/built-in-examples/basics/Blink/) |
| **2 (Botão)** | Entrada Digital Pull-up | [Arduino Button Example (Official)](https://docs.arduino.cc/built-in-examples/digital/Button/) |
| **3 (Ultrassom)** | HC-SR04 e Divisor | [ESP32 HC-SR04 with Arduino IDE (RNT)](https://www.google.com/search?q=https://randomnerdtutorials.com/esp32-hc-sr04-ultrasonic-sensor-arduino-ide/) |
| **4 (RFID)** | RC522 com SPI | [ESP32 with MFRC522 RFID Reader/Writer (RNT)](https://randomnerdtutorials.com/esp32-mfrc522-rfid-reader-arduino/) |
| **5 (IoT)** | DHT11 para ThingSpeak | [ESP32 Publish Sensor Readings to ThingSpeak (RNT)](https://randomnerdtutorials.com/esp32-thingspeak-publish-arduino/) |

-----
ls
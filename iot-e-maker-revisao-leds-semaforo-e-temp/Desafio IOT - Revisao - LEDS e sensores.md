# Desafio IoT & Maker: Fundamentos Elétricos e Iluminação Inteligente com Arduino

**Público-alvo:** Alunos de Análise e Desenvolvimento de Sistemas.
**Duração Máxima Sugerida:** 100 minutos.

**Objetivo Geral:** Revisar e consolidar princípios elétricos básicos (tensão, corrente, resistência), ligações em protoboard, e aplicar esses conceitos em projetos de IoT com Arduino e componentes fundamentais (LEDs e, opcionalmente, sensores).

**Metodologia:** Aprendizagem baseada em Desafios (Challenge-Based Learning) e Tutorial Guiado.

## 1. Materiais Necessários (por grupo/aluno)

| Item | Quantidade | Notas |
| :--- | :--- | :--- |
| Placa Arduino UNO (ou similar) | 1 | |
| Protoboard (mini ou média) | 1 | |
| LED Vermelho | 1 | Desafio Nível 1 |
| LED Verde | 1 | Desafio Nível 2 |
| LED Amarelo | 1 | Desafio Nível 2 |
| Resistor 220 ohms a 330 ohms) | 3 | Essencial para proteger os LEDs\! |
| Jumpers (Fios) | 8-10 | Diversas cores (opcional, mas recomendado) |
| **Opcional 1:** Sensor de Distância Ultrassônico (HC-SR04) | 1 | Desafio Nível 3 (Extra) |
| **Opcional 2:** Sensor de Temperatura e Umidade (DHT11/DHT22) | 1 | Desafio Nível 3 (Extra) |
| Cabo USB para Arduino | 1 | |

-----

## 2. Desafios Progressivos

O desafio é dividido em três níveis, aumentando a complexidade e aplicando diferentes conceitos.

### Nível 1: O LED Solitário (Revisão Fundamentos)

| Conceitos Chave | Duração Estimada |
| :--- | :--- |
| Tensão, Corrente, Resistência (Lei de Ohm) | 20 min |
| Anodo e Catodo do LED | |
| Saídas Digitais (HIGH/LOW) | |
| `setup()` e `loop()` do Arduino | |

#### **Desafio 1.1:** Acender e Proteger

Ligue um LED vermelho na protoboard e ao Arduino. Calcule e utilize o resistor correto para **proteger o LED**, ligando-o à porta digital **13**. O LED deve **permanecer aceso** continuamente.

#### **Desafio 1.2:** Pisca-Pisca (Blink)

Modifique o código para que o LED vermelho pisque a cada $1$ segundo (fique aceso por 1000ms e apagado por 1000ms).

### Nível 2: O Semáforo Simples (Saídas Múltiplas e Lógica)

| Conceitos Chave | Duração Estimada |
| :--- | :--- |
| Uso de Múltiplas Saídas Digitais | 35 min |
| Variáveis e Constantes em *Clean Code* | |
| Estruturas de Controle de Fluxo (`delay()`) | |
| Mapeamento de Pinos | |

#### **Desafio 2.1:** Semáforo Básico

Ligue mais dois LEDs (verde e amarelo) nas portas digitais **$12$** e **$11$**, respectivamente. O código deve simular o ciclo de um semáforo (Verde -> Amarelo -> Vermelho -> Verde...), com a seguinte temporização:

1.  **Verde:** 4 segundos
2.  **Amarelo:** 1$ segundo
3.  **Vermelho:** 5 segundos

### Nível 3: Opcional com Sensores (Entradas e Lógica Condicional)

| Conceitos Chave | Duração Estimada |
| :--- | :--- |
| Uso de Entradas Analógicas ou Digitais (Sensores) | 45 min |
| Bibliotecas | |
| Comunicação Serial (Debug) | |
| Estruturas Condicionais (`if/else`) | |

#### **Desafio 3.1 (A - Distância):** Sinalização de Proximidade

  * **Componente:** Sensor Ultrassônico HC-SR04.
  * **Lógica:** Use o sensor para medir a distância. O LED Vermelho deve **piscar** se a distância medida for **menor que 20cm**. Caso contrário, o LED Verde deve permanecer **aceso**.

#### **Desafio 3.2 (B - Temperatura):** Alerta de Temperatura

  * **Componente:** Sensor de Temperatura e Umidade (DHT11/DHT22).
  * **Lógica:** Leia a temperatura ambiente. O LED Vermelho deve acender se a temperatura for **superior a 28 graus C** (Alerta de Calor). Caso contrário, o LED Verde deve permanecer aceso. **(Requer instalação de biblioteca)**

-----

## 3. Tutorial Guiado: Nível 1 (O LED Solitário)

### **Conceito Chave: O Resistor Protetor**

Um LED (Diodo Emissor de Luz) é sensível à corrente. Se aplicarmos os 5V da porta do Arduino diretamente, a corrente será muito alta e o LED irá queimar. Usamos a **Lei de Ohm (R = V/I)** para calcular o resistor limitador de corrente:

  * $V_{Fonte} aprox 5V (Arduino)
  * $V_{LED} aprox 2V (Tensão de operação do LED Vermelho)
  * $I_{Desejada} aprox 20mA ou 0.02A (Corrente segura para o LED)

{Tensão no Resistor } (V_R) = V_{Fonte} - V_{LED} 
-> V_R = 5V - 2V = 3V

{Resistência } (R) = V_R / I_{Desejada} 
-> R = 3V / 0.02A = 150 ohms

**Regra Prática:** Um resistor entre 220 ohms e 330 ohms é seguro e muito comum para LEDs com Arduino. **Utilizaremos 220 ohms ou maior.**

### **Ligações (Hardware)**

1.  Conecte o **Catodo (perna mais curta)** do LED à linha de **GND (Terra)** da protoboard.
2.  Conecte um resistor de 220 ohms entre a linha **GND** e um ponto no protoboard.
3.  Conecte o **Anodo (perna mais longa)** do LED a um pino digital do Arduino, como o **13**.
4.  Conecte o pino **GND** do Arduino à linha de **GND** da protoboard.

### **Código (Software) - Desafio 1.2 (Blink) com Clean Code**

```arduino
/*
  Projeto: Desafio Nível 1 - Pisca-Pisca (Blink)
  Objetivo: Acender e apagar o LED a cada 1 segundo.
*/

// 1. CONSTANTES GLOBAIS
const int LED_VERMELHO = 13; 
const int TEMPO_ACESO = 1000;  // 1 segundo
const int TEMPO_APAGADO = 1000; // 1 segundo


// 2. FUNÇÃO SETUP
void setup() {
  // Configura o pino 13 como saída
  pinMode(LED_VERMELHO, OUTPUT);
}


// 3. FUNÇÃO LOOP
void loop() {
  // LIGAR
  digitalWrite(LED_VERMELHO, HIGH);
  delay(TEMPO_ACESO); 

  // DESLIGAR
  digitalWrite(LED_VERMELHO, LOW);
  delay(TEMPO_APAGADO); 
}
```

-----

## 4. Tutorial Guiado: Nível 2 (Semáforo Simples)

### **Conceito Chave: Mapeamento de Pinos**

| LED | Pino (Arduino) |
| :--- | :--- |
| Verde | 12 |
| Amarelo | 11 |
| Vermelho | 10 |

### **Ligações (Hardware)**

1.  Adicione o LED Amarelo no pino 11 com seu próprio resistor (220 ohms).
2.  Adicione o LED Verde no pino 12 com seu próprio resistor (220 ohms).
3.  **Todos os catodos** (pernas curtas) dos LEDs devem ser ligados à linha de **GND** da protoboard (linha Terra).

### **Código (Software) - Desafio 2.1 (Semáforo) com Clean Code**

```arduino
/*
  Projeto: Desafio Nível 2 - Semáforo Simples
  Objetivo: Simular o ciclo de um semáforo com temporização correta.
*/

// 1. CONSTANTES DE PINOS
const int PINO_VERDE = 12;
const int PINO_AMARELO = 11;
const int PINO_VERMELHO = 10;

// 2. CONSTANTES DE TEMPO (em milissegundos)
const int TEMPO_VERDE = 4000; // 4 segundos
const int TEMPO_AMARELO = 1000; // 1 segundo
const int TEMPO_VERMELHO = 5000; // 5 segundos


// 3. FUNÇÃO SETUP
void setup() {
  // Configurar todos os pinos como OUTPUT
  pinMode(PINO_VERDE, OUTPUT);
  pinMode(PINO_AMARELO, OUTPUT);
  pinMode(PINO_VERMELHO, OUTPUT);
}


// 4. FUNÇÕES DE ESTADOS (Clean Code: encapsulamento da lógica de cada estado)
void estadoVerde() {
  // Apenas o LED Verde acende
  digitalWrite(PINO_VERDE, HIGH);
  digitalWrite(PINO_AMARELO, LOW);
  digitalWrite(PINO_VERMELHO, LOW);
  delay(TEMPO_VERDE);
}

void estadoAmarelo() {
  // Apenas o LED Amarelo acende
  digitalWrite(PINO_VERDE, LOW);
  digitalWrite(PINO_AMARELO, HIGH);
  digitalWrite(PINO_VERMELHO, LOW);
  delay(TEMPO_AMARELO);
}

void estadoVermelho() {
  // Apenas o LED Vermelho acende
  digitalWrite(PINO_VERDE, LOW);
  digitalWrite(PINO_AMARELO, LOW);
  digitalWrite(PINO_VERMELHO, HIGH);
  delay(TEMPO_VERMELHO);
}


// 5. FUNÇÃO LOOP
void loop() {
  // A sequência do semáforo
  estadoVerde();
  estadoAmarelo();
  estadoVermelho();
}
```

-----

## 5. Tutorial Guiado: Nível 3 (Desafios Opcionais com Sensores)

### Desafio 3.1 (A - Distância): Sinalização de Proximidade (HC-SR04)

#### **Conceitos Chave: Ultrassom e Cálculo de Distância**

O sensor ultrassônico HC-SR04 mede a distância emitindo uma onda sonora e medindo o tempo que ela leva para retornar (eco). A função `pulseIn()` do Arduino é usada para medir esse tempo (duração).

Distância (cm) = Duração (microsegundos) / 58

Onde 58 é o fator de conversão do tempo de ida e volta para centímetros, baseado na velocidade do som.

#### **Ligações (Hardware)**

| Pino do Sensor | Pino do Arduino | Função |
| :--- | :--- | :--- |
| **VCC** | 5V | Alimentação |
| **GND** | GND | Terra |
| **Trig** | 9 | Saída (Envia o pulso ultrassônico) |
| **Echo** | 8 | Entrada (Recebe o pulso de retorno) |
| **LED Vermelho** | 13 (com resistor 220 ohms) | Sinalização de Alerta |
| **LED Verde** | 12 (com resistor 220 ohms) | Sinalização de OK |

#### **Código (Software) - Desafio 3.1 com Clean Code**

```arduino
/*
  Projeto: Desafio Nível 3A - Sinalização de Proximidade
  Objetivo: Acender LED Vermelho (pisca) ou Verde (fixo) baseado na distância.
*/

// 1. CONSTANTES DE PINOS
const int PINO_TRIGGER = 9;   // Pino de envio de som (Trigger)
const int PINO_ECHO = 8;      // Pino de recepção de som (Echo)
const int LED_ALERTA = 13;    // LED Vermelho (Alerta)
const int LED_OK = 12;        // LED Verde (Distância OK)

// 2. CONSTANTES DE LÓGICA
const int DISTANCIA_LIMITE = 20; // Limite de 20 cm para alerta

// 3. VARIÁVEIS GLOBAIS
long duracao; // Variável para armazenar a duração do pulso
int distanciaCm; // Variável para armazenar a distância em cm


// 4. FUNÇÃO SETUP
void setup() {
  pinMode(PINO_TRIGGER, OUTPUT);
  pinMode(PINO_ECHO, INPUT);
  pinMode(LED_ALERTA, OUTPUT);
  pinMode(LED_OK, OUTPUT);

  // Inicializa a comunicação serial para debug (Monitor Serial)
  Serial.begin(9600);
}


// 5. FUNÇÃO DE CÁLCULO (Encapsula a lógica do sensor)
int calcularDistancia() {
  // Limpa o Trigger, garantindo um pulso limpo.
  digitalWrite(PINO_TRIGGER, LOW);
  delayMicroseconds(2); 

  // Envia o pulso de 10us para iniciar a medição.
  digitalWrite(PINO_TRIGGER, HIGH);
  delayMicroseconds(10); 
  digitalWrite(PINO_TRIGGER, LOW);

  // Lê o tempo do Echo (tempo que o som demorou para ir e voltar).
  duracao = pulseIn(PINO_ECHO, HIGH);

  // Converte a duração para distância em centímetros.
  distanciaCm = duracao * 0.034 / 2; // (Velocidade do Som / 2)
  // Ou usando o fator simplificado (Duração / 58)
  // distanciaCm = duracao / 58;
  
  return distanciaCm;
}


// 6. FUNÇÃO LOOP
void loop() {
  int d = calcularDistancia(); // Realiza a medição

  Serial.print("Distância: ");
  Serial.print(d);
  Serial.println(" cm");

  // Lógica de controle do LED
  if (d < DISTANCIA_LIMITE && d > 0) {
    // ALERTA (Pisca o Vermelho, apaga o Verde)
    digitalWrite(LED_OK, LOW);
    digitalWrite(LED_ALERTA, HIGH);
    delay(200); // Pisca rápido
    digitalWrite(LED_ALERTA, LOW);
    delay(200);
  } else {
    // OK (Acende o Verde, apaga o Vermelho)
    digitalWrite(LED_ALERTA, LOW);
    digitalWrite(LED_OK, HIGH);
  }

  delay(50); // Pequeno delay para estabilidade de leitura
}
```

### Desafio 3.2 (B - Temperatura): Alerta de Temperatura (DHT11/DHT22)

#### **Conceitos Chave: Sensor Digital e Bibliotecas**

Sensores complexos como o DHT (que mede Temperatura e Umidade) requerem uma **biblioteca** para interpretar o sinal digital enviado. O uso de bibliotecas simplifica o código, transformando a leitura de um sinal bruto em valores prontos (como 25.5 graus C).

**Passos para os alunos (Instalação da Biblioteca):**

1.  No IDE do Arduino, vá em **Sketch \> Incluir Biblioteca \> Gerenciar Bibliotecas...**
2.  Procure por **"DHT sensor library"** (de Adafruit) e instale.
3.  Procure por **"Adafruit Unified Sensor"** e instale (é uma dependência).

#### **Ligações (Hardware)**

| Pino do Sensor (DHT) | Pino do Arduino | Função |
| :--- | :--- | :--- |
| **VCC** | 5V | Alimentação |
| **GND** | GND | Terra |
| **Data (Dados)** | 7 | Saída de Dados (Geralmente requer um resistor Pull-up de 10k ohms para 5V, mas costuma funcionar sem) |
| **LED Vermelho** | 13 (com resistor 220 ohms) | Alerta de Calor |
| **LED Verde** | 12 (com resistor 220 ohms) | Temperatura OK |

#### **Código (Software) - Desafio 3.2 com Clean Code**

```arduino
/*
  Projeto: Desafio Nível 3B - Alerta de Temperatura
  Objetivo: Acender LED Vermelho (Alerta) ou Verde (OK) baseado na temperatura.
  REQUER: Biblioteca Adafruit DHT Sensor e Adafruit Unified Sensor.
*/

// 1. INCLUSÃO DE BIBLIOTECAS
#include <DHT.h> 

// 2. CONSTANTES DE PINOS
const int PINO_DHT = 7;     // Pino de dados do DHT
const int LED_ALERTA = 13;  // LED Vermelho (Acima do limite)
const int LED_OK = 12;      // LED Verde (Abaixo do limite)

// 3. CONSTANTES DE LÓGICA E TIPO DE SENSOR
#define TIPO_SENSOR DHT11     // Defina o modelo do seu sensor (DHT11 ou DHT22)
const float TEMPERATURA_LIMITE = 28.0; // Limite de 28.0 graus Celsius para alerta

// 4. INSTANCIAÇÃO DO OBJETO DHT
DHT dht(PINO_DHT, TIPO_SENSOR);


// 5. FUNÇÃO SETUP
void setup() {
  pinMode(LED_ALERTA, OUTPUT);
  pinMode(LED_OK, OUTPUT);

  Serial.begin(9600);
  Serial.println("Inicializando Sensor DHT...");

  // Inicia a leitura do sensor
  dht.begin(); 
}


// 6. FUNÇÃO LOOP
void loop() {
  // O DHT tem um tempo de resposta lento. Uma leitura a cada 2 segundos é ideal.
  delay(2000); 

  // Lê a temperatura em Celsius. O 't' é um float.
  float t = dht.readTemperature();

  // Verifica se a leitura falhou (muitas vezes retorna NaN - Not a Number).
  if (isnan(t)) {
    Serial.println("Falha ao ler o sensor DHT!");
    return; // Pula o restante do loop
  }

  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" *C");

  // Lógica de controle do LED
  if (t > TEMPERATURA_LIMITE) {
    // ALERTA (Acende o Vermelho)
    digitalWrite(LED_OK, LOW);
    digitalWrite(LED_ALERTA, HIGH);
    Serial.println("ALERTA: Temperatura acima do limite!");
  } else {
    // OK (Acende o Verde)
    digitalWrite(LED_ALERTA, LOW);
    digitalWrite(LED_OK, HIGH);
    Serial.println("Temperatura OK.");
  }
}
```
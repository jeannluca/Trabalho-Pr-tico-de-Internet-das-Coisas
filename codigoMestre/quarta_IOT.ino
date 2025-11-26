/* --- CÓDIGO MESTRE ESP32S (COMUNICAÇÃO BLYNK + SERIAL) --- */

// 1. Definição das Credenciais e Bibliotecas
#define BLYNK_TEMPLATE_ID "TMPL2tQaKSBwJ"
#define BLYNK_TEMPLATE_NAME "Jefferson Teles"
#define BLYNK_AUTH_TOKEN "QWwpZsAXr5cyGtdawNpXVd-ddz-XvaOw" // Token de autenticação

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// 2. Configurações de Rede
char ssid[] = "INTELBRAS"; // Nome da rede Wi-Fi
char pass[] = "";          // Senha da rede

// 3. Pinos para Comunicação Serial com o Arduino
const int rxPin = 16;
const int txPin = 17; // O pino 17 envia o comando para o Arduino

// 4. Variáveis de Controle de Tempo (Timer)
bool timerAtivo = false;          // Flag para saber se o tempo está contando
unsigned long tempoInicial = 0;   // Armazena o momento exato do início
const long duracaoTimer = 10000;  // Define 10 segundos para resetar o App

// 5. Função que recebe comandos do App Blynk (Botão V0)
BLYNK_WRITE(V0)
{
  int estado = param.asInt(); // Lê o estado do botão (0 ou 1)
  
  // Se o botão foi ligado (1):
  if (estado == 1) {
    Serial.println("Comando V0 recebido: ABRIR. Iniciando Timer.");
    
    // Envia o comando "90" via Serial para o Arduino abrir a trava
    Serial2.println(90); 
    
    // Inicia a contagem do tempo no ESP32
    timerAtivo = true;
    tempoInicial = millis();
  }
}

void setup()
{
  // Inicializa Monitor Serial (Debug)
  Serial.begin(115200);
  delay(100);
  
  // Inicializa Serial2 para falar com o Arduino (Baud Rate 9600 deve ser igual no Arduino)
  Serial2.begin(9600, SERIAL_8N1, rxPin, txPin);

  Serial.println("Conectando ao Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Conectado!");
}

void loop()
{
  Blynk.run(); // Mantém a conexão com a nuvem ativa

  // 6. Lógica de Reset Automático do App
  // Se o timer está ativo E já se passaram 15 segundos
  if (timerAtivo && (millis() - tempoInicial >= duracaoTimer)) {
    Serial.println("10s passados. Resetando App e enviando comando de segurança.");
    
    Serial2.println(0); // Reforço: Manda Arduino fechar (caso ele não tenha fechado)
    
    // Atualiza o botão no aplicativo para "Desligado"
    Blynk.virtualWrite(V0, 0); 
    
    timerAtivo = false; // Encerra o timer
  }
}

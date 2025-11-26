/* --- CÓDIGO ESCRAVO (ARDUINO UNO + SERVO) --- */

#include <Servo.h>

Servo meuServo;
int pinoServo = 2; // Pino digital onde o servo está conectado

// 1. Variáveis do Timer de Segurança (Autofechamento)
bool timerAtivo = false;        
unsigned long tempoInicial = 0; 
const long duracaoTimer = 10000; // 10 segundos para fechar a trava fisicamente

void setup() {
  // Inicia Serial para escutar o ESP32 (RX0)
  Serial.begin(9600); 
  
  meuServo.attach(pinoServo);
  meuServo.write(0); // Estado inicial: Trancado (0 graus)
}

void loop() {
  // 2. Verificação de recebimento de dados via Serial
  if (Serial.available() > 0) {
    int angulo = Serial.parseInt(); // Lê o número enviado pelo ESP32
    
    // Se recebeu comando de abertura (> 0)
    if (angulo > 0) {
      meuServo.write(90); // Ação física: Move para 90 graus
      
      // Inicia o timer de segurança local
      timerAtivo = true;
      tempoInicial = millis();
      
      Serial.print("Abrindo. Timer de 10s iniciado. Valor: ");
      Serial.println(angulo);
    }
  }
  
  // 3. Lógica do Timer (Non-blocking delay)
  // Verifica se passaram 10 segundos desde a abertura
  if (timerAtivo && (millis() - tempoInicial >= duracaoTimer)) {
    Serial.println("Tempo limite atingido. Fechando trava.");
    
    meuServo.write(0);  // Ação física: Move para 0 graus (Tranca)
    timerAtivo = false; // Reseta o timer
  }
}

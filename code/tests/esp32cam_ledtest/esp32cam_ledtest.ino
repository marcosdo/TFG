// Definir el pin del LED incorporado
const int ledPin = 4; // El LED incorporado en la ESP32-CAM está conectado al pin GPIO 4

// Variable para almacenar el estado del LED
bool ledState = false;

void setup() {
  // Inicializar el pin del LED como salida
  pinMode(ledPin, OUTPUT);
  // Inicializar el LED apagado
  digitalWrite(ledPin, LOW);
}

void loop() {
  // Cambiar el estado del LED
  ledState = !ledState;
  digitalWrite(ledPin, ledState);

  // Esperar un segundo (1000 milisegundos)
  delay(1000);
}

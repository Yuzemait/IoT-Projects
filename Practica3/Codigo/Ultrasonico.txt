//Mi medidor de distancia
const int trigPin = 5; // Pin conectado al Trigger del sensor ultrasónico
const int echoPin = 18; // Pin conectado al Echo del sensor ultrasónico
// Definir la velocidad del sonido en cm/us
#define SOUND_SPEED 0.034 // Velocidad del sonido en cm/us
#define CM_TO_INCH 0.393701 // Conversión de centímetros a pulgadas
long duration; // Variable para almacenar el tiempo que tarda el eco en volver
float distanceCm; // Variable para almacenar la distancia en centímetros
float distanceInch; // Variable para almacenar la distancia en pulgadas
void setup() {
Serial.begin(9600); // Iniciar la comunicación serial para depuración
pinMode(trigPin, OUTPUT); // Configurar el pin Trigger como salida
pinMode(echoPin, INPUT); // Configurar el pin Echo como entrada
}
void loop() {
digitalWrite(trigPin, LOW); // Asegurarse de que el Trigger está apagado
delayMicroseconds(2);
// Enviar una señal de 10 microsegundos al Trigger
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Leer el tiempo de viaje del pulso usando el pin Echo
duration = pulseIn(echoPin, HIGH);
// Calcular la distancia en centímetros
distanceCm = duration * SOUND_SPEED / 2;
// Convertir la distancia a pulgadas
distanceInch = distanceCm * CM_TO_INCH;
// Mostrar la distancia en el Monitor Serial
Serial.print("Distance (cm): ");
Serial.println(distanceCm);
Serial.print("Distance (inch): ");
Serial.println(distanceInch);
delay(1000); // Esperar un segundo antes de la siguiente medición
}

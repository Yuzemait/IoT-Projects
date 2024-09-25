#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

/* Declare LCD object for SPI
 Adafruit_PCD8544(CLK,DIN,D/C,CE,RST); */
Adafruit_PCD8544 display = Adafruit_PCD8544(14, 23, 4, 15, 2);
int contrastValue = 40; // Default Contrast Value
const int adcPin = 34;
int adcValue = 0;

//Mi medidor de distancia
const int trigPin = 5; // Pin conectado al Trigger del sensor ultrasónico
const int echoPin = 18; // Pin conectado al Echo del sensor ultrasónico
// Definir la velocidad del sonido en cm/us
#define SOUND_SPEED 0.034 // Velocidad del sonido en cm/us
#define CM_TO_INCH 0.393701 // Conversión de centímetros a pulgadas
long duration; // Variable para almacenar el tiempo que tarda el eco en volver
float distanceCm; // Variable para almacenar la distancia en centímetros
float distanceInch; // Variable para almacenar la distancia en pulgadas

void setup()
{
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); // Configurar el pin Trigger como salida
  pinMode(echoPin, INPUT); // Configurar el pin Echo como entrada
  /* Initialize the Display*/
  display.begin();

  /* Change the contrast using the following API*/
  display.setContrast(contrastValue);

  /* Clear the buffer */
  display.clearDisplay();
  display.display();
  delay(1000);

}

void loop()
{
  // Calcular distancia
  digitalWrite(trigPin, LOW); // Asegurarse de que el Trigger está apagado
  delayMicroseconds(2);
  // Enviar una señal de 10 microsegundos al Trigger
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Leer el tiempo de viaje del pulso usando el pin Echo
  duration = pulseIn(echoPin, HIGH);
  Serial.println(duration);
  // Calcular la distancia en centímetros
  distanceCm = duration * SOUND_SPEED / 2;
  // Convertir la distancia a pulgadas
  distanceInch = distanceCm * CM_TO_INCH;
  // Mostrar la distancia en el Monitor Serial
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);

  display.setContrast(contrastValue);

// Imprimir distancia display
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0,1);
  display.setTextSize(2);
  display.print("|ESP32|");
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(2,22);
  display.print(distanceCm);
  display.print(" cm");
  display.drawFastHLine(0,32,83,BLACK);
  display.setCursor(2, 38);
  display.print(distanceInch);
  display.print(" inch");
  display.display();


  delay(1000);
}
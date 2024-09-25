#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

/* Declare LCD object for SPI
 Adafruit_PCD8544(CLK,DIN,D/C,CE,RST); */
Adafruit_PCD8544 display = Adafruit_PCD8544(18, 23, 4, 15, 2);
int contrastValue = 40; // Default Contrast Value
const int adcPin = 34;
int adcValue = 0;
#include "DHT.h" // Librería para sensor DHT
#define DHTPIN 5 // Pin al que está conectado el sensor
#define DHTTYPE DHT11 // Tipo de sensor
DHT dht(DHTPIN, DHTTYPE); // Inicialización del sensor

void setup()
{
  Serial.begin(9600);
  Serial.println(F("DHTxx test!")); // Mensaje de inicio
  dht.begin(); // Iniciar el sensor
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
  display.setContrast(contrastValue);
  

  float h = dht.readHumidity(); // Leer humedad
  float t = dht.readTemperature(); // Leer temperatura
  if (isnan(h) || isnan(t)) { // Verificar errores en la lectura
  Serial.println(F("Failed to read from DHT sensor!"));
  return;
  }
// Imprimir humedad y temperatura
display.clearDisplay();
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0,1);
  display.setTextSize(2);
  display.print("|ESP32|");
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(2,22);
  display.print("Temp: ");
  display.print(t);
  display.print("C");
  display.drawFastHLine(0,32,83,BLACK);
  display.setCursor(2, 38);
  display.print("Hum: ");
  display.print(h);
  display.print("h");
  display.display();

  Serial.print("Humedad: ");
  Serial.println(h);
  Serial.print("Temperatura: ");
  Serial.println(t);
  delay(1000);
}
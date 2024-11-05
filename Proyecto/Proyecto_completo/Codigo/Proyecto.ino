#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_SSD1306.h>

#define TEMP 40.0
#define MIN_DISTANCE 10.0
#define LED_PIN 12 

// Pantalla OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//Infrarrojo
Adafruit_MLX90614 mlx = Adafruit_MLX90614(); //Se declara una variable u objeto para el sensor  

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

  // Comprobar si la pantalla OLED comienza correctamente
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // La dirección 0x3C es común, ajusta si es necesario
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Detener si no se detecta el display
  }

  // Start screen
  // Limpiar el buffer de la pantalla
  display.clearDisplay();
  display.setTextSize(1.5);             // Tamaño del texto
  display.setTextColor(SSD1306_WHITE); // Color del texto

  delay(2000); // Pause for 2 seconds

  pinMode(trigPin, OUTPUT); // Configurar el pin Trigger como salida
  pinMode(echoPin, INPUT); // Configurar el pin Echo como entrada

  mlx.begin(); //Se inicia el sensor

  // Configurar el pin del LED como salida
  pinMode(LED_PIN, OUTPUT);

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
  // Calcular la distancia en centímetros
  distanceCm = duration * SOUND_SPEED / 2;
  // Convertir la distancia a pulgadas
  distanceInch = distanceCm * CM_TO_INCH;
  // Mostrar la distancia en el Monitor Serial
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);

  //Se imprimen los datos de sensor infrarrojo
  Serial.print("Temperatura Ambiente= "); 
  Serial.print(mlx.readAmbientTempC()); Serial.println(" °C");
  Serial.print("Temperatura del Objeto= "); 
  Serial.print(mlx.readObjectTempC()); Serial.println(" °C"); 

  mostrarDatosOLED(distanceCm, mlx.readObjectTempC(),mlx.readAmbientTempC());
  prenderLED(mlx.readObjectTempC());


  delay(1000);
}

void mostrarDatosOLED(float distance, float tempObj, float tempAmb) {
   // Limpiar la pantalla antes de escribir
  display.clearDisplay();

  // Mostrar distancia
  display.setCursor(0, 0);
  // display.print("Distancia: ");
  // display.print(distance);
  // display.println(" cm");
  if (distance < MIN_DISTANCE){
    display.println("DETECTADO!!!");
  } else {
    display.println("NADIE CERCA");
  }

  // Mostrar temperatura
  display.setCursor(0, 16); // Cambiar a la siguiente línea para mostrar la temperatura
  display.print("T: ");
  display.print(tempObj);
  display.println(" C");

  

  // Actualizar el display con el nuevo contenido
  display.display();
}

void prenderLED(float temp){
  if (temp < TEMP) {
    digitalWrite(LED_PIN, HIGH);  // Enciende el LED si la temperatura es menor a 40
    Serial.println("A calentarse!");
  } else {
    digitalWrite(LED_PIN, LOW);   // Apaga el LED si la temperatura es mayor o igual a 40
    Serial.println("Temperatura deseada");
  }
}
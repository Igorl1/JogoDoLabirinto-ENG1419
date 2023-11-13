#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Servo.h>
Servo myservo; // cria o objeto myservo
Servo myservo1; // cria o objeto myservo
int pinX = A0;   
int pinY = A1; 

Adafruit_MPU6050 srituhobby;
int angulo = 90;

const int numSamples = 10;
int xSamples[numSamples];
int ySamples[numSamples];
int sampleIndex = 0;

void setup()
{
Serial.begin(9600);
myservo.attach(6); // configura pino D6 - controle do Servo
myservo1.attach(7); // configura pino D6 - controle do Servo
for (int angulo = 70; angulo <= 110; angulo++) {
 myservo1.write(angulo);
 delay(15);
 }
delay(1000);
for (int angulo = 100; angulo >= 90; angulo--) {
 myservo1.write(angulo);
 delay(15);
 }
 delay(1000);
for (int angulo = 70; angulo <= 110; angulo++) {
 myservo.write(angulo);
 delay(15);
 }
delay(1000);
for (int angulo = 110; angulo >= 90; angulo--) {
 myservo.write(angulo);
 delay(15);
 }
 Wire.begin();
  srituhobby.begin();
  srituhobby.setAccelerometerRange(MPU6050_RANGE_8_G);//2_G,4_G,8_G,16_G
  srituhobby.setGyroRange(MPU6050_RANGE_500_DEG);//250,500,1000,2000
  srituhobby.setFilterBandwidth(MPU6050_BAND_21_HZ);

 delay(1000);
}


int averageFilter(int* samples, int numSamples) {
  int sum = 0;
  for (int i = 0; i < numSamples; i++) {
    sum += samples[i];
  }
  return sum / numSamples;
}

void loop()
{
sensors_event_t a, g, temp;
  srituhobby.getEvent(&a, &g, &temp);

 int xValue = a.acceleration.x;
 int yValue = a.acceleration.y;



//int xValue = analogRead(pinX);
//int yValue = analogRead(pinY);

  xSamples[sampleIndex] = xValue;
  ySamples[sampleIndex] = yValue;

  int xFiltered = averageFilter(xSamples, numSamples);
  int yFiltered = averageFilter(ySamples, numSamples);
  int xMapped = map(xFiltered, -10, 10, 110, 70);
  int yMapped = map(yFiltered, -10, 10, 70, 110);

  myservo1.write(xMapped); // controle PWM do servo
  Serial.print("Servo 1 (x): "); Serial.print(xMapped); Serial.print("//"); 

  sampleIndex = (sampleIndex + 1) % numSamples;//para o siguente
  myservo.write(yMapped); // controle PWM do servo
  Serial.print("Servo 2 (y): "); Serial.println(yMapped);

  delay(100);


}

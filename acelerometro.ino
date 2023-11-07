#include <I2Cdev.h>
#include <MPU6050.h> 
#include <Servo.h>
#include <GFButton.h>

MPU6050 MPU;
Servo servo1, servo2;
GFButton botao1(11); // Alterar pino conforme necessidade
int16_t axlsb,aylsb,azlsb,gxlsb,gylsb,gzlsb;
float angRoll, angAntRoll = 0, angPitch, angAntPitch = 0, ax, ay, az;

void setAngulos(){
  angAntRoll = angRoll;
  angAntPitch = angPitch;
}

void setup() {
  Serial.begin(9600);
  MPU.initialize();
  servo1.attach(3);
  servo2.attach(2);
  servo1.write(0);
  servo2.write(0);
  botao1.setPressHandler(setAngulos);
}

void loop() {
  MPU.getMotion6(&axlsb, &aylsb, &azlsb, &gxlsb, &gylsb, &gzlsb);
  ax = (float)axlsb/16384-0.01;  // Converte medidas do acelerometro de LSB para valores reais [ O número subtraido é a calibração ]
  ay = (float)aylsb/16384-0.01;
  az = (float)azlsb/16384-0.02;

  angRoll = atan(ay/sqrt(ax*ax*az*az))*1/(3.142/180);    // Calcula os angulos em graus
  angPitch = atan(ax/sqrt(ay*ay*az*az))*1/(3.142/180);

  servo1.write(angRoll-angAntRoll);      // Pode também fazer mapeamento dos angulos para não ultrapassarem limites
  servo2.write(angPitch-angAntPitch);
  
  Serial.print("aX = "); Serial.print(ax);
  Serial.print(" | aY = "); Serial.print(ay);
  Serial.print(" | aX_map = "); Serial.print(ay);
  Serial.print(" | aY_map = "); Serial.print(ay);
  Serial.print(" | angRoll = "); Serial.print(angRoll);
  Serial.print(" | angPitch = "); Serial.print(angPitch);
  Serial.println();

  delay(1000);
}

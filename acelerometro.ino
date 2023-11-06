#include <I2Cdev.h>
#include <MPU6050.h> 

MPU6050 MPU;
int16_t ax,ay,az,gx,gy,gz;
float angRoll, angPitch, ax2, ay2, az2;

void setup() {
  Serial.begin(9600);
  MPU.initialize();
}

void loop() {
  MPU.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  ax2 = (float)ax/16384-0.01;  // Converte medidas do acelerometro de LSB para valores reais [ O número subtraido é a calibração ]
  ay2 = (float)ay/16384-0.01;
  az2 = (float)az/16384-0.02;

  angRoll = atan(ay2/sqrt(ax2*ax2*az2*az2))*1/(3.142/180);    // Calcula os angulos em graus
  angPitch = atan(ax2/sqrt(ay2*ay2*az2*az2))*1/(3.142/180);
  
  Serial.print("aX = "); Serial.print(ax2);
  Serial.print(" | aY = "); Serial.print(ay2);
  Serial.print(" | angRoll = "); Serial.print(angRoll);
  Serial.print(" | angPitch = "); Serial.print(angPitch);
  Serial.println();

  delay(1000);
}

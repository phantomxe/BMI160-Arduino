#include <BMI160Gen.h>

const int cs_pins[] = {10, 9, 8, 7};

void setup() {
  Serial.begin(9600); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open

  // initialize device
  Serial.println("Initializing IMU device...");
  if(!BMI160.multiSpiBegin(cs_pins)) {
    Serial.println("Initializing failed!");
    while(1) {}
  }
  
  for(int i = 0; i < 4; i++) {
    BMI160.setSelectPin(cs_pins[i]);

    uint8_t dev_id = BMI160.getDeviceID();
    Serial.print("DEVICE ID: ");
    Serial.println(dev_id, HEX);

    // Set the accelerometer range to 250 degrees/second
    BMI160.setGyroRange(250);
    Serial.println("Initializing IMU device...done.");
  } 

}

void loop() {
  int gxRaw, gyRaw, gzRaw;         // raw gyro values
  float gx, gy, gz;

  // read raw gyro measurements from device

  for(int i = 0; i < 4; i++) {
    BMI160.setSelectPin(cs_pins[i]);
    BMI160.readGyro(gxRaw, gyRaw, gzRaw);

    // convert the raw gyro data to degrees/second
    gx = convertRawGyro(gxRaw);
    gy = convertRawGyro(gyRaw);
    gz = convertRawGyro(gzRaw);

    // display tab-separated gyro x/y/z values
    Serial.print("g:\t");
    Serial.print(gx);
    Serial.print("\t");
    Serial.print(gy);
    Serial.print("\t");
    Serial.print(gz);
    Serial.println();

    delay(100);
  }
  delay(500);
}

float convertRawGyro(int gRaw) {
  // since we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767

  float g = (gRaw * 250.0) / 32768.0;

  return g;
}
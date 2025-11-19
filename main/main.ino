#include <BleMouse.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

BleMouse bleMouse("ESP32 BLE Mouse");
Adafruit_MPU6050 mpu;

// Button pins
#define LEFT_CLICK_PIN   18
#define RIGHT_CLICK_PIN  19
#define SCROLL_UP_PIN    14
#define SCROLL_DOWN_PIN  27

// Base sensitivity and threshold
float baseSensitivity = 10.0;  // Adjust for overall speed
float threshold       = 0.02;  // Increased deadzone to reduce jitter

// Offsets for calibration
float gyroX_offset = 0;
float gyroY_offset = 0;
float gyroZ_offset = 0;

// Gyro calibration function
void calibrateGyro(int samples = 500) {
  sensors_event_t a, g, temp;
  float sumX = 0, sumY = 0, sumZ = 0;

  Serial.println("Calibrating gyro... Keep the device still.");
  delay(1000); // Small delay to settle

  for (int i = 0; i < samples; i++) {
    mpu.getEvent(&a, &g, &temp);
    sumX += g.gyro.x;
    sumY += g.gyro.y;
    sumZ += g.gyro.z;
    delay(5);
  }

  gyroX_offset = sumX / samples;
  gyroY_offset = sumY / samples;
  gyroZ_offset = sumZ / samples;

  Serial.println("Calibration complete.");
  Serial.print("Offsets - X: "); Serial.print(gyroX_offset, 6);
  Serial.print(" Y: "); Serial.print(gyroY_offset, 6);
  Serial.print(" Z: "); Serial.println(gyroZ_offset, 6);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Mouse with MPU6050 (Auto-Calibrated)");

  bleMouse.begin();

  if (!mpu.begin()) {
    Serial.println("MPU6050 not found, check wiring!");
    while (1) delay(10);
  }

  // Configure MPU ranges
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // Setup button pins
  pinMode(LEFT_CLICK_PIN, INPUT_PULLUP);
  pinMode(RIGHT_CLICK_PIN, INPUT_PULLUP);
  pinMode(SCROLL_UP_PIN, INPUT_PULLUP);
  pinMode(SCROLL_DOWN_PIN, INPUT_PULLUP);

  // Run auto-calibration
  calibrateGyro();

  Serial.println("Setup complete, waiting for BLE connection...");
}

void loop() {
  if (bleMouse.isConnected()) {
    
    // Get sensor readings
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Apply offsets
    float gyroX = g.gyro.x - gyroX_offset; // Roll
    float gyroY = g.gyro.y - gyroY_offset; // Pitch
    float gyroZ = g.gyro.z - gyroZ_offset; // Yaw

    float moveX = gyroZ;
    float moveY = gyroY;

    // Deadzone filter
    if (fabs(moveX) < threshold) moveX = 0;
    if (fabs(moveY) < threshold) moveY = 0;

    // Dynamic sensitivity
    float speed = sqrt(moveX * moveX + moveY * moveY);
    float dynamicSensitivity = baseSensitivity * (1 + speed * 0.2);

    // Apply scaling
    moveX *= dynamicSensitivity;
    moveY *= dynamicSensitivity + 2;

    // Move cursor
    if ((int)moveX != 0 || (int)moveY != 0) {
      bleMouse.move((int)-moveX, (int)moveY); // Invert X for natural feel
    }

    // Button input
    if (digitalRead(LEFT_CLICK_PIN) == LOW) {
      bleMouse.press(MOUSE_LEFT);
    } else {
      bleMouse.release(MOUSE_LEFT);
    }

    if (digitalRead(RIGHT_CLICK_PIN) == LOW) {
      bleMouse.press(MOUSE_RIGHT);
    } else {
      bleMouse.release(MOUSE_RIGHT);
    }

    if (digitalRead(SCROLL_UP_PIN) == LOW) {
      bleMouse.move(0, 0, 1); // Scroll up
      delay(150);
    }

    if (digitalRead(SCROLL_DOWN_PIN) == LOW) {
      bleMouse.move(0, 0, -1); // Scroll down
      delay(150);
    }
  }

  delay(5); // smooth loop
}

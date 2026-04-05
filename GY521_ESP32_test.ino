// ============================================================
//  IMU GY-521 (MPU6050) + ESP32 — เช็คอุปกรณ์เบื้องต้น
// ============================================================
//
//  การต่อขา GY-521 → ESP32
//  --------------------------------
//  GY-521      ESP32
//  --------    -------
//  VCC     →   3.3V
//  GND     →   GND
//  SCL     →   GPIO 22  (I2C Clock)
//  SDA     →   GPIO 21  (I2C Data)
//  AD0     →   GND      (I2C Address = 0x68)
//  INT     →   ไม่ต้องต่อ (ใช้แบบ Polling)
//
//  ติดตั้ง Library ก่อน:
//  VS Code → เปิด Library Manager → ค้นหา "MPU6050" 
//  → ติดตั้ง "MPU6050 by Electronic Cats" หรือ "Adafruit MPU6050"
// ============================================================

#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// ============================================================
//  Setup
// ============================================================
void setup() {
  Serial.begin(115200);
  Serial.println("=== IMU GY-521 (MPU6050) + ESP32 เช็คอุปกรณ์ ===");
  Serial.println("ต่อขา: VCC→3.3V, GND→GND, SCL→GPIO22, SDA→GPIO21");
  Serial.println("--------------------------------------------------");

  Wire.begin(21, 22);   // SDA, SCL
  mpu.initialize();

  // เช็คว่าเชื่อมต่อได้ไหม
  if (mpu.testConnection()) {
    Serial.println(" MPU6050 เชื่อมต่อสำเร็จ!");
  } else {
    Serial.println(" ไม่พบ MPU6050 — เช็คการต่อขาอีกครั้ง");
    while (true);   // หยุดรอถ้าหาเซ็นเซอร์ไม่เจอ
  }

  delay(1000);
}

// ============================================================
//  Loop — อ่านและแสดงผลทุก 200ms
// ============================================================
void loop() {
  // ตัวแปรรับค่า raw จากเซ็นเซอร์
  int16_t ax, ay, az;   // Accelerometer (ความเร่ง)
  int16_t gx, gy, gz;   // Gyroscope (ความเร็วเชิงมุม)

  // อ่านค่าจาก MPU6050
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // แปลงค่า raw → หน่วยจริง
  // Accelerometer: ค่า raw หารด้วย 16384 → หน่วย g (±2g mode)
  float accX = ax / 16384.0;
  float accY = ay / 16384.0;
  float accZ = az / 16384.0;

  // Gyroscope: ค่า raw หารด้วย 131 → หน่วย °/s (±250°/s mode)
  float gyroX = gx / 131.0;
  float gyroY = gy / 131.0;
  float gyroZ = gz / 131.0;

  // อุณหภูมิ
  float temp = mpu.getTemperature() / 340.0 + 36.53;

  // แสดงผล Accelerometer
  Serial.println("--- Accelerometer (g) ---");
  Serial.print("  X: "); Serial.print(accX, 2);
  Serial.print("  Y: "); Serial.print(accY, 2);
  Serial.print("  Z: "); Serial.println(accZ, 2);

  // แสดงผล Gyroscope
  Serial.println("--- Gyroscope (°/s) ---");
  Serial.print("  X: "); Serial.print(gyroX, 2);
  Serial.print("  Y: "); Serial.print(gyroY, 2);
  Serial.print("  Z: "); Serial.println(gyroZ, 2);

  // แสดงผลอุณหภูมิ
  Serial.print("--- Temperature: ");
  Serial.print(temp, 1);
  Serial.println(" °C ---");

  Serial.println("--------------------------------------------------");

  delay(200);
}

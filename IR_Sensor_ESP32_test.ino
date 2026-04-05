// ============================================================
//  IR Slotted Optical Sensor (Speed Sensor) + ESP32
//  เช็คอุปกรณ์เบื้องต้น — นับรอบ + คำนวณความเร็ว
// ============================================================
//
//  การต่อขา Sensor → ESP32
//  --------------------------------
//  Sensor      ESP32
//  --------    -------
//  VCC     →   3.3V หรือ 5V
//  GND     →   GND
//  OUT     →   GPIO 34  (ใช้ขา Input-only ของ ESP32 ได้เลย)
//
//  หลักการทำงาน:
//  - เมื่อมีสิ่งของผ่านช่องเซ็นเซอร์ → สัญญาณ OUT เปลี่ยน (HIGH→LOW หรือ LOW→HIGH)
//  - นับจำนวนครั้งที่สัญญาณเปลี่ยน → รู้จำนวนรอบที่หมุน
// ============================================================

#define SENSOR_PIN  34   // ขา OUT ของเซ็นเซอร์

// --- ตัวแปรนับรอบ (ใช้ volatile เพราะเปลี่ยนใน Interrupt) ---
volatile int pulseCount = 0;

// --- ตัวแปรคำนวณความเร็ว ---
int rpm = 0;
unsigned long lastTime = 0;

// จำนวน Slot ของดิสก์ที่ใช้ (ปกติดิสก์พลาสติกที่มาด้วยมี 20 ช่อง)
#define SLOTS_PER_REV  20

// ============================================================
//  Interrupt — เรียกทุกครั้งที่เซ็นเซอร์ตรวจพบการเปลี่ยนแปลง
// ============================================================
void IRAM_ATTR onPulse() {
  pulseCount++;
}

// ============================================================
//  Setup
// ============================================================
void setup() {
  Serial.begin(115200);
  Serial.println("=== IR Slotted Sensor + ESP32 เช็คอุปกรณ์ ===");
  Serial.println("ต่อขา: VCC→3.3V, GND→GND, OUT→GPIO34");
  Serial.println("----------------------------------------------");

  pinMode(SENSOR_PIN, INPUT);

  // ตั้ง Interrupt — ตรวจจับขาขึ้น (RISING) ของสัญญาณ
  attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), onPulse, RISING);

  lastTime = millis();
}

// ============================================================
//  Loop — คำนวณและแสดงผลทุก 1 วินาที
// ============================================================
void loop() {
  unsigned long currentTime = millis();

  // คำนวณทุก 1000ms (1 วินาที)
  if (currentTime - lastTime >= 1000) {

    // หยุด Interrupt ชั่วคราวเพื่ออ่านค่าอย่างปลอดภัย
    detachInterrupt(digitalPinToInterrupt(SENSOR_PIN));

    // คำนวณ RPM
    // RPM = (จำนวน pulse ใน 1 วิ / จำนวน Slot) x 60
    rpm = (pulseCount * 60) / SLOTS_PER_REV;

    Serial.print("Pulse Count: ");
    Serial.print(pulseCount);
    Serial.print("  |  RPM: ");
    Serial.println(rpm);

    // รีเซ็ตค่าสำหรับรอบถัดไป
    pulseCount = 0;
    lastTime = currentTime;

    // เปิด Interrupt อีกครั้ง
    attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), onPulse, RISING);
  }
}

// ============================================================
//  Ultrasonic HC-SR04 + ESP32 — เช็คอุปกรณ์เบื้องต้น
// ============================================================
//
//  การต่อขา HC-SR04 → ESP32
//  --------------------------------
//  HC-SR04     ESP32
//  --------    -------
//  VCC     →   5V
//  GND     →   GND
//  TRIG    →   GPIO 5
//  ECHO    →   GPIO 18
//
//  หลักการทำงาน:
//  - ESP32 ส่ง Pulse สั้นๆ ออกทาง TRIG (10 microsecond)
//  - เซ็นเซอร์ยิงคลื่นเสียงออกไป
//  - รอรับสัญญาณสะท้อนกลับที่ขา ECHO
//  - วัดเวลาที่ใช้ → คำนวณระยะทาง
// ============================================================

#define TRIG_PIN  5
#define ECHO_PIN  18

// ============================================================
//  ฟังก์ชันวัดระยะทาง (คืนค่าเป็น cm)
// ============================================================
float getDistance() {
  // ส่ง Trigger pulse 10 microsecond
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // วัดเวลาที่ ECHO เป็น HIGH (timeout 30ms = ระยะสูงสุด ~5m)
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  // ถ้าไม่มีสัญญาณตอบกลับ
  if (duration == 0) {
    return -1;
  }

  // คำนวณระยะทาง
  // ความเร็วเสียง = 340 m/s = 0.034 cm/us
  // หารด้วย 2 เพราะเสียงเดินทางไปและกลับ
  float distance = duration * 0.034 / 2;

  return distance;
}

// ============================================================
//  Setup
// ============================================================
void setup() {
  Serial.begin(115200);
  Serial.println("=== Ultrasonic HC-SR04 + ESP32 เช็คอุปกรณ์ ===");
  Serial.println("ต่อขา: VCC→5V, GND→GND, TRIG→GPIO5, ECHO→GPIO18");
  Serial.println("------------------------------------------------");

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

// ============================================================
//  Loop — วัดและแสดงผลทุก 500ms
// ============================================================
void loop() {
  float distance = getDistance();

  if (distance == -1) {
    Serial.println("ระยะทาง: ไม่พบวัตถุ (เกิน 400 cm)");
  } else {
    Serial.print("ระยะทาง: ");
    Serial.print(distance, 1);   // ทศนิยม 1 ตำแหน่ง
    Serial.println(" cm");
  }

  delay(500);
}

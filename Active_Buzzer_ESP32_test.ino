// ============================================================
//  Active Buzzer + ESP32 — เช็คอุปกรณ์เบื้องต้น
// ============================================================
//
//  การต่อขา Active Buzzer → ESP32
//  --------------------------------
//  Buzzer      ESP32
//  --------    -------
//  VCC (+) →   GPIO 23
//  GND (-) →   GND
//
//  หลักการ:
//  - Active Buzzer มีวงจรสั่นในตัว
//  - แค่ให้ไฟ (HIGH) → มีเสียง
//  - ตัดไฟ (LOW) → เงียบ
//  - ไม่ต้องใช้ PWM หรือกำหนดความถี่เอง
// ============================================================

#define BUZZER_PIN  23

void setup() {
  Serial.begin(115200);
  Serial.println("=== Active Buzzer + ESP32 ===");

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {

  // ทดสอบ 1: ร้องสั้นๆ 1 ครั้ง
  Serial.println("[1] ร้องสั้น 1 ครั้ง");
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
  delay(1000);

  // ทดสอบ 2: ร้องยาว 1 ครั้ง
  Serial.println("[2] ร้องยาว 1 ครั้ง");
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);
  delay(1000);

  // ทดสอบ 3: ร้องสั้น 3 ครั้งติดกัน (แบบแจ้งเตือน)
  Serial.println("[3] ร้องสั้น 3 ครั้ง");
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }

  Serial.println("=== รอ 3 วินาที แล้วเริ่มใหม่ ===\n");
  delay(3000);
}

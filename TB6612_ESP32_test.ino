// ============================================================
//  TB6612 + ESP32 — เช็คอุปกรณ์ครั้งแรก
//  STBY ต่อ 5V ตรงๆ (ไม่ต้องควบคุมจาก ESP32)
// ============================================================
//
//  การต่อขา TB6612 → ESP32
//  --------------------------------
//  TB6612      ESP32
//  --------    -------
//  AIN1    →   GPIO 27
//  AIN2    →   GPIO 26
//  PWMA    →   GPIO 14
//  BIN1    →   GPIO 25
//  BIN2    →   GPIO 33
//  PWMB    →   GPIO 32
//  STBY    →   5V (เปิดใช้งานตลอด)
//  GND     →   GND (ร่วมกับ ESP32 และ Battery)
//  VM      →   แรงดันมอเตอร์ (6V–12V จาก Battery)
//  VCC     →   3.3V หรือ 5V (ไฟเลี้ยง Logic)
// ============================================================

// --- กำหนดขา Motor A ---
#define AIN1  27
#define AIN2  26
#define PWMA  14

// --- กำหนดขา Motor B ---
#define BIN1  25
#define BIN2  33
#define PWMB  32

// --- ตั้งค่า PWM (LEDC ของ ESP32) ---
#define FREQ        5000   // ความถี่ PWM (Hz)
#define RESOLUTION  8      // ความละเอียด 8-bit (0-255)
#define CH_A        0      // LEDC Channel สำหรับ Motor A
#define CH_B        1      // LEDC Channel สำหรับ Motor B

// ============================================================
//  ฟังก์ชันควบคุมมอเตอร์
// ============================================================

// Motor A — หมุนไปข้างหน้า
void motorA_Forward(int speed) {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  ledcWrite(CH_A, speed);   // speed: 0–255
}

// Motor A — หมุนถอยหลัง
void motorA_Backward(int speed) {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  ledcWrite(CH_A, speed);
}

// Motor A — หยุด
void motorA_Stop() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  ledcWrite(CH_A, 0);
}

// Motor B — หมุนไปข้างหน้า
void motorB_Forward(int speed) {
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  ledcWrite(CH_B, speed);
}

// Motor B — หมุนถอยหลัง
void motorB_Backward(int speed) {
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  ledcWrite(CH_B, speed);
}

// Motor B — หยุด
void motorB_Stop() {
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  ledcWrite(CH_B, 0);
}

// ============================================================
//  Setup
// ============================================================
void setup() {
  Serial.begin(115200);
  Serial.println("=== TB6612 + ESP32 เช็คอุปกรณ์ ===");

  // ตั้งค่าขา Direction เป็น OUTPUT
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // ตั้งค่า LEDC PWM
  ledcSetup(CH_A, FREQ, RESOLUTION);
  ledcAttachPin(PWMA, CH_A);

  ledcSetup(CH_B, FREQ, RESOLUTION);
  ledcAttachPin(PWMB, CH_B);

  Serial.println("ตั้งค่าขาเสร็จแล้ว เริ่มทดสอบ...");
  delay(1000);
}

// ============================================================
//  Loop — ทดสอบทีละขั้นตอน
// ============================================================
void loop() {

  // --- ทดสอบ 1: Motor A หมุนไปข้างหน้า ---
  Serial.println("[1] Motor A → หมุนไปข้างหน้า (2 วินาที)");
  motorA_Forward(180);   // speed 180/255 ≈ 70%
  delay(2000);
  motorA_Stop();
  delay(500);

  // --- ทดสอบ 2: Motor A หมุนถอยหลัง ---
  Serial.println("[2] Motor A → หมุนถอยหลัง (2 วินาที)");
  motorA_Backward(180);
  delay(2000);
  motorA_Stop();
  delay(500);

  // --- ทดสอบ 3: Motor B หมุนไปข้างหน้า ---
  Serial.println("[3] Motor B → หมุนไปข้างหน้า (2 วินาที)");
  motorB_Forward(180);
  delay(2000);
  motorB_Stop();
  delay(500);

  // --- ทดสอบ 4: Motor B หมุนถอยหลัง ---
  Serial.println("[4] Motor B → หมุนถอยหลัง (2 วินาที)");
  motorB_Backward(180);
  delay(2000);
  motorB_Stop();
  delay(500);

  // --- ทดสอบ 5: Motor A + B หมุนพร้อมกัน (เดินหน้า) ---
  Serial.println("[5] Motor A+B → เดินหน้าพร้อมกัน (3 วินาที)");
  motorA_Forward(180);
  motorB_Forward(180);
  delay(3000);
  motorA_Stop();
  motorB_Stop();
  delay(500);

  // --- ทดสอบ 6: Motor A + B ถอยหลังพร้อมกัน ---
  Serial.println("[6] Motor A+B → ถอยหลังพร้อมกัน (3 วินาที)");
  motorA_Backward(180);
  motorB_Backward(180);
  delay(3000);
  motorA_Stop();
  motorB_Stop();

  Serial.println("=== ทดสอบครบแล้ว รอ 5 วินาที แล้วเริ่มใหม่ ===\n");
  delay(5000);
}

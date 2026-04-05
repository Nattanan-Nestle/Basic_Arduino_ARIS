// ============================================================
//  Passive Buzzer + ESP32 — เช็คอุปกรณ์เบื้องต้น
// ============================================================
//
//  การต่อขา Passive Buzzer → ESP32
//  --------------------------------
//  Buzzer      ESP32
//  --------    -------
//  VCC (+) →   GPIO 23
//  GND (-) →   GND
//
//  หลักการ:
//  - Passive Buzzer ไม่มีวงจรสั่นในตัว
//  - ต้องส่งสัญญาณ PWM ความถี่ต่างๆ เพื่อให้เกิดเสียง
//  - ความถี่ต่าง → เสียงโน้ตต่าง → เล่นเพลงได้!
//  - ใช้ ledcWriteTone() ของ ESP32 สั่งความถี่ได้เลย
//
//  ตารางโน้ต (Hz):
//  โด=262, เร=294, มี=330, ฟา=349, ซอล=392, ลา=440, ที=494
// ============================================================

#define BUZZER_PIN  23
#define BUZZER_CH   0      // LEDC Channel

// --- กำหนดความถี่โน้ต (Hz) ---
#define NOTE_C  262   // โด
#define NOTE_D  294   // เร
#define NOTE_E  330   // มี
#define NOTE_F  349   // ฟา
#define NOTE_G  392   // ซอล
#define NOTE_A  440   // ลา
#define NOTE_B  494   // ที

// ฟังก์ชันเล่นโน้ต
void playNote(int frequency, int duration) {
  ledcWriteTone(BUZZER_CH, frequency);
  delay(duration);
  ledcWriteTone(BUZZER_CH, 0);   // หยุดเสียง
  delay(50);                      // หยุดสั้นๆ ระหว่างโน้ต
}

void setup() {
  Serial.begin(115200);
  Serial.println("=== Passive Buzzer + ESP32 ===");

  // ตั้งค่า LEDC PWM
  ledcSetup(BUZZER_CH, 2000, 8);
  ledcAttachPin(BUZZER_PIN, BUZZER_CH);
}

void loop() {

  // ทดสอบ 1: เล่นโน้ตทีละตัว โด-เร-มี-ฟา-ซอล-ลา-ที
  Serial.println("[1] เล่นโน้ต โด ถึง ที");
  playNote(NOTE_C, 300);
  playNote(NOTE_D, 300);
  playNote(NOTE_E, 300);
  playNote(NOTE_F, 300);
  playNote(NOTE_G, 300);
  playNote(NOTE_A, 300);
  playNote(NOTE_B, 300);
  delay(500);

  // ทดสอบ 2: เสียงแจ้งเตือน (สูงต่ำสลับ)
  Serial.println("[2] เสียงแจ้งเตือน");
  for (int i = 0; i < 3; i++) {
    playNote(1000, 200);   // เสียงสูง
    playNote(500,  200);   // เสียงต่ำ
  }
  delay(500);

  // ทดสอบ 3: เพลง Happy Birthday ท่อนแรก
  Serial.println("[3] Happy Birthday 🎂");
  playNote(NOTE_C, 300);
  playNote(NOTE_C, 150);
  playNote(NOTE_D, 400);
  playNote(NOTE_C, 400);
  playNote(NOTE_F, 400);
  playNote(NOTE_E, 800);

  Serial.println("=== รอ 3 วินาที แล้วเริ่มใหม่ ===\n");
  delay(3000);
}

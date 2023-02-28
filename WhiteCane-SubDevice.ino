#include <SPI.h>
#include <MFRC522.h>

#define CDS A1 // 조도센서 핀번호 지정
#define IR 4   // 적외선 센서 핀번호 지정
#define LED 5  // LED 핀번호 지정
#define RST 9  // RFID RST 핀번호 지정
#define SDA 10 // RFID SDA 핀번호 지정

MFRC522 mfrc522(SDA, RST); // MFRC522 객체 생성

int detection_flag = 0; // 장애물 감지 Flag

void setup() {
  Serial.begin(9600);    // Initialize serial communications with the PC
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MRFC522 card
  pinMode(IR, INPUT);    // 적외선 센서 INPUT 설정
  pinMode(LED, OUTPUT);  // LED INPUT 설정
}

void loop() {
  obstacle_detection();  // 장애물 감지
  led_control(); // LED 제어
  rfid_reader(); // RFID TAG 읽기
}

// 적외선 센서를 통해 장애물 감지 확인
void obstacle_detection() {
  int ir = digitalRead(IR);

  if (ir == 0) { // 장애물이 감지되었을 때
    if (detection_flag == 0) {
      detection_flag = 1;
      Serial.println("Obstacle Detection");
    }
  } else { // 장애물이 감지되지 않었을 때
    if (detection_flag == 1) {
      detection_flag = 0;
      Serial.println("Obstacle not detected");
    }
  }
}

// 주변 밝기에 따라 LED ON/OFF 제어
void led_control() {
  int cds = analogRead(CDS);

  if (cds >= 500) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}

// RFID TAG 읽기
void rfid_reader() {
  // 키 준비 (초기 세팅은 FFFFFFFFFFFFh로 되어 있음)
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  byte block;
  byte len;
  MFRC522::StatusCode status;

  if (! mfrc522.PICC_IsNewCardPresent()) { // 새 TAG 찾기
    return;
  }

  if (! mfrc522.PICC_ReadCardSerial()) {   // TAG 중 하나 선택
    return;
  }

  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); // TAG에 대한 세부 정보 복사

  byte buffer[18];
  block = 1;
  len = 18;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Authentication failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // TAG에 저장된 장소 확인
  String buffer_str;
  for (uint8_t i = 0; i < 16; i++) {
    if (buffer[i] != 32)  { // 32가 공백을 의미
      continue;
    } else {
      buffer[i] = '\0';
      buffer_str = String((char*)buffer);
      Serial.print("Location : ");
      Serial.println(buffer_str);
      break;
    }
  }
  Serial.println();

  delay(1000); // 다음 TAG를 읽기 위해 필요한 대기 시간

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
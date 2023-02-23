#define IR 4   // 적외선 센서 핀번호 지정
#define LED 5  // LED 핀번호 지정
#define CDS A1 // 조도센서 핀번호 지정

int detection_flag = 0; // 장애물 감지 Flag

void setup() {
  Serial.begin(9600);    // Serial open

  pinMode(IR, INPUT);    // 적외선 센서 INPUT 설정
  pinMode(LED, OUTPUT);  // LED INPUT 설정
}

void loop() {
  obstacle_detection();  // 장애물 감지
  led_control(); // LED 제어
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
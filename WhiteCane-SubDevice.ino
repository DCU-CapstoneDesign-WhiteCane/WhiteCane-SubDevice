#define IR 4   // 적외선 센서 핀번호 지정
#define LED 5  // LED 핀번호 지정
#define CDS A1 // 조도센서 핀번호 지정

void setup() {
  Serial.begin(9600);    // Serial open

  pinMode(IR, INPUT);    // 적외선 센서 INPUT 설정
  pinMode(LED, OUTPUT);  // LED INPUT 설정
}

void loop() {
  obstacle_detection();  // 장애물 감지
  led_control();
}

void obstacle_detection() { // 적외선 센서를 통해 장애물 감지 Check
  int ir = digitalRead(IR);

  if (ir == 0) {
    Serial.println("Obstacle Detection");
  }
}

void led_control() { // 주변 밝기에 따라 LED ON/OFF 제어
  int cds = analogRead(CDS);

  if (cds >= 500) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}
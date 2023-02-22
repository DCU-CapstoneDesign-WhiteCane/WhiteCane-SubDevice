#define IR 4  // 적외선 센서 핀번호 지정

void setup() {
  Serial.begin(9600);  // Serial open
  pinMode(IR, INPUT);  // 적외선 센서 INPUT 설정
}

void loop() {
  obstacle_detection();  // 장애물 감지
}

void obstacle_detection() { // 적외선 센서를 통해 장애물 감지 Check
  int ir = digitalRead(IR);

  if (ir == 0) {
    Serial.println("Obstacle Detection");
  }
}
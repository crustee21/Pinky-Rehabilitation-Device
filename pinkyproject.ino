
#include <Servo.h>

Servo fingerServo;

//pins
const int EMG_PIN = A0;
const int SERVO_PIN = 9;

//servo limits
const int OPEN_POS = 40;
const int CLOSED_POS = 180;

//emg settings
int baseline = 0;
int openThreshold = 0;
int closeThreshold = 0;

//emg strength for max assistance
const int MAX_EMG_ABOVE_BASELINE = 520;

//smoothing
float filteredEMG = 0;
const float alpha = 0.15;
//move servo
float currentAngle = CLOSED_POS;
bool active = false;

void setup() {
  Serial.begin(115200);
  analogReadResolution(10);

  fingerServo.attach(SERVO_PIN);
  fingerServo.write(CLOSED_POS);

  delay(3000);

  //calibrate
  long total = 0;

  for (int i = 0; i < 500; i++) {
    total += analogRead(EMG_PIN);
    delay(2);
  }

  baseline = total / 500;

  openThreshold = baseline + 30;
  closeThreshold = baseline + 45;

  filteredEMG = baseline;

  delay(500);
}

void loop() {

  //read emg
  int rawEMG = analogRead(EMG_PIN);

  //exponential moving average
  filteredEMG =
      alpha * rawEMG +
      (1.0 - alpha) * filteredEMG;

  int emg = (int)filteredEMG;

  //hysterisis
  if (!active && emg >= closeThreshold) {
    active = true;
  }

  if (active && emg <= openThreshold) {
    active = false;
  }

  //servo target
  int targetAngle = CLOSED_POS;

  if (active) {
    int maxEMG = baseline + MAX_EMG_ABOVE_BASELINE;

    int limitedEMG = constrain(
        emg,
        closeThreshold,
        maxEMG
    );

    targetAngle = map(
        limitedEMG,
        closeThreshold,
        maxEMG,
        CLOSED_POS,
        OPEN_POS
    );
  }

  //smooth movement
  const float SERVO_STEP = 1.0;

  if (currentAngle < targetAngle) {
    currentAngle += SERVO_STEP;
    if (currentAngle > targetAngle)
      currentAngle = targetAngle;
  }
  else if (currentAngle > targetAngle) {
    currentAngle -= SERVO_STEP;
    if (currentAngle < targetAngle)
      currentAngle = targetAngle;
  }

  fingerServo.write((int)currentAngle);

  //serial plotter
  Serial.print("EMG:");
  Serial.print(emg);

  Serial.print("\tOpenThreshold:");
  Serial.print(openThreshold);

  Serial.print("\tCloseThreshold:");
  Serial.print(closeThreshold);

  Serial.print("\tServo:");
  Serial.println((int)currentAngle);

  delay(8);
}

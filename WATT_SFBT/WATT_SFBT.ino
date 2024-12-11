int test[] = { 0, 0, 0 };
float totalTime = 0;
float minTime = 0;
float mentalAge = 0;

// Pin definitions
const int ldrPins[] = { 13, 12, 11, 10, 9, 8, 7, 6, 5, 4 };  // Analog pins for 10 LDRs
const int buzzerPin = 3;                                     // Digital pin for the buzzer
const int YELLOWLED = 2;                                     // Digital pin for the yellow LED


char readSignal() {
  char com = 'x';
  if (Serial.available()) {
    com = Serial.read();
  }
  return com;
}

void resetPatientData() {
  for (int i = 0; i < 3; i++) { test[i] = 0; }
  totalTime = 0;
  minTime = 0;
  mentalAge = 0;
}

void sendReport() {
  String s = String(test[0]) + '@' + String(test[1]) + '@' + String(test[2]) + '@' + String(mentalAge);
  Serial.println(s);
}

void sendTestTime(int i) {
  String s = String(test[i]);
  char msgType[] = { 'A', 'B', 'C' };
  Serial.print(msgType[i] + s);
}

float minimum() {
  float m = test[0];
  for (int i = 1; i < 3; i++) {
    if (m > test[i]) m = test[i];
  }
  return m;
}

float total() {
  float t = 0;
  for (int i = 0; i < 3; i++) {
    t += test[i];
  }
  return t;
}

float startTest() {
  beep();
  unsigned long startTime = millis();

  // check Blocks placement
  while (!blocksPlaced()) {
    if (readSignal() == 'X') {
      //Stop Test
      yellow_and_beep();
      return 0;
    }
    y_blink();
  }

  unsigned long endTime = millis();  // Capture the end time

  // Calculate the time elapsed in seconds
  float elapsedTime = (endTime - startTime) / 1000;
  yellow_and_beep();
  return elapsedTime;
}

bool blocksPlaced() {
  for (int i = 0; i < sizeof(ldrPins) / sizeof(ldrPins[0]); i++) {
    int ldrValue = digitalRead(ldrPins[i]);
    if (ldrValue == 0) return false;
    else return true;
  }
}

float calculateMA() {
  // Define MA value based on the total time and minimum time
  if (totalTime < 34 && minTime < 10) {
    return 20;
  } else if (totalTime < 34 && minTime < 10.5) {
    return 19;
  } else if (totalTime < 34 && minTime < 11) {
    return 18;
  } else if (totalTime < 35 && minTime < 11) {
    return 17;
  } else if (totalTime < 35 && minTime < 11.5) {
    return 16;
  } else if (totalTime < 36 && minTime < 12) {
    return 15;
  } else if (totalTime < 37 && minTime < 12) {
    return 14.5;
  } else if (totalTime < 39 && minTime < 12.5) {
    return 14;
  } else if (totalTime < 41 && minTime < 12.5) {
    return 13.5;
  } else if (totalTime < 43 && minTime < 13) {
    return 13;
  } else if (totalTime < 46 && minTime < 13.5) {
    return 12.5;
  } else if (totalTime < 49 && minTime < 14) {
    return 12;
  } else if (totalTime < 52 && minTime < 14.5) {
    return 11.5;
  } else if (totalTime < 55 && minTime < 15) {
    return 11;
  } else if (totalTime < 59 && minTime < 16) {
    return 10.5;
  } else if (totalTime < 61 && minTime < 16.5) {
    return 10;
  } else if (totalTime < 64 && minTime < 17.5) {
    return 9.5;
  } else if (totalTime < 68 && minTime < 18.5) {
    return 9;
  } else if (totalTime < 72 && minTime < 19) {
    return 8.5;
  } else if (totalTime < 77 && minTime < 20) {
    return 8;
  } else if (totalTime < 83 && minTime < 21.5) {
    return 7.5;
  } else if (totalTime < 90 && minTime < 23) {
    return 7;
  } else if (totalTime < 98 && minTime < 25) {
    return 6.5;
  } else if (totalTime < 105 && minTime < 27) {
    return 6;
  } else if (totalTime < 114 && minTime < 31) {
    return 5.5;
  } else if (totalTime < 125 && minTime < 35) {
    return 5;
  } else if (totalTime < 133 && minTime < 40) {
    return 4.5;
  } else if (totalTime < 161 && minTime < 46) {
    return 4;
  } else if (totalTime < 216 && minTime < 56) {
    return 3.5;
  } else {
    return 0;  // Default value if none of the conditions are met
  }
}

void signature() {
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(YELLOWLED, HIGH);
  delay(100);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(YELLOWLED, LOW);
  delay(100);
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(YELLOWLED, HIGH);
  delay(100);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(YELLOWLED, LOW);
  delay(100);
}

void y_blink() {
  digitalWrite(YELLOWLED, HIGH);
  delay(200);
  digitalWrite(YELLOWLED, LOW);
  delay(200);
}

void yellow_and_beep() {
  digitalWrite(YELLOWLED, HIGH);
  digitalWrite(buzzerPin, HIGH);
  delay(100);
  digitalWrite(YELLOWLED, LOW);
  digitalWrite(buzzerPin, LOW);
}


void beep() {
  digitalWrite(buzzerPin, HIGH);
  delay(100);
  digitalWrite(buzzerPin, LOW);
}


void setup() {
  Serial.begin(9600);

  // Initialize the LDRs
  for (int i = 0; i < sizeof(ldrPins) / sizeof(ldrPins[0]); i++) {
    pinMode(ldrPins[i], INPUT_PULLUP);
  }

  // Initialize the buzzer and LEDs as output pins
  pinMode(buzzerPin, OUTPUT);
  pinMode(YELLOWLED, OUTPUT);

  digitalWrite(buzzerPin, LOW);
  digitalWrite(YELLOWLED, LOW);

  signature();
  delay(500);
  digitalWrite(buzzerPin, HIGH);
  delay(200);
  digitalWrite(buzzerPin, LOW);
}

void loop() {

  // put your main code here, to run repeatedly:
  switch (readSignal()) {
    case '1':  //Test 1
      test[0] = startTest();
      sendTestTime(0);
      break;
    case '2':  //Test 2
      test[1] = startTest();
      sendTestTime(1);
      break;
    case '3':  //Test 3
      test[2] = startTest();
      sendTestTime(2);
      break;
    case '4':  //Submit
      totalTime = total();
      minTime = minimum();
      mentalAge = calculateMA();
      sendReport();
      resetPatientData();
      signature();
  }
  delay(100);
}
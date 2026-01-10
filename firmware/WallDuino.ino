#include <IRremote.hpp>
#include <stdint.h>

#define WALLDUINO_VERSION 1.0

#define IR_RECEIVE_PIN 4
#define EN_PIN 6
#define A1_PIN 7
#define A2_PIN 8
#define POSITION_PIN A0
#define MOTOR_DIRECTION_PIN 9

const float MIN_POSITION = 20.0f;          // [deg]
const float MAX_POSITION = 250.0f;         // [deg]
const float MAX_MOVEMENT = 5.0f;           // Maximum degrees per button press
const float POSITION_TOLERANCE = 1.0f;     // [deg]
const unsigned long STOP_DELAY = 100;      // delay after motor stop [ms]
const unsigned long PRESET_TIMEOUT = 500;  // between consecutive clicks [ms]

// IR Remote codes

const uint8_t LEFT_COMMANDS[] = {
  0x4A,  // WallWizard
};

const uint8_t RIGHT_COMMANDS[] = {
  0x4E,  // WallWizard
};

const uint8_t HOME_COMMANDS[] = {
  0x4F,  // WallWizard
};

const uint8_t PRESET1_COMMANDS[] = {
  0x13,  // WallWizard
};

const uint8_t PRESET2_COMMANDS[] = {
  0x12,  // WallWizard
};

// Main logic

uint8_t preset1_counter = 0;
unsigned long preset1_timeout = 0;
float preset1_position = (MAX_POSITION + MIN_POSITION) / 2.0f;
uint8_t preset2_counter = 0;
unsigned long preset2_timeout = 0;
float preset2_position = (MAX_POSITION + MIN_POSITION) / 2.0f;

float targetPosition = -1;  // -1 means no active target

#define CHECK_COMMAND(arr) commandInArray(IrReceiver.decodedIRData.command, arr, sizeof(arr))

enum MotorState {
  STOPPED = 0,
  MOVING_LEFT = -1,
  MOVING_RIGHT = 1
};

MotorState motorState = STOPPED;

bool isMoving() {
  return motorState != STOPPED;
}

bool isMovingLeft() {
  return motorState != MOVING_LEFT;
}

bool isMovingRight() {
  return motorState != MOVING_RIGHT;
}

inline bool commandInArray(uint8_t cmd, const uint8_t* arr, uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    if (cmd == arr[i]) return true;
  }
  return false;
}

bool leftKeyPressed() {
  return CHECK_COMMAND(LEFT_COMMANDS);
}

bool rightKeyPressed() {
  return CHECK_COMMAND(RIGHT_COMMANDS);
}

bool homeKeyPressed() {
  return CHECK_COMMAND(HOME_COMMANDS);
}

bool preset1KeyPressed() {
  return CHECK_COMMAND(PRESET1_COMMANDS);
}

bool preset2KeyPressed() {
  return CHECK_COMMAND(PRESET2_COMMANDS);
}

void setup() {
  Serial.begin(115200);
  Serial.print("Starting WallDuino version ");
  Serial.print(WALLDUINO_VERSION);

  pinMode(EN_PIN, OUTPUT);
  pinMode(A1_PIN, OUTPUT);
  pinMode(A2_PIN, OUTPUT);
  pinMode(MOTOR_DIRECTION_PIN, INPUT_PULLUP);
  motorStop();
  unsetTarget();

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

int getMotorDirection() {
  return digitalRead(MOTOR_DIRECTION_PIN) == HIGH ? 1 : -1;
}


void motorLeft() {
  if (isMovingRight()) {
    motorStop();
  }
  motorState = MOVING_LEFT;
  digitalWrite(A1_PIN, HIGH);
  digitalWrite(A2_PIN, LOW);
  digitalWrite(EN_PIN, HIGH);
}

void motorRight() {
  if (isMovingLeft()) {
    motorStop();
  }
  motorState = MOVING_RIGHT;
  digitalWrite(A1_PIN, LOW);
  digitalWrite(A2_PIN, HIGH);
  digitalWrite(EN_PIN, HIGH);
}

void motorStop() {
  digitalWrite(EN_PIN, LOW);
  delay(STOP_DELAY);
  motorState = STOPPED;
}

// Return the current position in degrees
float readPosition() {
  int raw = analogRead(POSITION_PIN);  // 0..1023
  return raw * (270.0f / 1023.0f);
}

void setTarget(float target) {
  targetPosition = target;
  clampTarget();
}

void unsetTarget() {
  targetPosition = -1;
}

bool targetIsSet() {
  return targetPosition >= 0;
}

void increaseTarget() {
  setTarget(readPosition() + MAX_MOVEMENT);
}

void decreaseTarget() {
  setTarget(readPosition() - MAX_MOVEMENT);
}

// Clamps the target position to a safe range
void clampTarget() {
  if (targetPosition < MIN_POSITION) {
    targetPosition = MIN_POSITION;
  }
  if (targetPosition > MAX_POSITION) {
    targetPosition = MAX_POSITION;
  }
}

void printTarget() {
  Serial.print("Moving from ");
  Serial.print(readPosition());
  Serial.print(" to ");
  Serial.println(targetPosition);
}

void loop() {
  if (preset1_counter > 0 && preset1_timeout != 0 && millis() - preset1_timeout >= PRESET_TIMEOUT) {
    Serial.println("Preset 1 requested");
    setTarget(preset1_position);
    printTarget();
    preset1_counter = 0;
    preset1_timeout = 0;
  }
  if (preset2_counter > 0 && preset2_timeout != 0 && millis() - preset2_timeout >= PRESET_TIMEOUT) {
    Serial.println("Preset 2 requested");
    setTarget(preset2_position);
    printTarget();
    preset2_counter = 0;
    preset2_timeout = 0;
  }

  if (targetIsSet()) {
    float currentPosition = readPosition();
    // we allow an external jumper to change the actual direction of movement
    float deltaPosition = getMotorDirection() * (targetPosition - currentPosition);

    if (abs(deltaPosition) < POSITION_TOLERANCE) {
      motorStop();
      Serial.print("Target reached: ");
      Serial.print(targetPosition);
      Serial.print(" (deviation: ");
      Serial.print(deltaPosition);
      Serial.println(")");
      unsetTarget();
    } else {
      if (deltaPosition > 0) {
        motorRight();
      } else {
        motorLeft();
      }
    }
  }

  if (IrReceiver.decode()) {
    IrReceiver.printIRResultShort(&Serial);
    auto& data = IrReceiver.decodedIRData;

    if (leftKeyPressed()) {
      Serial.println("LEFT");
      decreaseTarget();
      printTarget();
    } else if (rightKeyPressed()) {
      Serial.println("RIGHT");
      increaseTarget();
      printTarget();
    } else if (homeKeyPressed()) {
      Serial.println("STOP");
      motorStop();
    } else if (preset1KeyPressed()) {
      Serial.println("PRESET 1");
      if (millis() - preset1_timeout < PRESET_TIMEOUT && preset1_timeout != 0) {
        // Pressed again within timeout
        preset1_counter++;
        Serial.print("Preset 1 counter: ");
        Serial.println(preset1_counter);

        if (preset1_counter >= 3) {
          preset1_position = readPosition();
          Serial.print("Preset 1 saved at position: ");
          Serial.println(preset1_position);
          preset1_counter = 0;  // Reset counter
          preset1_timeout = 0;  // Reset timeout
        } else {
          preset1_timeout = millis();  // Restart timeout
        }
      } else {
        // First press or timeout expired
        preset1_counter = 1;
        preset1_timeout = millis();
      }
    } else if (preset2KeyPressed()) {
      Serial.println("PRESET 2");
      if (millis() - preset2_timeout < PRESET_TIMEOUT && preset2_timeout != 0) {
        // Pressed again within timeout
        preset2_counter++;
        Serial.print("Preset 2 counter: ");
        Serial.println(preset2_counter);

        if (preset2_counter >= 3) {
          preset2_position = readPosition();
          Serial.print("Preset 2 saved at position: ");
          Serial.println(preset2_position);
          preset2_counter = 0;  // Reset counter
          preset2_timeout = 0;  // Reset timeout
        } else {
          preset2_timeout = millis();  // Restart timeout
        }
      } else {
        // First press or timeout expired
        preset2_counter = 1;
        preset2_timeout = millis();
      }
    }

    IrReceiver.resume();
  }
}

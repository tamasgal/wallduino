#include <IRremote.hpp>
#include <EEPROM.h>
#include <stdint.h>

#define WALLDUINO_VERSION 1.0

#define IR_RECEIVE_PIN 4  // 5
#define EN_PIN 9
#define A1_PIN 11  // 3
#define A2_PIN 10  //2
#define POSITION_PIN A0
#define MOTOR_DIRECTION_PIN 6
#define MIN_PRESSES_TO_SAVE_PRESET 7  // Logitech sends 3 with each press, so let's require 7 presses

#define EEPROM_PRESET1_ADDR 0
#define EEPROM_PRESET2_ADDR 4
#define EEPROM_MAGIC_ADDR 8
#define EEPROM_MAGIC_VALUE 0x42

#define SERIAL_DEBUG 0  // Set to 1 to enable, 0 to disable

#if SERIAL_DEBUG
#define DEBUG_BEGIN(baud) Serial.begin(baud)
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINT2(x, y) Serial.print(x, y)
#else
#define DEBUG_BEGIN(baud)
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT2(x, y)
#endif

const float MIN_POSITION = 80.00f;                                           // [deg]
const float MAX_POSITION = 200.0f;                                           // [deg]
const float MAX_MOVEMENT = 10.0f;                                            // Maximum degrees per button press
const float HOME_POSITION = MAX_POSITION;                                    // Home position, when HOME is pressed
const float POSITION_TOLERANCE = 3.0f;                                       // [deg]
const unsigned long STOP_DELAY = 100;                                        // delay after motor stop [ms]
const unsigned long PRESET_TIMEOUT = 500;                                    // between consecutive clicks [ms]
const float DEFAULT_PRESET_POSITION = (MIN_POSITION + MAX_POSITION) / 2.0f;  // Default position for presets

struct RemoteSignature {
  decode_type_t protocol;
  uint16_t address;
  uint8_t command;
};

// IR Remote code signatures

const RemoteSignature LEFT_COMMANDS[] = {
  { NEC, 0x1, 0x4A },  // WallWizard
  //{SAMSUNG, 0x7, 0x6C}  // Samsung red
  { RC6, 0x0, 0x6D},    // Logitec RED
};

const RemoteSignature RIGHT_COMMANDS[] = {
  { NEC, 0x1, 0x4E },  // WallWizard
  { RC6, 0x0, 0x70},    // Logitec BLUE
};

const RemoteSignature STOP_COMMANDS[] = {
  { NEC, 0x1, 0x4F },  // WallWizard
  { RC6, 0x0, 0xA},    // Logitec EXIT
};

const RemoteSignature PRESET1_COMMANDS[] = {
  { NEC, 0x1, 0x13 },  // WallWizard
  { RC6, 0x0, 0x6F},    // Logitec YELLOW
};

const RemoteSignature PRESET2_COMMANDS[] = {
  { NEC, 0x1, 0x12 },  // WallWizard
};


const RemoteSignature HOME_COMMANDS[] = {
  { NEC, 0x1, 0xD },  // WallWizard
  { RC6, 0x0, 0x6E},    // Logitec GREEN
};

// Main logic

uint8_t preset1_counter = 0;
unsigned long preset1_timeout = 0;
float preset1_position = DEFAULT_PRESET_POSITION;
uint8_t preset2_counter = 0;
unsigned long preset2_timeout = 0;
float preset2_position = DEFAULT_PRESET_POSITION;

float targetPosition = -1;  // -1 means no active target

#define CHECK_COMMAND(arr) matchesSignature(arr, sizeof(arr) / sizeof(arr[0]), \
                                            IrReceiver.decodedIRData.protocol, \
                                            IrReceiver.decodedIRData.address, \
                                            IrReceiver.decodedIRData.command)
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
  return motorState == MOVING_LEFT;
}

bool isMovingRight() {
  return motorState == MOVING_RIGHT;
}

bool matchesSignature(const RemoteSignature* signatures, size_t count,
                      decode_type_t protocol, uint16_t address, uint8_t command) {
  for (size_t i = 0; i < count; i++) {
    if (signatures[i].protocol == protocol && signatures[i].address == address && signatures[i].command == command) {
      return true;
    }
  }
  return false;
}

bool leftKeyPressed() {
  return CHECK_COMMAND(LEFT_COMMANDS);
}

bool rightKeyPressed() {
  return CHECK_COMMAND(RIGHT_COMMANDS);
}

bool stopKeyPressed() {
  return CHECK_COMMAND(STOP_COMMANDS);
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


void writeFloatToEEPROM(int address, float value) {
  EEPROM.put(address, value);
}

float readFloatFromEEPROM(int address) {
  float value;
  EEPROM.get(address, value);
  return value;
}

void initializeEEPROM() {
  uint8_t magic = EEPROM.read(EEPROM_MAGIC_ADDR);
  if (magic != EEPROM_MAGIC_VALUE) {
    // EEPROM not initialized, write defaults
    Serial.println("Initializing EEPROM with default values...");
    writeFloatToEEPROM(EEPROM_PRESET1_ADDR, DEFAULT_PRESET_POSITION);
    writeFloatToEEPROM(EEPROM_PRESET2_ADDR, DEFAULT_PRESET_POSITION);
    EEPROM.write(EEPROM_MAGIC_ADDR, EEPROM_MAGIC_VALUE);
    preset1_position = DEFAULT_PRESET_POSITION;
    preset2_position = DEFAULT_PRESET_POSITION;
    Serial.print("Default preset positions set to: ");
    Serial.println(DEFAULT_PRESET_POSITION);
  } else {
    // EEPROM already initialized, read values
    Serial.println("Loading preset positions from EEPROM...");
    preset1_position = readFloatFromEEPROM(EEPROM_PRESET1_ADDR);
    preset2_position = readFloatFromEEPROM(EEPROM_PRESET2_ADDR);
    Serial.print("Preset 1 position: ");
    Serial.println(preset1_position);
    Serial.print("Preset 2 position: ");
    Serial.println(preset2_position);
  }
}

void savePreset1Position(float position) {
  preset1_position = position;
  writeFloatToEEPROM(EEPROM_PRESET1_ADDR, position);
  Serial.print("Preset 1 saved to EEPROM at position: ");
  Serial.println(preset1_position);
}

void savePreset2Position(float position) {
  preset2_position = position;
  writeFloatToEEPROM(EEPROM_PRESET2_ADDR, position);
  Serial.print("Preset 2 saved to EEPROM at position: ");
  Serial.println(preset2_position);
}

void setup() {
  Serial.begin(115200);
#if SERIAL_DEBUG
    delay(3000);  // give the serial monitor time to open and see the very first messages
#endif

  Serial.print("\nStarting WallDuino version ");
  Serial.println(WALLDUINO_VERSION);

  pinMode(EN_PIN, OUTPUT);
  pinMode(A1_PIN, OUTPUT);
  pinMode(A2_PIN, OUTPUT);
  pinMode(MOTOR_DIRECTION_PIN, INPUT_PULLUP);
  motorStop();
  unsetTarget();
  initializeEEPROM();

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

int getMotorDirection() {
  return digitalRead(MOTOR_DIRECTION_PIN) == HIGH ? 1 : -1;
}


void motorLeft() {
  if (isMovingRight()) {
    DEBUG_PRINTLN("Motor is moving right, stopping first.");
    motorStop();
    Serial.println("Now moving left.");
  }
  motorState = MOVING_LEFT;
  digitalWrite(A1_PIN, HIGH);
  digitalWrite(A2_PIN, LOW);
  digitalWrite(EN_PIN, HIGH);
}

void motorRight() {
  if (isMovingLeft()) {
    DEBUG_PRINTLN("Motor is moving left, stopping first.");
    motorStop();
    Serial.println("Now moving right.");
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
  Serial.print("Motor stopped at ");
  float pos = readPosition();
  Serial.println(pos);
}

// Return the current position in degrees
float readPosition() {
  int raw = analogRead(POSITION_PIN);  // 0..1023
  return raw * (270.0f / 1023.0f);
}

void setTarget(float target) {
  targetPosition = clampPosition(target);
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

// Clamps the position to a safe range
float clampPosition(float position) {
  if (position < MIN_POSITION) {
    return MIN_POSITION;
  }
  if (position > MAX_POSITION) {
    return MAX_POSITION;
  }
  return position;
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
#if SERIAL_DEBUG
    IrReceiver.printIRResultShort(&Serial);
#endif
    auto& data = IrReceiver.decodedIRData;

    if (leftKeyPressed()) {
      Serial.println("LEFT");
      decreaseTarget();
      printTarget();
    } else if (rightKeyPressed()) {
      Serial.println("RIGHT");
      increaseTarget();
      printTarget();
    } else if (stopKeyPressed()) {
      Serial.println("STOP");
      unsetTarget();
      motorStop();
    } else if (homeKeyPressed()) {
      Serial.println("STOP");
      setTarget(HOME_POSITION);
      Serial.print("Going home: ");
      Serial.println(HOME_POSITION);
    } else if (preset1KeyPressed()) {
      Serial.println("PRESET 1");
      if (millis() - preset1_timeout < PRESET_TIMEOUT && preset1_timeout != 0) {
        // Pressed again within timeout
        preset1_counter++;
        Serial.print("Preset 1 counter: ");
        Serial.println(preset1_counter);

        if (preset1_counter >= MIN_PRESSES_TO_SAVE_PRESET) {
          savePreset1Position(readPosition());
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

        if (preset2_counter >= MIN_PRESSES_TO_SAVE_PRESET) {
          savePreset2Position(readPosition());
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

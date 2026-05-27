// Automatic Door Locking System with Fingerprint Authentication
// Arduino UNO | Fingerprint Scanner | Relay | Electronic Door Lock (12V) | Li-ion Battery
// Ramco Institute of Technology - Kiruthika R - 6th Semester 2026

#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX for fingerprint sensor
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

const int relayPin = 7;  // Relay connected to pin 7 (controls 12V door lock)
const int ledPin = 9;    // Green LED - access granted
const int buzzerPin = 8; // Buzzer - access denied

void setup() {
  Serial.begin(9600);
  finger.begin(57600);
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Door locked by default

  if (finger.verifyPassword()) {
    Serial.println("Fingerprint sensor found!");
  } else {
    Serial.println("Fingerprint sensor not found!");
    while (1);
  }
  Serial.println("System Ready - Place finger to unlock...");
}

int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;

  return finger.fingerID;
}

void loop() {
  int id = getFingerprintID();

  if (id > 0) {
    // Fingerprint matched - unlock door
    Serial.print("Access Granted! ID: ");
    Serial.println(id);
    digitalWrite(relayPin, HIGH); // Relay ON - activates 12V door lock
    digitalWrite(ledPin, HIGH);
    delay(5000); // Door open for 5 seconds
    digitalWrite(relayPin, LOW); // Lock again
    digitalWrite(ledPin, LOW);
    Serial.println("Door Locked Again.");
  } else {
    // No match - deny access
    digitalWrite(buzzerPin, HIGH);
    Serial.println("Access Denied!");
    delay(1000);
    digitalWrite(buzzerPin, LOW);
  }
  delay(500);
}

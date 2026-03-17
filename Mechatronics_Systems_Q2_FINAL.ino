/*
 * ============================================================
 *
 * Name: Christopher Redfern
 * BEng Electrical and Electronics Engineering
 * University of Lincoln
 * ELE2003M - Mechatronics Systems
 * Question 2: Automatic Light Control System
 *
 * ============================================================
 *
 * Description:
 *   Automatically switches an LED on when the environment
 *   becomes dark, using an LDR (Light Dependent Resistor)
 *   in a voltage divider circuit.
 *
 *   Potentiometer sets threshold level (displayed as 0-100%)
 *   I2C LCD displays ambient light level and threshold as %
 *
 * Hardware:
 *   - Arduino (Freenove)
 *   - LDR (one leg to 5V, other leg to A0 + 10k to GND)
 *   - LED -> Pin 9 (via 220 ohm resistor to GND)
 *   - Potentiometer -> A1
 *   - I2C LCD 1602 -> SDA=A4, SCL=A5
 *
 * Threshold Logic:
 *   lightLevel < threshold = DARK  -> LED ON
 *   lightLevel > threshold = BRIGHT -> LED OFF
 * ============================================================
 */

#include <LiquidCrystal_I2C.h>

// --- I2C LCD Setup ---
// Address 0x27 is most common — if screen is blank try 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- Pin Definitions ---
const int LDR_PIN = A0;  // Analogue input from LDR voltage divider
const int POT_PIN = A1;  // Potentiometer for threshold setting
const int LED_PIN = 9;   // Output LED (PWM capable pin)

// --- Variable name to match Scott's code ---
const int res = A0;      // Alias for LDR_PIN used in Scott's LCD section

// --- Default Threshold ---
int threshold = 500;


// ============================================================
// SETUP - Runs once when Arduino powers on
// ============================================================
void setup() {
  pinMode(LED_PIN, OUTPUT);

  // Initialise I2C LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Light Control");
  lcd.setCursor(0, 1);
  lcd.print("System Ready...");
  delay(2000);
  lcd.clear();

  // Start Serial Monitor
  Serial.begin(9600);
  Serial.println("Automatic Light Control System Started");
  Serial.println("Reading LDR on A0...");
}


// ============================================================
// LOOP - Runs repeatedly after setup()
// ============================================================
void loop() {

  // Step 1: Read raw values
  int lightLevel = analogRead(LDR_PIN);
  int potval     = analogRead(POT_PIN);  // potval matches Scott's variable name

  // Step 2: Set threshold from raw pot value
  threshold = potval;

  // Step 3: Scott's LCD code — unchanged
  int percent = map(potval, 0, 1023, 0, 100);
  int ambientlevel = map(analogRead(res), 0, 900, 0, 100);
  ambientlevel = constrain(ambientlevel, 0, 100);   // I've added constrain () to prevent the ambient reading exceeding 100% under very bright conditions
  lcd.setCursor(0,0);
  lcd.print("Ambient: ");
  lcd.print(ambientlevel);
  lcd.print(char(0x25));
  lcd.print(" ");
  lcd.setCursor(0,1);
  lcd.print("Set: ");
  lcd.print(percent);
  lcd.print(char(0x25));
  lcd.print(" ");

  // Step 4: Serial Monitor output
  Serial.print("Light Level: ");
  Serial.print(lightLevel);
  Serial.print(" (");
  Serial.print(ambientlevel);
  Serial.print("%) | Threshold: ");
  Serial.print(threshold);
  Serial.print(" (");
  Serial.print(percent);
  Serial.print("%) | Status: ");

  // Step 5: Compare and control LED
  if (lightLevel < threshold) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("DARK -> LED ON");
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("BRIGHT -> LED OFF");
  }

  // Step 6: Small delay before next reading
  delay(200);
}

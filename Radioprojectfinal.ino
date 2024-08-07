#include <Wire.h>
#include <TEA5767Radio.h>
#include <Encoder.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// OLED I2C address
#define OLED_ADDRESS 0x3C








// Define pins for the rotary encoder
#define ENCODER_PIN_A 2
#define ENCODER_PIN_B 3

// Initialize the radio and the encoder
TEA5767Radio radio = TEA5767Radio();
Encoder encoder(ENCODER_PIN_A, ENCODER_PIN_B);

// Variables to store the frequency and previous encoder position
float frequency = 93.0;
float frequencyStep = 0.025; // Change in frequency per encoder tick
long lastEncoderPosition = 0;

void setup() 
{ 
  Wire.begin();
  radio.setFrequency(frequency);

  // Initialize serial communication
  Serial.begin(9600);

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Infinite loop to halt execution
  }

  // Clear the display buffer
  display.clearDisplay();

}

void loop() 
{
  // Get the current position of the encoder
  long newEncoderPosition = encoder.read();
  
  // Check if the encoder value has changed
  if (newEncoderPosition != lastEncoderPosition) {
    long positionChange = newEncoderPosition - lastEncoderPosition;
    frequency += positionChange * frequencyStep;
    lastEncoderPosition = newEncoderPosition; // Update the last position
    
    // Ensure the frequency is within the valid range (e.g., 87.5 to 108.0 MHz)
    if (frequency < 87.5) frequency = 87.5;
    if (frequency > 108.0) frequency = 108.0;
    
    // Set the new frequency on the radio
    radio.setFrequency(frequency);

  display.clearDisplay();

  // Set text size, color, and cursor position
    

    
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(0, 15);     
  display.setTextSize(3); 
  display.print(frequency);

  
  display.display();


  }
  
  delay(50); // Add a small delay to avoid excessive polling
}

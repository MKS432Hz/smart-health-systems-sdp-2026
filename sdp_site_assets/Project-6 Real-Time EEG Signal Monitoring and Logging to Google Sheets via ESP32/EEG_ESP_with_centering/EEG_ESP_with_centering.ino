#define SAMPLE_RATE 256             // Sampling rate in Hz
#define BAUD_RATE 9600             // Serial speed
#define INPUT_PIN 34                 // Use ADC1 channel on GPIO34
#define ADC_RESOLUTION 12            // ESP32 max ADC resolution (12-bit)
#define V_REF 3.3                    // ADC reference voltage in volts
#define ADC_MID 2048                 // Midpoint of 12-bit ADC (virtual 0V)

void setup() {
  Serial.begin(BAUD_RATE);
  analogReadResolution(ADC_RESOLUTION);  // Set ADC to 12-bit (0–4095)
}

void loop() {
  static unsigned long lastMicros = 0; 
  unsigned long now = micros();
  unsigned long interval = 1000000UL / SAMPLE_RATE;

  if (now - lastMicros >= interval) {
    lastMicros = now;

    int raw_adc = analogRead(INPUT_PIN);                    // Read from ADC
    float signal = raw_adc - ADC_MID;                       // Remove 1.65V bias (simulate 0V-centered signal)
    float filtered = EEGFilter(signal);                     // Apply digital filter
    Serial.println(filtered, 2);                            // Output filtered signal
  }
}

// -------------------- Digital IIR Bandpass Filter --------------------
// Designed in cascaded biquad form, cutoff ~1–50 Hz, fs = 500 Hz
float EEGFilter(float input) {
  float output = input;

  {
    static float z1, z2;
    float x = output - -0.95391350 * z1 - 0.25311356 * z2;
    output = 0.00735282 * x + 0.01470564 * z1 + 0.00735282 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2;
    float x = output - -1.20596630 * z1 - 0.60558332 * z2;
    output = 1.00000000 * x + 2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2;
    float x = output - -1.97690645 * z1 - 0.97706395 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2;
    float x = output - -1.99071687 * z1 - 0.99086813 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }

  return output;
}

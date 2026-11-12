#define SAMPLE_RATE 500
#define BAUD_RATE 9600

// Define the input pin for the EEG sensor
#define INPUT_PIN {A0}

void setup() {
  Serial.begin(BAUD_RATE);  // Initialize serial communication
}

void loop() {
  static unsigned long past = 0;
  unsigned long present = micros();
  unsigned long interval = present - past;
  past = present;

  static long timer = 0;
  timer -= interval;

  if (timer < 0) {
    timer += 1000000 / SAMPLE_RATE;

    // Read and filter data from A0 sensor
    float sensor_value = analogRead(INPUT_PIN); // Read sensor value
    float filtered_signal = EEGFilter(sensor_value); // Apply filtering
    Serial.println(filtered_signal, 2); // Print value with 2 decimal places
  }
}

// Band-Pass Butterworth IIR digital filter
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

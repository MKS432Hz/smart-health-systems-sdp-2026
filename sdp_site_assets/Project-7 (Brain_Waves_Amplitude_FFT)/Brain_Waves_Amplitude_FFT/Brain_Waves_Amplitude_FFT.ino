#include <arduinoFFT.h>

#define SAMPLES 128
#define SAMPLING_FREQUENCY 256
#define ANALOG_PIN 34
#define VREF 3.3
#define ADC_BITS 12
#define ADC_CENTER 2048
#define SERIAL_BAUD 115200

double vReal[SAMPLES];
double vImag[SAMPLES];

// Instantiate FFT object with buffers and parameters
ArduinoFFT<double> fft = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY);

unsigned int sampling_period_us;

void setup() {
  Serial.begin(SERIAL_BAUD);
  analogReadResolution(ADC_BITS);
  sampling_period_us = round(1000000.0 / SAMPLING_FREQUENCY);
  delay(1000);
}

void loop() {
  // === 1. Sample EEG signal ===
  for (int i = 0; i < SAMPLES; i++) {
    unsigned long startMicros = micros();

    int raw = analogRead(ANALOG_PIN);
    int centered = raw - ADC_CENTER;
    double voltage = (centered * VREF) / pow(2, ADC_BITS);
    vReal[i] = voltage;
    vImag[i] = 0;

    while (micros() - startMicros < sampling_period_us) {
      // Wait for next sample
    }
  }

  // === 2. Perform FFT (correct lowercase API) ===
  fft.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  fft.compute(FFT_FORWARD);
  fft.complexToMagnitude();

  // === 3. Frequency band amplitudes ===
  double delta = getBandAmplitude(vReal, 0.5, 4);
  double theta = getBandAmplitude(vReal, 4, 8);
  double alpha = getBandAmplitude(vReal, 8, 13);
  double beta  = getBandAmplitude(vReal, 13, 30);
  double gamma = getBandAmplitude(vReal, 30, 100);

  // === 4. Serial Plotter display ===
  Serial.print("Delta: "); Serial.print(delta); Serial.print("\t");
  Serial.print("Theta: "); Serial.print(theta); Serial.print("\t");
  Serial.print("Alpha: "); Serial.print(alpha); Serial.print("\t");
  Serial.print("Beta: ");  Serial.print(beta); Serial.print("\t");
  Serial.print("Gamma: "); Serial.println(gamma);

  delay(10);
}

// === Frequency band amplitude calculation ===
double getBandAmplitude(double* vData, double lowFreq, double highFreq) {
  uint16_t lowIndex  = (lowFreq * SAMPLES) / SAMPLING_FREQUENCY;
  uint16_t highIndex = (highFreq * SAMPLES) / SAMPLING_FREQUENCY;

  double sum = 0;
  for (uint16_t i = lowIndex; i <= highIndex; i++) {
    sum += vData[i];
  }
  return sum / (highIndex - lowIndex + 1);
}

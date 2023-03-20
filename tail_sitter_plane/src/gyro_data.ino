// // Gesture classification is a simple and at the same time great example of what machine learning can do. It uses lots of "messy" data to classify things.

// // In this project we will make a classifier with 4 classes, idle, up_down, left_right and circle. This project will be made using an Esp32 board($8), because it is very cheap when compared with the "classic" example using the Arduino Nano 33 BLE Sense($35).

// // Data Acquisition
// // To upload data to the Edge Impulse we need to use the Edge Impulse CLI, follow this tutorial to see how to install it.

// // The tool we are going to use is the Data Forwarder, this tool uploads the data available in the serial to the Edge Impulse. We need to use an Arduino Sketch to upload data to the serial with some sample frequency and with ", " to separate the data, in this case we will be sending acceleration data.

// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>
// #include <Wire.h>
// #define FREQUENCY_HZ        60
// #define INTERVAL_MS         (1000 / (FREQUENCY_HZ + 1))
// // objeto da classe Adafruit_MPU6050
// Adafruit_MPU6050 mpu;
// static unsigned long last_interval_ms = 0;
// void setup() {
// Serial.begin(115200);
// Serial.println("Classificador de gestos com TinyML");
// // Try to initialize!
// if (!mpu.begin()) {
// Serial.println("Failed to find MPU6050 chip");
// while (1) {
// delay(10);
// }
// }
// Serial.println("MPU6050 Found!");
// mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
// Serial.print("Accelerometer range set to: ");
// switch (mpu.getAccelerometerRange()) {
// case MPU6050_RANGE_2_G:
// Serial.println("+-2G");
// break;
// case MPU6050_RANGE_4_G:
// Serial.println("+-4G");
// break;
// case MPU6050_RANGE_8_G:
// Serial.println("+-8G");
// break;
// case MPU6050_RANGE_16_G:
// Serial.println("+-16G");
// break;
// }
// mpu.setGyroRange(MPU6050_RANGE_500_DEG);
// Serial.print("Gyro range set to: ");
// switch (mpu.getGyroRange()) {
// case MPU6050_RANGE_250_DEG:
// Serial.println("+- 250 deg/s");
// break;
// case MPU6050_RANGE_500_DEG:
// Serial.println("+- 500 deg/s");
// break;
// case MPU6050_RANGE_1000_DEG:
// Serial.println("+- 1000 deg/s");
// break;
// case MPU6050_RANGE_2000_DEG:
// Serial.println("+- 2000 deg/s");
// break;
// }
// mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
// Serial.print("Filter bandwidth set to: ");
// switch (mpu.getFilterBandwidth()) {
// case MPU6050_BAND_260_HZ:
// Serial.println("260 Hz");
// break;
// case MPU6050_BAND_184_HZ:
// Serial.println("184 Hz");
// break;
// case MPU6050_BAND_94_HZ:
// Serial.println("94 Hz");
// break;
// case MPU6050_BAND_44_HZ:
// Serial.println("44 Hz");
// break;
// case MPU6050_BAND_21_HZ:
// Serial.println("21 Hz");
// break;
// case MPU6050_BAND_10_HZ:
// Serial.println("10 Hz");
// break;
// case MPU6050_BAND_5_HZ:
// Serial.println("5 Hz");
// break;
// }
// Serial.println("");
// delay(100);
// }
// void loop() {
// sensors_event_t a, g, temp;
// if (millis() > last_interval_ms + INTERVAL_MS) {
// last_interval_ms = millis();
// mpu.getEvent(&a, &g, &temp);
// Serial.print(a.acceleration.x);
// Serial.print(",");
// Serial.print(a.acceleration.y);
// Serial.print(",");
// Serial.println(a.acceleration.z);
// }
// }
// After the Esp32 board is ready to send data we need to connect our Edge Impulse account with the CLI, it should like this:

// It is important to understand the limitations of this method, we can´t use it to upload audio data for example, since the sampling frequency should be at least two times higher then the maximum frequency of the signal( Nyquist sampling theorem) and we can´t transmit data at such high frequency using the serial.
// After that you can start to gather some data, you can do a lot of moviments, just make sure that they are not much alike( you will be able to decide this in the feature explorer later).

// Designing the Impulse
// An impulse is the set of acquisition, processing and inferencing data.

// The spectral analysis creates features for the Keras model to use, in this stage the signal is filtered and generates two spectrums one of frequency( using FFT) and other of energy( PSD)

// We can see how well our classes are separated using the feature explorer:

// They look very distinct, so we should get a good result with the model.

// This data goes to the Neural Network, since this is a simple task the neural network should be simple as well.

// After we train the model we can see the performance in the confusion matrix:

// It looks pretty good, but the performace will probably lower with the test data.

// After you are satisfied with your model you can embed it using de Arduino IDE and the library generated in the Edge Impulse.

// Just like another library in the Arduino we need to include it and use their functions to do an inference.

// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>
// #include <Wire.h>
// #include <gesture_class_ESP32_dataForwarder_inferencing.h>

// #define FREQUENCY_HZ        60
// #define INTERVAL_MS         (1000 / (FREQUENCY_HZ + 1))
// #define RED 16
// #define GREEN 17
// #define BLUE 18

// // objeto da classe Adafruit_MPU6050
// Adafruit_MPU6050 mpu;

// float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
// size_t feature_ix = 0;

// static unsigned long last_interval_ms = 0;


// void setup() {
//   Serial.begin(115200);

//   ledcSetup(0, 5000, 8);
//   /*
//     ledcAttachPin(RED, 0);
//     ledcAttachPin(GREEN, 1);
//     ledcAttachPin(BLUE, 2);
//   */

//   pinMode(RED, OUTPUT);
//   pinMode(GREEN, OUTPUT);
//   pinMode(BLUE, OUTPUT);



//   if (!mpu.begin()) {
//     Serial.println("Failed to find MPU6050 chip");
//     while (1) {
//       delay(10);
//     }
//   }
//   Serial.println("MPU6050 Found!");

//   mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
//   Serial.print("Accelerometer range set to: ");
//   switch (mpu.getAccelerometerRange()) {
//     case MPU6050_RANGE_2_G:
//       Serial.println("+-2G");
//       break;
//     case MPU6050_RANGE_4_G:
//       Serial.println("+-4G");
//       break;
//     case MPU6050_RANGE_8_G:
//       Serial.println("+-8G");
//       break;
//     case MPU6050_RANGE_16_G:
//       Serial.println("+-16G");
//       break;
//   }
//   mpu.setGyroRange(MPU6050_RANGE_500_DEG);
//   Serial.print("Gyro range set to: ");
//   switch (mpu.getGyroRange()) {
//     case MPU6050_RANGE_250_DEG:
//       Serial.println("+- 250 deg/s");
//       break;
//     case MPU6050_RANGE_500_DEG:
//       Serial.println("+- 500 deg/s");
//       break;
//     case MPU6050_RANGE_1000_DEG:
//       Serial.println("+- 1000 deg/s");
//       break;
//     case MPU6050_RANGE_2000_DEG:
//       Serial.println("+- 2000 deg/s");
//       break;
//   }

//   mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
//   Serial.print("Filter bandwidth set to: ");
//   switch (mpu.getFilterBandwidth()) {
//     case MPU6050_BAND_260_HZ:
//       Serial.println("260 Hz");
//       break;
//     case MPU6050_BAND_184_HZ:
//       Serial.println("184 Hz");
//       break;
//     case MPU6050_BAND_94_HZ:
//       Serial.println("94 Hz");
//       break;
//     case MPU6050_BAND_44_HZ:
//       Serial.println("44 Hz");
//       break;
//     case MPU6050_BAND_21_HZ:
//       Serial.println("21 Hz");
//       break;
//     case MPU6050_BAND_10_HZ:
//       Serial.println("10 Hz");
//       break;
//     case MPU6050_BAND_5_HZ:
//       Serial.println("5 Hz");
//       break;
//   }

//   Serial.println("");
//   delay(100);

//   Serial.print("Features: ");
//   Serial.println(EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
//   Serial.print("Label count: ");
//   Serial.println(EI_CLASSIFIER_LABEL_COUNT);

// }

// void loop() {
//   sensors_event_t a, g, temp;

//   if (millis() > last_interval_ms + INTERVAL_MS) {
//     last_interval_ms = millis();

//     mpu.getEvent(&a, &g, &temp);

//     features[feature_ix++] = a.acceleration.x;
//     features[feature_ix++] = a.acceleration.y;
//     features[feature_ix++] = a.acceleration.z;

//     if (feature_ix == EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
//       Serial.println("Running the inference...");
//       signal_t signal;
//       ei_impulse_result_t result;
//       int err = numpy::signal_from_buffer(features, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
//       if (err != 0) {
//         ei_printf("Failed to create signal from buffer (%d)\n", err);
//         return;
//       }

//       EI_IMPULSE_ERROR res = run_classifier(&signal, &result, true);

//       if (res != 0) return;

//       ei_printf("Predictions ");
//       ei_printf("(DSP: %d ms., Classification: %d ms.)",
//                 result.timing.dsp, result.timing.classification);
//       ei_printf(": \n");

//       for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
//         ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
//         if (result.classification[ix].value > 0.6) {
//           if (result.classification[ix].label == "up_down")
//           {
//             /*// color code #00C9CC (R = 0,   G = 201, B = 204)
//               //analogWrite(RED, 0);
//               ledcWrite(0, 0);
//               //analogWrite(GREEN, 201);
//               ledcWrite(1, 201);
//               //analogWrite(BLUE, 204);
//               ledcWrite(2, 204);
//               Serial.println("up and down");*/
//             digitalWrite(RED, HIGH);
//             digitalWrite(GREEN, LOW);
//             digitalWrite(BLUE, LOW);
//           } else if (result.classification[ix].label == "left_right")
//           {
//             digitalWrite(RED, LOW);
//             digitalWrite(GREEN, HIGH);
//             digitalWrite(BLUE, LOW);
//           } else if (result.classification[ix].label == "circle")
//           {
//             digitalWrite(RED, LOW);
//             digitalWrite(GREEN, LOW);
//             digitalWrite(BLUE, HIGH);
//           } else
//           {
//             digitalWrite(RED, LOW);
//             digitalWrite(GREEN, LOW);
//             digitalWrite(BLUE, LOW);
//           }
//         }
//       }
//       feature_ix = 0;
//     }

//   }
// }

// void ei_printf(const char *format, ...) {
//   static char print_buf[1024] = { 0 };

//   va_list args;
//   va_start(args, format);
//   int r = vsnprintf(print_buf, sizeof(print_buf), format, args);
//   va_end(args);

//   if (r > 0) {
//     Serial.write(print_buf);
//   }
// }
// In this application I am using a RGB led to show what class was detected, you can also see in the serial monitor.

// Schematics
// MPU6050 connection
// Source:
// https://randomnerdtutorials.com/esp32-mpu-6050-accelerometer-gyroscope-arduino/

// Code
// Inference
// Data Acquisition
// InferenceC/C++
// Inferencing data using Arduino IDE and Esp32

// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>
// #include <Wire.h>
// #include <gesture_class_ESP32_dataForwarder_inferencing.h>

// #define FREQUENCY_HZ        60
// #define INTERVAL_MS         (1000 / (FREQUENCY_HZ + 1))

// // objeto da classe Adafruit_MPU6050
// Adafruit_MPU6050 mpu;

// float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
// size_t feature_ix = 0;

// static unsigned long last_interval_ms = 0;


// void setup() {
//   Serial.begin(115200);

//   if (!mpu.begin()) {
//     Serial.println("Failed to find MPU6050 chip");
//     while (1) {
//       delay(10);
//     }
//   }
//   Serial.println("MPU6050 Found!");

//   mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
//   Serial.print("Accelerometer range set to: ");
//   switch (mpu.getAccelerometerRange()) {
//     case MPU6050_RANGE_2_G:
//       Serial.println("+-2G");
//       break;
//     case MPU6050_RANGE_4_G:
//       Serial.println("+-4G");
//       break;
//     case MPU6050_RANGE_8_G:
//       Serial.println("+-8G");
//       break;
//     case MPU6050_RANGE_16_G:
//       Serial.println("+-16G");
//       break;
//   }
//   mpu.setGyroRange(MPU6050_RANGE_500_DEG);
//   Serial.print("Gyro range set to: ");
//   switch (mpu.getGyroRange()) {
//     case MPU6050_RANGE_250_DEG:
//       Serial.println("+- 250 deg/s");
//       break;
//     case MPU6050_RANGE_500_DEG:
//       Serial.println("+- 500 deg/s");
//       break;
//     case MPU6050_RANGE_1000_DEG:
//       Serial.println("+- 1000 deg/s");
//       break;
//     case MPU6050_RANGE_2000_DEG:
//       Serial.println("+- 2000 deg/s");
//       break;
//   }

//   mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
//   Serial.print("Filter bandwidth set to: ");
//   switch (mpu.getFilterBandwidth()) {
//     case MPU6050_BAND_260_HZ:
//       Serial.println("260 Hz");
//       break;
//     case MPU6050_BAND_184_HZ:
//       Serial.println("184 Hz");
//       break;
//     case MPU6050_BAND_94_HZ:
//       Serial.println("94 Hz");
//       break;
//     case MPU6050_BAND_44_HZ:
//       Serial.println("44 Hz");
//       break;
//     case MPU6050_BAND_21_HZ:
//       Serial.println("21 Hz");
//       break;
//     case MPU6050_BAND_10_HZ:
//       Serial.println("10 Hz");
//       break;
//     case MPU6050_BAND_5_HZ:
//       Serial.println("5 Hz");
//       break;
//   }

//   Serial.println("");
//   delay(100);

//   Serial.print("Features: ");
//   Serial.println(EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
//   Serial.print("Label count: ");
//   Serial.println(EI_CLASSIFIER_LABEL_COUNT);

// }

// void loop() {
//   sensors_event_t a, g, temp;

//   if (millis() > last_interval_ms + INTERVAL_MS) {
//     last_interval_ms = millis();

//     mpu.getEvent(&a, &g, &temp);

//     features[feature_ix++] = a.acceleration.x;
//     features[feature_ix++] = a.acceleration.y;
//     features[feature_ix++] = a.acceleration.z;

//     if (feature_ix == EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
//       Serial.println("Running the inference...");
//       signal_t signal;
//       ei_impulse_result_t result;
//       int err = numpy::signal_from_buffer(features, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
//       if (err != 0) {
//         ei_printf("Failed to create signal from buffer (%d)\n", err);
//         return;
//       }

//       EI_IMPULSE_ERROR res = run_classifier(&signal, &result, true);

//       if (res != 0) return;

//       ei_printf("Predictions ");
//       ei_printf("(DSP: %d ms., Classification: %d ms.)",
//                 result.timing.dsp, result.timing.classification);
//       ei_printf(": \n");

//       for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
//         ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
//       }
//       feature_ix = 0;
//     }

//   }
// }

// void ei_printf(const char *format, ...) {
//   static char print_buf[1024] = { 0 };

//   va_list args;
//   va_start(args, format);
//   int r = vsnprintf(print_buf, sizeof(print_buf), format, args);
//   va_end(args);

//   if (r > 0) {
//     Serial.write(print_buf);
//   }
// }
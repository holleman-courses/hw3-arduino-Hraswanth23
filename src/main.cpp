#include <Arduino.h>
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "sin_predictor_model_data.h" // Model data header

// Define TensorFlow Lite model version
#define TENSORFLOW_LITE_VERSION_MAJOR 2
#define TENSORFLOW_LITE_VERSION_MINOR 4
#define TENSORFLOW_LITE_VERSION_PATCH 0

// Function prototype
void invoke_tflite_model();

// TensorFlow Lite global variables
constexpr int tensor_arena_size = 128 * 1024; // Adjust memory as needed
uint8_t tensor_arena[tensor_arena_size];

tflite::MicroInterpreter* interpreter; 

void setup() {
    Serial.begin(115200);
    
    while (!Serial) {
        ; // Wait for Serial Monitor to open
    }

    Serial.println("Starting TensorFlow Lite Model on Arduino...");

    // Initialize TFLite model
    static tflite::AllOpsResolver resolver;
    const tflite::Model* model = tflite::GetModel(sin_predictor_tflite);

    if (model->version() != TFLITE_SCHEMA_VERSION) {
        Serial.println("Model version mismatch!");
        return;
    }

    static tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, tensor_arena_size);
    interpreter = &static_interpreter;
    
    if (interpreter->AllocateTensors() != kTfLiteOk) {
        Serial.println("Tensor allocation failed!");
        return;
    } else {
        Serial.println("Tensor allocation successful.");
    }

    Serial.println("Model Initialization Complete!");
}

void loop() {
    delay(1000);  // Delay between inference runs

    // Measure printing time
    unsigned long t0 = micros();
    Serial.println("Running inference...");
    unsigned long t1 = micros();

    // Perform inference
    invoke_tflite_model();
    unsigned long t2 = micros();

    unsigned long t_print = t1 - t0;
    unsigned long t_infer = t2 - t1;

    // Print timing results
    Serial.print("Printing time = ");
    Serial.print(t_print);
    Serial.println(" us");

    Serial.print("Inference time = ");
    Serial.print(t_infer);
    Serial.println(" us");

    Serial.println("--------------------");
}

// Function to invoke TensorFlow Lite model
void invoke_tflite_model() {
    Serial.println("DEBUG: Inside invoke_tflite_model()");
    
    if (interpreter == nullptr) {
        Serial.println("ERROR: Interpreter is NULL!");
        return;
    }

    Serial.println("DEBUG: Allocating Tensors...");
    if (interpreter->AllocateTensors() != kTfLiteOk) {
        Serial.println("ERROR: Tensor allocation failed!");
        return;
    }

    Serial.println("DEBUG: Running inference...");
    if (interpreter->Invoke() != kTfLiteOk) {
        Serial.println("ERROR: Inference failed!");
        return;
    }

    Serial.println("DEBUG: Inference completed, checking output...");
}
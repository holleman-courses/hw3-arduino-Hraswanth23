#ifndef SIN_PREDICTOR_MODEL_DATA_H
#define SIN_PREDICTOR_MODEL_DATA_H

#include <Arduino.h>
#include <cstdint>

// Declare the model data array as `extern`
extern const unsigned char sin_predictor_tflite[];
extern const unsigned int sin_predictor_tflite_len;

#endif // SIN_PREDICTOR_MODEL_DATA_H
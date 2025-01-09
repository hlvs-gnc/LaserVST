/**
 * @file params.h
 *
 * @brief Parameter definitions for the Laser VST Plugin.
 *
 * This file defines constants and enumerations for managing the parameters
 * used in the Laser Processor and Controller. Parameters include gain and
 * oscillator frequencies, which can be automated and adjusted dynamically.
 *
 * @details
 * The parameters defined here are used to control the behavior of the
 * oscillators and gain processing within the VST plugin. Default values are
 * provided for initialization, and each parameter has a unique ID for
 * identification.
 *
 * Features:
 * - Parameter IDs for gain and oscillator frequencies.
 * - Default values for initialization.
 * - Compatible with Steinberg's VST3 parameter handling.
 *
 * Dependencies:
 * - Steinberg VST3 SDK
 *
 * @copyright Radar2000
 * This work is licensed under Creative Commons
 * Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 * @author Radar2000
 */

#ifndef PARAMS_H_
#define PARAMS_H_

#include "pluginterfaces/vst/vsttypes.h"

using namespace Steinberg;
using namespace Vst;

// Default values for parameters
#define default_WaveType WaveType::kSine
#define default_Gain 1.0  ///< Default gain value.
#define default_Osc1 0.8  ///< Default frequency multiplier for Oscillator 1.
#define default_Osc2 0.8  ///< Default frequency multiplier for Oscillator 2.

enum WaveType {
  kSine = 0,
  kSaw,
  kSquare
};

enum WaveParams : ParamID {
  kWaveForm = 100
};

/**
 * @enum FrequencyParams
 * @brief Parameter IDs for oscillator frequency controls.
 */
enum FrequencyParams : ParamID {
  kOsc1 = 200,  ///< Parameter ID for Oscillator 1 frequency.
  kOsc2         ///< Parameter ID for Oscillator 2 frequency.
};

/**
 * @enum GainParams
 * @brief Parameter IDs for gain controls.
 */
enum GainParams : ParamID {
  kParamGainId = 300  ///< Parameter ID for gain control.
};

#endif  // PARAMS_H_

/**
 * @file laser_processor.h
 *
 * @brief Declaration of the Laser Processor for the Laser VST Plugin.
 *
 * This file defines the LaserProcessor class, which implements the audio
 * processing logic for the Laser VST Plugin. It handles polyphonic synthesis,
 * MIDI event processing, and dynamic parameter updates for oscillators and
 * gain.
 *
 * @details
 * The Laser Processor supports up to 8 voices for polyphony. It generates
 * oscillating signals based on frequency parameters and gain values,
 * dynamically allocating voices based on MIDI NoteOn and NoteOff events. It
 * also supports state saving and restoration.
 *
 * Features:
 * - Polyphonic voice management with up to 8 voices.
 * - Real-time parameter updates for gain and oscillator frequencies.
 * - Handles MIDI events (NoteOn/NoteOff) to trigger and release voices.
 * - Supports stereo audio output and automation-ready parameters.
 *
 * Dependencies:
 * - Steinberg VST3 SDK
 * - Standard Math Library (math.h)
 *
 * @copyright Radar2000
 * This work is licensed under Creative Commons
 * Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 * @author Radar2000
 */

#ifndef LASER_PROCESSOR_H_
#define LASER_PROCESSOR_H_

#include "params.h"
#include "voice.h"

#include "public.sdk/source/vst/vstaudioeffect.h"

// The `std` namespace is used for standard library components
using namespace std;

// The Steinberg namespace contains components of the VST3 SDK.
// The Vst namespace is specifically for VST-related classes and enums.
using namespace Steinberg;
using namespace Vst;

// Define a constant for the number of voices used in the plugin.
static const int kNbrVoices = 8;  // A fixed number of voices for the plugin
                                  // (e.g., 8 voices for polyphony).

// Mathematical constants
#define PI2 (3.14159265f * 2.f)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// MIDI constants
constexpr int kMIDINoteA4 = 69;         ///< MIDI note number for A4 (440 Hz).
constexpr float kFrequencyA4 = 440.0f;  ///< Frequency of A4 in Hz.

namespace Radar {

/**
 * @class LaserProcessor
 * @brief The main audio processor for the Laser VST Plugin.
 *
 * This class handles audio processing, event handling, parameter updates,
 * and voice management for polyphonic synthesis.
 */
class LaserProcessor : public AudioEffect {
 public:
  LaserProcessor();                 ///< Constructor.
  ~LaserProcessor() SMTG_OVERRIDE;  ///< Destructor.

  // Factory method for creating instances
  static FUnknown* createInstance(void* /*context*/) {
    return (IAudioProcessor*) new LaserProcessor;
  }

  // VST AudioEffect overrides
  tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
  tresult PLUGIN_API terminate() SMTG_OVERRIDE;
  tresult PLUGIN_API setActive(TBool state) SMTG_OVERRIDE;
  tresult PLUGIN_API setupProcessing(ProcessSetup& newSetup) SMTG_OVERRIDE;
  tresult PLUGIN_API canProcessSampleSize(int32 symbolicSampleSize) SMTG_OVERRIDE;
  tresult PLUGIN_API process(ProcessData& data) SMTG_OVERRIDE;

  // State persistence
  tresult PLUGIN_API setState(IBStream* state) SMTG_OVERRIDE;
  tresult PLUGIN_API getState(IBStream* state) SMTG_OVERRIDE;

 protected:
  // Parameters and voice settings
  ParamValue kWaveFormType = WaveType::kSine;  ///< Gain parameter.

  // Array of `Voice` objects with a size defined by `kNbrVoices` (default 8).
  Voice voices[kNbrVoices];

  ParamValue mGain = default_Gain;  ///< Gain parameter.
  ParamValue mGainReduction = 0.f;  ///< Gain reduction.

  // Oscillator parameters
  float fOsc1 = default_Osc1;  ///< Oscillator 1 frequency multiplier.
  float fOsc2 = default_Osc2;  ///< Oscillator 2 frequency multiplier.
  float fOsc1Phase = 0.f;      ///< Phase for Oscillator 1.
  float fOsc2Phase = 0.f;      ///< Phase for Oscillator 2.
  float fFrequency = 0.f;      ///< Frequency value for the oscillator.
  float fVolume = 0.2f;         ///< Volume level.
  float fDeltaAngle = 0.f;     ///< Phase increment for oscillators.
};

}  // namespace Radar

#endif  // LASER_PROCESSOR_H_

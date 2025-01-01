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
#include "public.sdk/source/vst/vstaudioeffect.h"

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
 * This class handles audio processing, MIDI event handling, parameter updates,
 * and voice management for polyphonic synthesis.
 */
class LaserProcessor : public Steinberg::Vst::AudioEffect {
 public:
  LaserProcessor();                 ///< Constructor.
  ~LaserProcessor() SMTG_OVERRIDE;  ///< Destructor.

  // Factory method for creating instances
  static Steinberg::FUnknown* createInstance(void* /*context*/) {
    return (Steinberg::Vst::IAudioProcessor*)new LaserProcessor;
  }

  // VST AudioEffect overrides
  Steinberg::tresult PLUGIN_API initialize(Steinberg::FUnknown* context)
      SMTG_OVERRIDE;
  Steinberg::tresult PLUGIN_API terminate() SMTG_OVERRIDE;
  Steinberg::tresult PLUGIN_API setActive(Steinberg::TBool state) SMTG_OVERRIDE;
  Steinberg::tresult PLUGIN_API
  setupProcessing(Steinberg::Vst::ProcessSetup& newSetup) SMTG_OVERRIDE;
  Steinberg::tresult PLUGIN_API
  canProcessSampleSize(Steinberg::int32 symbolicSampleSize) SMTG_OVERRIDE;
  Steinberg::tresult PLUGIN_API process(Steinberg::Vst::ProcessData& data)
      SMTG_OVERRIDE;

  // State persistence
  Steinberg::tresult PLUGIN_API setState(Steinberg::IBStream* state)
      SMTG_OVERRIDE;
  Steinberg::tresult PLUGIN_API getState(Steinberg::IBStream* state)
      SMTG_OVERRIDE;

 protected:
  // Parameters and voice settings
  Steinberg::Vst::ParamValue kWaveFormType = WaveType::kSine;  ///< Gain parameter.

  Steinberg::Vst::ParamValue mGain = default_Gain;  ///< Gain parameter.
  Steinberg::Vst::ParamValue mGainReduction = 0.f;  ///< Gain reduction.

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

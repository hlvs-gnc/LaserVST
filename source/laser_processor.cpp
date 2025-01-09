/**
 * @file laser_processor.cpp
 *
 * @brief Implementation of the Laser Processor VST Plugin.
 *
 * This file implements the Laser Processor, a polyphonic VST plugin designed to
 * handle multiple simultaneous notes (polyphony). It generates oscillating
 * signals based on frequency parameters and gain values and processes audio
 * input and MIDI events.
 *
 * @details
 * The Laser Processor supports up to 8 voices, allowing it to play multiple
 * notes simultaneously. It processes input MIDI events, dynamically allocates
 * voices, and synthesizes output audio using sinusoidal oscillators. The code
 * follows the Steinberg VST3 SDK standard for plugin development.
 *
 * Features:
 * - Polyphonic voice handling with up to 8 simultaneous notes.
 * - Real-time parameter updates for gain and oscillator frequencies.
 * - Support for MIDI NoteOn and NoteOff events.
 * - Audio output with stereo channels.
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

#include "laser_processor.h"
#include "laser_cids.h"

#include "voice.h"

#include "base/source/fstreamer.h"

#include "pluginterfaces/vst/ivstparameterchanges.h"

#include "public.sdk/source/vst/hosting/eventlist.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"

using namespace Steinberg;
using namespace std;

static const int kNbrVoices = 8;
Voice voices[kNbrVoices];

namespace Radar {
//-----------------------------------------------------------------------------
// LaserProcessor
//-----------------------------------------------------------------------------
LaserProcessor::LaserProcessor() {
  //--- set the wanted controller for our processor
  setControllerClass(kLaserControllerUID);
}

//-----------------------------------------------------------------------------
LaserProcessor::~LaserProcessor() {}

//-----------------------------------------------------------------------------
tresult PLUGIN_API LaserProcessor::initialize(FUnknown* context) {
  // Here the Plug-in will be instantiated

  //---always initialize the parent-------
  tresult result = AudioEffect::initialize(context);

  // if everything Ok, continue
  if (result != kResultOk) {
    return result;
  }

  //--- create Audio IO ------
  // addAudioInput (STR16 ("Stereo In"), SpeakerArr::kStereo);
  addAudioOutput(STR16("Stereo Out"), SpeakerArr::kStereo);

  /* If you don't need an event bus, you can remove the next line */
  addEventInput(STR16("Event In"), 1);

  return kResultOk;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API LaserProcessor::terminate() {
  // Here the Plug-in will be de-instantiated, last possibility to remove some
  // memory!

  //---do not forget to call parent ------
  return AudioEffect::terminate();
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API LaserProcessor::setActive(TBool state) {
  //--- called when the Plug-in is enable/disable (On/Off) -----
  if (!state) {
    for (int v = 0; v < kNbrVoices; ++v) {
      voices[v] = Voice();  // Reset each voice
    }
  }

  return AudioEffect::setActive(state);
}

static float getWaveSample(float phase, WaveParams type) {
  switch (type) {
    case kSine:
    default:
      // Sine wave
      return sinf(phase);

    case kSaw:
      // Saw wave: goes from -1.0 to +1.0 as phase goes 0..2π
      // (phase / 2π) => range [0..1], scale to [0..2], shift => [-1..+1]
      return 2.0f * (phase / (2.0f * (float)M_PI)) - 1.0f;

    case kSquare:
      // Square wave: +1 if sin(phase) ≥ 0; -1 if sin(phase) < 0
      return (sinf(phase) >= 0.0f) ? 1.0f : -1.0f;
  }
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API LaserProcessor::process(Vst::ProcessData& data) {
  //--- First : Read inputs parameter changes-----------
  if (data.inputParameterChanges) {
    // for each parameter defined by its ID
    int32 numParamsChanged = data.inputParameterChanges->getParameterCount();

    for (int32 index = 0; index < numParamsChanged; index++) {

      if (Vst::IParamValueQueue* paramQueue =
              data.inputParameterChanges->getParameterData(index)) {

        if (paramQueue != NULL) {

          Vst::ParamValue value;
          int32 sampleOffset;
          int32 numPoints = paramQueue->getPointCount();

          if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) ==
              kResultTrue) {

            switch (paramQueue->getParameterId()) {
              case WaveParams::kWaveForm:
                // value is in [0..1] but we have 3 discrete waveforms:
                // convert param float 0..1 => int 0..2
                kWaveFormType = static_cast<int>(value * 2.999999f);
                break;

              case GainParams::kParamGainId:
                mGain = (float) value;
                break;

              case FrequencyParams::kOsc1:
                fOsc1 = (float) value;
                break;

              case FrequencyParams::kOsc2:
                fOsc2 = (float) value;
                break;
            }
          }
        }
      }
    }
  }

  //---Second: Read input events-------------
  // get the list of all event changes
  Vst::IEventList* events = data.inputEvents;

  if (events != NULL) {
    int32 numEvent = events->getEventCount();

    for (int32 i = 0; i < numEvent; i++) {
      Vst::Event event;

      if (events->getEvent(i, event) == kResultOk) {

        switch (event.type) {
          case Vst::Event::kNoteOnEvent: {
            // Find a free voice or steal one
            for (int v = 0; v < kNbrVoices; ++v) {
              if (!voices[v].active) {  // Find an inactive voice
                voices[v].frequency =
                  kFrequencyA4 * powf(2.f, (event.noteOn.pitch - kMIDINoteA4)
                                            / 12.f);

                voices[v].deltaAngle = PI2 * voices[v].frequency
                                      / data.processContext->sampleRate;

                voices[v].phase1 = 0.f;
                voices[v].phase2 = 0.f;
                voices[v].volume = 0.3f;
                voices[v].gainReduction = event.noteOn.velocity;

                // Reset envelope to attack phase
                voices[v].envelopeLevel = 0.f;
                voices[v].envelopePhase = 0.f;  // Attack phase
                voices[v].active = true;
                break;
              }
            }
            break;
          }
          case Vst::Event::kNoteOffEvent: {
            // Turn off voices matching the note
            for (int v = 0; v < kNbrVoices; ++v) {
              if (fabsf(voices[v].frequency -
                        kFrequencyA4 * powf(2.f, (float)(event.noteOff.pitch -
                                                          kMIDINoteA4) /
                                                      12.f)) <= 0.001f) {
                // Trigger release phase
                voices[v].envelopePhase = 1.f;  // Start release
              }
            }
            break;
          }
        }
      }
    }
  }

  //--- Here, you have to implement your processing

  // now we will produce the output
  // mark our outputs has not silent
  data.outputs[0].silenceFlags = 0;

  float gain = mGain - mGainReduction;

  if (gain < 0.f) {  // gain should always positive or zero
    gain = 0.f;
  }

  Vst::Sample32* outL = data.outputs[0].channelBuffers32[0];
  Vst::Sample32* outR = data.outputs[0].channelBuffers32[1];

  for (int32 i = 0; i < data.numSamples; i++) {
    float sampleL = 0.f;
    float sampleR = 0.f;

    // Mix all active voices
    for (int v = 0; v < kNbrVoices; ++v) {
      if (voices[v].active) {
        // Handle Envelope Attack/ Release
        if (voices[v].envelopePhase < 1.f) {  // Attack phase
          voices[v].envelopeLevel +=
              1.f / (Voice::attackTime * data.processContext->sampleRate);
          if (voices[v].envelopeLevel >= 1.f) {
            voices[v].envelopeLevel = 1.f;
          }
        } else {  // Release phase
          voices[v].envelopeLevel *=
            exp(-5.f /
                (Voice::releaseTime * data.processContext->sampleRate));

          // Continue processing the tail instead of cutting off immediately
          if (voices[v].envelopeLevel <= 0.001f) {  // Close to zero threshold
            voices[v].envelopeLevel = 0.f;
            voices[v].active = false;
          }
        }

        // Apply Gain and Envelope to the Oscillators
        float gain = mGain * (1.f - voices[v].gainReduction);  // Scale gain
        float osc1 = fOsc1 * getWaveSample(voices[v].phase1,
                                           (WaveParams)kWaveFormType);
        float osc2 = fOsc2 * getWaveSample(voices[v].phase2,
                                           (WaveParams)kWaveFormType);

        // Combine oscillators and apply envelope and gain
        float voiceSample = (osc1 + osc2) *
                            voices[v].volume * voices[v].envelopeLevel * gain;

        // Smooth exponential scaling
        voiceSample *= powf(voices[v].envelopeLevel, 2.f);

        // Mix into stereo output
        sampleL += voiceSample;  // Left channel
        sampleR += voiceSample;  // Right channel

        voices[v].phase1 += voices[v].deltaAngle;
        voices[v].phase2 += voices[v].deltaAngle * 2.f;

        // Smooth phase wrapping for continuity
        if (voices[v].phase1 > PI2) voices[v].phase1 -= PI2;
        if (voices[v].phase2 > PI2) voices[v].phase2 -= PI2;
      }
    }
    
    // DC offset removal and clipping protection
    sampleL = std::min(1.f, std::max(-1.f, sampleL));
    sampleR = std::min(1.f, std::max(-1.f, sampleR));

    // Write output
    outL[i] = sampleL;
    outR[i] = sampleR;
  }

  return kResultOk;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API
LaserProcessor::setupProcessing(Vst::ProcessSetup& newSetup) {
  //--- called before any processing ----
  return AudioEffect::setupProcessing(newSetup);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API
LaserProcessor::canProcessSampleSize(int32 symbolicSampleSize) {
  // by default kSample32 is supported
  if (symbolicSampleSize == Vst::kSample32) {
    return kResultTrue;
  }

  // disable the following comment if your processing support kSample64
  /* if (symbolicSampleSize == Vst::kSample64)
          return kResultTrue; */

  return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API LaserProcessor::setState(IBStream* state) {
  if (!state) {
    return kResultFalse;
  }

  // called when we load a preset, the model has to be reloaded
  IBStreamer streamer(state, kLittleEndian);
  
  float fval;

  if (streamer.readFloat(fval) == false) {
    return kResultFalse;
  }

  kWaveFormType = static_cast<int>(fval);

  float savedParamGain = 0.f;
  if (streamer.readFloat(savedParamGain) == false) {
    return kResultFalse;
  }
  
  mGain = savedParamGain;

  if (streamer.readFloat(fval) == false) {
    return kResultFalse;
  }

  fOsc1 = fval;

  if (streamer.readFloat(fval) == false) {
    return kResultFalse;
  }

  fOsc2 = fval;

  return kResultOk;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API LaserProcessor::getState(IBStream* state) {
  // here we need to save the model (preset or project)
  float toSaveParamGain = mGain;

  IBStreamer streamer(state, kLittleEndian);
  
  streamer.writeFloat(static_cast<float>(kWaveFormType));

  streamer.writeFloat(toSaveParamGain);

  streamer.writeFloat(fOsc1);
  streamer.writeFloat(fOsc2);

  return kResultOk;
}

//-----------------------------------------------------------------------------
}  // namespace Radar

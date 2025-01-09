/**
 * @file laser_controller.cpp
 *
 * @brief Implementation of the Laser Controller for the Laser VST Plugin.
 *
 * This file implements the controller for the Laser VST Plugin. It is
 * responsible for managing the parameters of the plugin and synchronizing them
 * with the processor. The controller also handles GUI interactions and
 * saves/restores parameter states.
 *
 * @details
 * The Laser Controller initializes and manages parameters such as gain and
 * oscillator frequencies. It interacts with the processor to sync parameter
 * values and allows automation of parameters. It also provides an editor
 * interface for GUI integration.
 *
 * Features:
 * - Parameter management for gain and oscillators.
 * - Synchronization with the processor's state.
 * - Support for GUI integration using VSTGUI.
 * - Saving and restoring parameter states.
 *
 * Dependencies:
 * - Steinberg VST3 SDK
 * - VSTGUI for graphical editor support
 *
 * @copyright Radar2000
 * This work is licensed under Creative Commons
 * Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 * @author Radar2000
 */

#include "laser_controller.h"

#include "base/source/fstreamer.h"
#include "laser_cids.h"
#include "params.h"

#include "pluginterfaces/base/ibstream.h"
#include "vstgui/plugin-bindings/vst3editor.h"

namespace Radar {

//------------------------------------------------------------------------
// LaserController Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API LaserController::initialize(FUnknown* context) {
  // Here the Plug-in will be instantiated

  //---do not forget to call parent ------
  tresult result = EditControllerEx1::initialize(context);
  if (result != kResultOk) {
    return result;
  }

  parameters.addParameter(STR16("WAVE"), STR16("Form"), 3, default_WaveType,
                          Vst::ParameterInfo::kCanAutomate,
                          WaveParams::kWaveForm);

  // Create Parameters
  parameters.addParameter(STR16("GAIN"), STR16("dB"), 0, default_Gain,
                          Vst::ParameterInfo::kCanAutomate,
                          GainParams::kParamGainId);

  setKnobMode(Vst::kLinearMode);
  parameters.addParameter(STR16("OSC1"), STR16("Hz"), 0, default_Osc1,
                          Vst::ParameterInfo::kCanAutomate,
                          FrequencyParams::kOsc1);

  parameters.addParameter(STR16("OSC2"), STR16("Hz"), 0, default_Osc2,
                          Vst::ParameterInfo::kCanAutomate,
                          FrequencyParams::kOsc2);

  return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API LaserController::terminate() {
  // Here the Plug-in will be de-instantiated, last possibility to remove some
  // memory!

  //---do not forget to call parent ------
  return EditControllerEx1::terminate();
}

//------------------------------------------------------------------------
tresult PLUGIN_API LaserController::setComponentState(IBStream* state) {
  // Here you get the state of the component (Processor part)
  if (!state) {
    return kResultFalse;
  }

  IBStreamer streamer(state, kLittleEndian);

  float fval;

  if (streamer.readFloat(fval) == false) {
    return kResultFalse;
  }

  setParamNormalized(kWaveForm, fval);

  float savedParamGain = 0.f;
  if (streamer.readFloat(savedParamGain) == false) {
    return kResultFalse;
  }

  // sync with our parameter
  if (auto param = parameters.getParameter(GainParams::kParamGainId)) {
    param->setNormalized(savedParamGain);
  }

  if (streamer.readFloat(fval) == false) {
    return kResultFalse;
  }

  setParamNormalized(kOsc1, fval);

  if (streamer.readFloat(fval) == false) {
    return kResultFalse;
  }

  setParamNormalized(kOsc2, fval);

  return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API LaserController::setState(IBStream* state) {
  // Here you get the state of the controller

  return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API LaserController::getState(IBStream* state) {
  // Here you are asked to deliver the state of the controller (if needed)
  // Note: the real state of your plug-in is saved in the processor

  return kResultTrue;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API LaserController::createView(FIDString name) {
  // Here the Host wants to open your editor (if you have one)
  if (FIDStringsEqual(name, Vst::ViewType::kEditor)) {
    // create your editor here and return a IPlugView ptr of it
    auto* view = new VSTGUI::VST3Editor(this, "view", "laser_editor.uidesc");
    return view;
  }
  return nullptr;
}

//------------------------------------------------------------------------
}  // namespace Radar

/**
 * @file laser_controller.h
 *
 * @brief Declaration of the Laser Controller for the Laser VST Plugin.
 *
 * This file defines the LaserController class, which handles parameter
 * management and GUI interactions for the Laser VST Plugin. It provides an
 * interface for setting and retrieving parameter values and synchronizing with
 * the processor state.
 *
 * @details
 * The Laser Controller manages plugin parameters such as gain and oscillator
 * frequencies. It also supports GUI integration using VSTGUI and enables
 * automation of parameters.
 *
 * Features:
 * - Parameter management for gain and oscillators.
 * - Synchronization with processor state for consistency.
 * - GUI integration using VSTGUI for user-friendly controls.
 * - Supports saving and restoring parameter states.
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

#ifndef LASER_CONTROLLER_H_
#define LASER_CONTROLLER_H_

#include "public.sdk/source/vst/vsteditcontroller.h"

namespace Radar {

/**
 * @class LaserController
 * @brief The main controller for parameter management and GUI handling.
 *
 * This class manages parameters and GUI interactions for the Laser VST Plugin.
 */
class LaserController : public Steinberg::Vst::EditControllerEx1 {
 public:
  LaserController() = default;                 ///< Constructor.
  ~LaserController() SMTG_OVERRIDE = default;  ///< Destructor.

  // Factory method for creating instances
  static Steinberg::FUnknown* createInstance(void* /*context*/) {
    return (Steinberg::Vst::IEditController*)new LaserController;
  }

  // VST Controller overrides
  Steinberg::tresult PLUGIN_API initialize(Steinberg::FUnknown* context)
      SMTG_OVERRIDE;
  Steinberg::tresult PLUGIN_API terminate() SMTG_OVERRIDE;

  // Parameter state management
  Steinberg::tresult PLUGIN_API setComponentState(Steinberg::IBStream* state)
      SMTG_OVERRIDE;
  Steinberg::tresult PLUGIN_API setState(Steinberg::IBStream* state)
      SMTG_OVERRIDE;
  Steinberg::tresult PLUGIN_API getState(Steinberg::IBStream* state)
      SMTG_OVERRIDE;

  // GUI handling
  Steinberg::IPlugView* PLUGIN_API createView(Steinberg::FIDString name)
      SMTG_OVERRIDE;

  // Interface handling
  DEFINE_INTERFACES
  END_DEFINE_INTERFACES(EditController)
  DELEGATE_REFCOUNT(EditController)

 protected:
  // Protected methods and members can be added here
};

}  // namespace Radar

#endif  // LASER_CONTROLLER_H_

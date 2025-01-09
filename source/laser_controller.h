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
#include "pluginterfaces/vst/ivstmidicontrollers.h"

using namespace Steinberg;
using namespace Vst;

namespace Radar {

/**
 * @class LaserController
 * @brief The main controller for parameter management and GUI handling.
 *
 * This class manages parameters and GUI interactions for the Laser VST Plugin.
 */
class LaserController : public EditControllerEx1 {
 public:
  LaserController() = default;                 ///< Constructor.
  ~LaserController() SMTG_OVERRIDE = default;  ///< Destructor.

  // Factory method for creating instances
  static FUnknown* createInstance(void* /*context*/) {
    return (IEditController*) new LaserController;
  }

  // VST Controller overrides
  tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
  tresult PLUGIN_API terminate() SMTG_OVERRIDE;

  // Parameter state management
  tresult PLUGIN_API setComponentState(IBStream* state) SMTG_OVERRIDE;
  tresult PLUGIN_API setState(IBStream* state) SMTG_OVERRIDE;
  tresult PLUGIN_API getState(IBStream* state) SMTG_OVERRIDE;

  // GUI handling
  IPlugView* PLUGIN_API createView(FIDString name) SMTG_OVERRIDE;

  // Interface handling
  DEFINE_INTERFACES
  END_DEFINE_INTERFACES(EditController)
  DELEGATE_REFCOUNT(EditController)

 protected:
  // Protected methods and members can be added here
};

}  // namespace Radar

#endif  // LASER_CONTROLLER_H_

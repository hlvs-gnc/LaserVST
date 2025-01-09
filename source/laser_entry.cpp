/**
 * @file pluginfactory.cpp
 *
 * @brief Factory definition for the Laser VST Plugin.
 *
 * This file defines the VST3 factory for the Laser VST Plugin, which registers
 * the processor and controller components with the VST host. It specifies the
 * plugin metadata, including its name, category, version, and entry points.
 *
 * @details
 * The factory uses Steinberg's VST3 SDK macros to define the components
 * (processor and controller) and provide them to the host environment. The
 * plugin is identified using unique FUIDs, and metadata such as version and
 * category is embedded for compatibility with DAWs.
 *
 * Features:
 * - Registers the LaserProcessor and LaserController components.
 * - Specifies plugin metadata (name, version, category, SDK compatibility).
 * - Supports distributed processing and controller instantiation.
 *
 * Dependencies:
 * - Steinberg VST3 SDK
 * - Project version file (version.h)
 *
 * @copyright 2024 Radar2000
 * This work is licensed under Creative Commons
 * Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 * @author Radar2000
 */

#include "laser_cids.h"        ///< Unique identifiers (FUIDs) for components.
#include "laser_controller.h"  ///< Controller definition.
#include "laser_processor.h"   ///< Processor definition.
#include "public.sdk/source/main/pluginfactory.h"  ///< VST3 factory definitions.
#include "version.h"  ///< Plugin version information.

#define stringPluginName "Laser"  ///< Name of the plugin.

using namespace Steinberg;
using namespace Vst;
using namespace Radar;

//------------------------------------------------------------------------
//  VST Plug-in Entry Point
//------------------------------------------------------------------------

/**
 * @brief Defines the VST factory and registers plugin components.
 *
 * This factory creates instances of the LaserProcessor and LaserController,
 * registers their metadata, and provides compatibility information for DAWs.
 */
BEGIN_FACTORY_DEF("Radar",                  ///< Company Name
                  "www.radar.com",          ///< Company Website
                  "mailto:info@radar.com")  ///< Support Email

//------------------------------------------------------------------------
// Register the Laser Processor Component
//------------------------------------------------------------------------
DEF_CLASS2(INLINE_UID_FROM_FUID(kLaserProcessorUID),  ///< Processor UID
           PClassInfo::kManyInstances,      ///< Multiple instances allowed
           kVstAudioEffectClass,            ///< Component category
           stringPluginName,                ///< Plugin name
           kDistributable,             ///< Supports distributed systems
           LaserVST3Category,               ///< Subcategory as Instrument
           FULL_VERSION_STR,                ///< Plugin version
           kVstVersionString,               ///< VST SDK version
           LaserProcessor::createInstance)  ///< Instantiation function

//------------------------------------------------------------------------
// Register the Laser Controller Component
//------------------------------------------------------------------------
DEF_CLASS2(INLINE_UID_FROM_FUID(kLaserControllerUID),  ///< Controller UID
           PClassInfo::kManyInstances,       ///< Multiple instances allowed
           kVstComponentControllerClass,     ///< Controller category
           stringPluginName "Controller",    ///< Controller name
           0,                                ///< Unused
           "",                               ///< Unused
           FULL_VERSION_STR,                 ///< Plugin version
           kVstVersionString,                ///< VST SDK version
           LaserController::createInstance)  ///< Instantiation function

//------------------------------------------------------------------------
// End of Factory Definition
//------------------------------------------------------------------------
END_FACTORY

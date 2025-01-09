/**
 * @file laser_cids.h
 *
 * @brief Unique IDs and category definitions for the Laser VST Plugin.
 *
 * This file defines globally unique identifiers (FUIDs) for the Laser VST
 * Plugin's processor and controller components. It also specifies the plugin
 * category for identification by host applications.
 *
 * @details
 * The FUIDs (Globally Unique Identifiers) are used by the Steinberg VST3
 * framework to identify the processor and controller components of the plugin.
 * These identifiers must remain unique to avoid conflicts with other plugins.
 * The plugin is categorized as an instrument, enabling it to be loaded in hosts
 * as a virtual instrument.
 *
 * Features:
 * - Unique identifiers (FUIDs) for the processor and controller components.
 * - Defines the plugin category as "Instrument" for host compatibility.
 * - Ensures compatibility with Steinberg VST3 SDK.
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

#pragma once

#include "pluginterfaces/base/funknown.h"  ///< Provides FUID definitions.
#include "pluginterfaces/vst/vsttypes.h"   ///< Defines VST-specific types.

using namespace Steinberg;

namespace Radar {

/**
 * @brief Unique identifier (FUID) for the Laser Processor component.
 *
 * This ID must be unique to ensure compatibility with VST hosts.
 */
static const FUID kLaserProcessorUID(0x2527AAF6, 0xAD4053EE,
                                                0xA92E6064, 0x887869B6);

/**
 * @brief Unique identifier (FUID) for the Laser Controller component.
 *
 * This ID must be unique to avoid conflicts with other VST controllers.
 */
static const FUID kLaserControllerUID(0x3DE549A1, 0xC39A5F96,
                                                 0xA86E536B, 0x13D5C7F9);

/**
 * @brief Plugin category used by hosts to classify the plugin type.
 *
 * The "Instrument" category specifies that this plugin can be used as
 * a virtual instrument in compatible DAWs.
 */
#define LaserVST3Category "Instrument"

}  // namespace Radar

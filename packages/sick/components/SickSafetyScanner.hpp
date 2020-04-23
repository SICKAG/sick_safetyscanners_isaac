// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-

// -- BEGIN LICENSE BLOCK ----------------------------------------------

/*!
*  Copyright (C) 2020, SICK AG, Waldkirch
*  Copyright (C) 2020, FZI Forschungszentrum Informatik, Karlsruhe, Germany
*
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

// -- END LICENSE BLOCK ------------------------------------------------

//----------------------------------------------------------------------
/*!
 * \file    SickSafetyScanner.hpp
 *
 * \author  Martin Schulze <schulze@fzi.de>
 * \date    2020-04-15
 */
//----------------------------------------------------------------------

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "engine/alice/alice_codelet.hpp"
#include "messages/messages.hpp"
#include "engine/core/byte.hpp"
#include "messages/messages.hpp"

#include "packages/sick/messages/safety_scan.hpp"
#include "packages/sick/messages/commands.hpp"

#include <sick_safetyscanners_base/SickSafetyscanners.h>

namespace isaac
{
namespace sick_safetyscanners
{
struct ConfigurationParams
{
    int channel{0};
    bool channel_enabled{true};

    float angle_offset{-90.0f};
    float angle_start{0.0f};
    float angle_end{0.0f};

    bool general_system_state{true};
    bool derived_settings{true};
    bool measurement_data{true};
    bool intrusion_data{true};
    bool application_io_data{true};

    float publishing_frequency_factor{1.0f};
};

class SickSafetyScanner : public isaac::alice::Codelet
{
public:
    void start() override;
    void tick() override;
    void stop() override;

    // A flatscan proto containing only the measurement data of the sensor.
    // All angle values are given in [radians].
    ISAAC_PROTO_TX(FlatscanProto, flatscan);

    // Safety scan proto containing raw data from the sensor.
    // Contains measurement data, derived values, the general system state, safety-field intrusion data and application related data.
    // All angle values are given in [radians].
    ISAAC_PROTO_TX(SafetyScanProto, safety_scan);

    // Find-Me sensor command relay with blink time [seconds].
    ISAAC_PROTO_RX(FindMeCommandProto, find_me_cmd);

    // OutputPath channel.
    ISAAC_PROTO_TX(OutputPathProto, output_path);

    // Use persistent config from device (reads from sensor).
    ISAAC_PARAM(bool, use_persistent_config, false);

    // The IP4 address of the sensor (for COLA2 commands).
    ISAAC_PARAM(std::string, sensor_ip, "192.168.1.11");
    // The IP4 address of the receiver of the sensor stream data.
    ISAAC_PARAM(std::string, host_ip, "192.168.1.100");
    // UDP port on the receiver of the sensor stream data.
    ISAAC_PARAM(int, host_udp_port, 0);
    // TCP port of the sensor (for COLA2 commands).
    ISAAC_PARAM(int, tcp_port, 2122);
    // The channel number used by the sensor.
    ISAAC_PARAM(int, channel, 0);
    // Determines whether to set the channel active.
    ISAAC_PARAM(bool, channel_enabled, true);

    // If enabled, this codlet publishes simple flatscan protos.
    ISAAC_PARAM(bool, flatscan_pub_active, false);
    // If enabled, this codlet publishes safety message protos.
    ISAAC_PARAM(bool, safety_pub_active, true);
    // If enabled, this codlet publishes outputPath message protos.
    ISAAC_PARAM(bool, outputpath_pub_active, false);

    // Angle offset [deg].
    ISAAC_PARAM(float, angle_offset, -90.0f);

    // Start angle and end angle [deg].
    ISAAC_PARAM(float, angle_start, 0.0f);
    ISAAC_PARAM(float, angle_end, 0.0f);

    // If enabled, the safety scan messages will general system state protos.
    ISAAC_PARAM(bool, general_system_state, true);
    // If enabled, the safety scan messages will contain derived value protos.
    ISAAC_PARAM(bool, derived_settings, true);
    // If enabled, the safety scan messages will contain measurement data protos.
    ISAAC_PARAM(bool, measurement_data, true);
    // If enabled, the safety scan messages will contain intrusion data protos.
    ISAAC_PARAM(bool, intrusion_data, true);
    // If enabled, the safety scan messages will application IO data protos.
    ISAAC_PARAM(bool, application_io_data, true);

    // A multiplicative factor to manipulate the publishing rate of the sensor.
    ISAAC_PARAM(int, publishing_frequency_factor, 1);

    // Sensor data receive timeout [milliseconds]
    ISAAC_PARAM(int, receive_timeout, 5000);

private:
    sick::datastructure::CommSettings m_comm_settings;
    std::unique_ptr<sick::SyncSickSafetyScanner> m_scanner;
    ConfigurationParams m_prev_params;
    float m_range_min{0.1};
    float m_range_max{std::numeric_limits<float>::infinity()};
    uint8_t m_e_interface_type{0};

    // Determines whether the ISAAC_PARAMs have been changed since the last tick.
    bool isParamSetDirty();
    // Update the configuration of the sensor. IP and port updates are ignored by a SickSafetyScanner instance after initialization.
    void updateDeviceConfig();
    // Updates the internal set of previous ISAAC_PARAM values.
    void updatePrevParams();
    // Requests the persistent configuration from the sensor.
    void readConfigFromDevice();
    // Requests the type information from the sensor.
    void readTypeCodeSettings();
    // Assemble and publish a flatscan proto from sensor data.
    void publishFlatScanProto(const sick::datastructure::Data &data);
    // Assemble and publish a safety scan proto from sensor data.
    void publishSafetyScan(const sick::datastructure::Data &data);
    // Assemble and publish an output path proto from sensor data.
    void publishOutputPath(const sick::datastructure::Data &data);
};

} // namespace sick_components
} // namespace isaac

ISAAC_ALICE_REGISTER_CODELET(isaac::sick_safetyscanners::SickSafetyScanner);
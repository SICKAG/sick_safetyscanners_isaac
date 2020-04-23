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
 * \file    SickSafetyScanner.cpp
 *
 * \author  Martin Schulze <schulze@fzi.de>
 * \date    2020-04-15
 */
//----------------------------------------------------------------------

#include "SickSafetyScanner.hpp"
#include "messages/tensor.hpp"
#include <algorithm>
#include <sick_safetyscanners_base/Exceptions.h>
#include <sick_safetyscanners_base/Types.h>

namespace isaac {
namespace sick_safetyscanners {

void SickSafetyScanner::start() {
  LOG_INFO("Starting SickSafetyScanner node");

  sick::types::ip_address_t sensor_ip{
      boost::asio::ip::address_v4::from_string(get_sensor_ip())};

  try {
    m_scanner = std::make_unique<sick::SyncSickSafetyScanner>(
        sensor_ip, get_tcp_port(), m_comm_settings);
  } catch (const sick::timeout_error &e) {
    reportFailure("Could not connect to SICK safety scanner: %s", e.what());
  } catch (const std::exception &e) {
    reportFailure("An unexpected error occured: %s", e.what());
  }

  // Fetch sensor type info from device
  readTypeCodeSettings();

  if (get_use_persistent_config()) {
    readConfigFromDevice();
  }

  tickBlocking();
} // namespace sick_safetyscanners

void SickSafetyScanner::tick() {
  if (isFirstTick() || isParamSetDirty()) {
    updatePrevParams();
    updateDeviceConfig();
  }

  try {
    sick::datastructure::Data data = m_scanner->receive(
        boost::posix_time::milliseconds(get_receive_timeout()));
    if (get_flatscan_pub_active()) {
      publishFlatScanProto(data);
    }
    if (get_safety_pub_active()) {
      publishSafetyScan(data);
    }
    if (get_outputpath_pub_active()) {
      publishOutputPath(data);
    }
  } catch (const sick::timeout_error &e) {
    reportFailure("Timeout while waiting to receive sensor data (UDP)");
  } catch (const sick::runtime_error&e) {
    reportFailure("An error occured %s", e.what());
  }

  if (rx_find_me_cmd().available()) {
    rx_find_me_cmd().processLatestNewMessage(
        [this](FindMeCommandProto::Reader reader, int64_t pubtime,
               int64_t acqtime) {
          uint16_t blink_time = reader.getBlinkTime();
          LOG_INFO("Sending find-me command with blink_time=%d [seconds]",
                   blink_time);
          try {
            m_scanner->findSensor(blink_time);
          } catch (const sick::runtime_error &e) {
            reportFailure("Error while executing find-me command on sensor: %s",
                          e.what());
          }
        });
  }
}

void SickSafetyScanner::stop() { LOG_INFO("Stopping SickSafetyScanner node"); }

void SickSafetyScanner::readTypeCodeSettings() {
  sick::datastructure::TypeCode type_code;
  try {
    m_scanner->requestTypeCode(type_code);
  } catch (const sick::runtime_error &e) {
    reportFailure("Error during requesting sensor type code: %s", e.what());
  }
  m_range_min = 0.1;
  m_range_max = type_code.getMaxRange();
  m_e_interface_type = type_code.getInterfaceType();
}

void SickSafetyScanner::readConfigFromDevice() {
  sick::datastructure::ConfigData config_data;
  try {
    m_scanner->requestPersistentConfig(config_data);
  } catch (const sick::runtime_error &e) {
    reportFailure("Error during requesting sensor persistent config: %s",
                  e.what());
  }

  set_channel(config_data.getChannel());
  set_channel_enabled(config_data.getEnabled());

  set_angle_start(config_data.getStartAngle());
  set_angle_end(config_data.getEndAngle());
  set_angle_offset(0.0f);

  auto features = config_data.getFeatures();
  using sick::SensorDataFeatures::isFlagSet;

  set_application_io_data(
      isFlagSet(features, sick::SensorDataFeatures::APPLICATION_DATA));
  set_measurement_data(
      isFlagSet(features, sick::SensorDataFeatures::MEASUREMENT_DATA));
  set_derived_settings(
      isFlagSet(features, sick::SensorDataFeatures::DERIVED_SETTINGS));
  set_intrusion_data(
      isFlagSet(features, sick::SensorDataFeatures::INTRUSION_DATA));
  set_general_system_state(
      isFlagSet(features, sick::SensorDataFeatures::GENERAL_SYSTEM_STATE));

  set_publishing_frequency_factor(config_data.getPublishingFrequency());
}

bool SickSafetyScanner::isParamSetDirty() {
  if (get_channel() != m_prev_params.channel ||
      get_channel_enabled() != m_prev_params.channel_enabled ||
      get_angle_offset() != m_prev_params.angle_offset ||
      get_angle_start() != m_prev_params.angle_start ||
      get_angle_end() != m_prev_params.angle_end ||
      get_general_system_state() != m_prev_params.general_system_state ||
      get_derived_settings() != m_prev_params.derived_settings ||
      get_measurement_data() != m_prev_params.measurement_data ||
      get_intrusion_data() != m_prev_params.intrusion_data ||
      get_application_io_data() != m_prev_params.application_io_data ||
      get_publishing_frequency_factor() !=
          m_prev_params.publishing_frequency_factor) {
    return true;
  }
  return false;
}

void SickSafetyScanner::updatePrevParams() {
  m_prev_params.channel = get_channel();
  m_prev_params.channel_enabled = get_channel_enabled();
  m_prev_params.angle_start = get_angle_start();
  m_prev_params.angle_offset = get_angle_offset();
  m_prev_params.angle_end = get_angle_end();
  m_prev_params.application_io_data = get_application_io_data();
  m_prev_params.derived_settings = get_derived_settings();
  m_prev_params.general_system_state = get_general_system_state();
  m_prev_params.intrusion_data = get_intrusion_data();
  m_prev_params.measurement_data = get_measurement_data();
  m_prev_params.publishing_frequency_factor = get_publishing_frequency_factor();
}

void SickSafetyScanner::updateDeviceConfig() {
  LOG_INFO("Updating device config. Host_ip and host_udp_port are only "
           "considered on first initialization.");
  sick::types::SensorFeatures features{sick::SensorDataFeatures::NONE};
  features = sick::SensorDataFeatures::toFeatureFlags(
      get_general_system_state(), get_derived_settings(),
      get_measurement_data(), get_intrusion_data(), get_application_io_data());

  sick::datastructure::CommSettings settings;
  settings.host_ip =
      sick::types::ip_address_t::address_v4::from_string(get_host_ip());
  settings.host_udp_port = get_host_udp_port();
  settings.features = features;
  settings.channel = get_channel();

  if (std::fabs(get_angle_start() - get_angle_end()) <=
      std::numeric_limits<float>::epsilon()) {
    settings.start_angle = RadToDeg(0.0f);
    settings.end_angle = RadToDeg(0.0f);
  } else {
    settings.start_angle = RadToDeg(get_angle_start()) - get_angle_offset();
    settings.end_angle = RadToDeg(get_angle_end()) - get_angle_offset();
  }

  settings.publishing_frequency = get_publishing_frequency_factor();
  settings.enabled = get_channel_enabled();
  settings.e_interface_type = m_e_interface_type;

  try {
    m_scanner->changeSensorSettings(settings);
  } catch (const sick::runtime_error &e) {
    reportFailure("Error during updating sensor settings: %s", e.what());
  }
}

void SickSafetyScanner::publishSafetyScan(
    const sick::datastructure::Data &data) {
  auto safety_scan_proto = tx_safety_scan().initProto();
  ToProto(data, safety_scan_proto, get_angle_offset());
  tx_safety_scan().publish();
}

void SickSafetyScanner::publishFlatScanProto(
    const sick::datastructure::Data &data) {
  if (data.getDerivedValuesPtr()->isEmpty() ||
      data.getMeasurementDataPtr()->isEmpty()) {
    LOG_WARNING("Publishing FlatScanProto is not possible when derived values "
                "or measurement data is disabled in the sensor.");
    return;
  }
  const std::size_t n_scan_points =
      data.getMeasurementDataPtr()->getNumberOfBeams();
  const std::vector<sick::datastructure::ScanPoint> scan_points =
      data.getMeasurementDataPtr()->getScanPointsVector();

  auto flat_scan_proto = tx_flatscan().initProto();
  flat_scan_proto.setInvalidRangeThreshold(m_range_min);
  flat_scan_proto.setOutOfRangeThreshold(m_range_max);
  flat_scan_proto.initRanges(n_scan_points);
  flat_scan_proto.initAngles(n_scan_points);
  // flat_scan_proto.initVisibilities(n_scan_points);

  for (std::size_t i = 0; i < n_scan_points; i++) {
    const auto scan_point = scan_points[i];

    // Ranges [meter]
    float range = static_cast<float>(
                      scan_point.getDistance() *
                      data.getDerivedValuesPtr()->getMultiplicationFactor()) *
                  1e-3; //  mm -> m
    flat_scan_proto.getRanges().set(i, range);

    // Angles [radians]
    float angle_rad = DegToRad(scan_point.getAngle() + get_angle_offset());
    flat_scan_proto.getAngles().set(i, angle_rad);

    // Visibility / Reflectivity
    // flat_scan_proto.getVisibilities().set(
    //     i, static_cast<float>(scan_point.getReflectivity()));
  }
  tx_flatscan().publish();
}

void SickSafetyScanner::publishOutputPath(
    const sick::datastructure::Data &data) {
  auto app_data = data.getApplicationDataPtr();
  auto outputs = app_data->getOutputs();

  auto eval_out = outputs.getEvalOutVector();
  auto eval_out_is_safe = outputs.getEvalOutIsSafeVector();
  auto eval_out_valid = outputs.getEvalOutIsValidVector();
  auto monitoring_case_numbers = outputs.getMonitoringCaseVector();
  auto monitoring_case_number_flags = outputs.getMonitoringCaseFlagsVector();

  auto outputpath_proto = tx_output_path().initProto();

  if (monitoring_case_number_flags.at(0)) {
    outputpath_proto.setActiveMonitoringCase(monitoring_case_numbers.at(0));
  }

  std::size_t n_eval_count = eval_out.size();
  outputpath_proto.initIsSafe(n_eval_count);
  outputpath_proto.initIsValid(n_eval_count);
  outputpath_proto.initStatus(n_eval_count);

  for (size_t i = 0; i < eval_out.size(); i++) {
    outputpath_proto.getStatus().set(i, eval_out[i]);
    outputpath_proto.getIsSafe().set(i, eval_out_is_safe[i]);
    outputpath_proto.getIsValid().set(i, eval_out_valid[i]);
  }

  tx_output_path().publish();
}

} // namespace sick_safetyscanners
} // namespace isaac
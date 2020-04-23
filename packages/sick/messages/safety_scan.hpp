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
 * \file    safety_scan.cpp
 *
 * \author  Martin Schulze <schulze@fzi.de>
 * \date    2020-04-15
 */
//----------------------------------------------------------------------

#pragma once

#include "packages/sick/messages/safety_scan.capnp.h"
#include "messages/proto_registry.hpp"
#include "messages/math.hpp"
#include <sick_safetyscanners_base/datastructure/ScanPoint.h>
#include <sick_safetyscanners_base/datastructure/Data.h>
#include <sick_safetyscanners_base/datastructure/FieldData.h>
#include <sick_safetyscanners_base/datastructure/DerivedValues.h>
#include <sick_safetyscanners_base/datastructure/IntrusionData.h>
#include <sick_safetyscanners_base/datastructure/IntrusionDatum.h>
#include <sick_safetyscanners_base/datastructure/GeneralSystemState.h>
#include <sick_safetyscanners_base/datastructure/MeasurementData.h>
#include <sick_safetyscanners_base/datastructure/ApplicationData.h>
#include <sick_safetyscanners_base/datastructure/ApplicationInputs.h>
#include <sick_safetyscanners_base/datastructure/ApplicationOutputs.h>

namespace isaac
{
namespace sick_safetyscanners
{

inline void ToProto(const sick::datastructure::ScanPoint &scan_point, ::ScanPointProto::Builder builder, float angle_offset)
{
    builder.initStatus();
    auto status = builder.getStatus();

    builder.setAngle(DegToRad(scan_point.getAngle() + angle_offset));
    builder.setDistance(scan_point.getDistance());

    status.setContamination(scan_point.getContaminationBit());
    status.setContaminationWarning(scan_point.getContaminationWarningBit());
    status.setGlare(scan_point.getGlareBit());
    status.setInfinite(scan_point.getInfiniteBit());
    status.setReflectivity(scan_point.getReflectivity());
    status.setReflector(scan_point.getReflectorBit());
    status.setValid(scan_point.getValidBit());
}

inline void ToProto(const sick::datastructure::DataHeader &data_header, ::DataHeaderProto::Builder builder)
{
    if (!data_header.isEmpty())
    {
        builder.initVersion();
        auto version = builder.getVersion();
        version.setVersion(data_header.getVersionIndicator());
        version.setMajor(data_header.getVersionMajorVersion());
        version.setMinor(data_header.getVersionMinorVersion());
        version.setRelease(data_header.getVersionRelease());

        builder.setScanNumber(data_header.getScanNumber());
        builder.setSequenceNumber(data_header.getSequenceNumber());
        builder.setSerialNumberOfDevice(data_header.getSerialNumberOfDevice());
        builder.setSerialNumberOfChannelPlug(data_header.getSerialNumberOfSystemPlug());

        builder.setChannelNumber(data_header.getChannelNumber());

        builder.initTimestamp();
        auto timestamp = builder.getTimestamp();
        timestamp.setDate(data_header.getTimestampDate());
        timestamp.setTime(data_header.getTimestampTime());
    }
}

inline void ToProto(const sick::datastructure::ApplicationInputs &application_inputs, ::ApplicationInputsProto::Builder builder)
{
    // UnsafeInputs
    builder.initUnsafeInputs();
    auto unsafe_inputs = builder.getUnsafeInputs();
    const std::size_t n_unsafe_inputs = application_inputs.getUnsafeInputsFlagsVector().size();

    unsafe_inputs.initInputSources(n_unsafe_inputs);
    unsafe_inputs.initFlags(n_unsafe_inputs);
    for (std::size_t i = 0; i < n_unsafe_inputs; i++)
    {
        unsafe_inputs.getInputSources().set(i, application_inputs.getUnsafeInputsInputSourcesVector()[i]);
        unsafe_inputs.getFlags().set(i, application_inputs.getUnsafeInputsFlagsVector()[i]);
    }

    // MonitoringCaseNumberInputs
    builder.initMonitoringCaseNumberInputs();
    auto monitoring_cases = builder.getMonitoringCaseNumberInputs();
    const std::size_t n_monitoring_cases = application_inputs.getMonitoringCasevector().size();

    monitoring_cases.initMonitoringCaseNumber(n_monitoring_cases);
    monitoring_cases.initMonitoringCaseNumberFlags(n_monitoring_cases);
    for (std::size_t i = 0; i < n_monitoring_cases; i++)
    {
        monitoring_cases.getMonitoringCaseNumber().set(i, application_inputs.getMonitoringCasevector()[i]);
        monitoring_cases.getMonitoringCaseNumberFlags().set(i, application_inputs.getMonitoringCaseFlagsVector()[i]);
    }

    // LinearVelocityInputs
    builder.initLinearVelocityInputs();
    auto velocity_inputs = builder.getLinearVelocityInputs();
    velocity_inputs.setVelocity0(application_inputs.getVelocity0());
    velocity_inputs.setVelocity0TransmittedSafely(application_inputs.getVelocity0TransmittedSafely());
    velocity_inputs.setVelocity0Valid(application_inputs.getVelocity0Valid());
    velocity_inputs.setVelocity1(application_inputs.getVelocity1());
    velocity_inputs.setVelocity1TransmittedSafely(application_inputs.getVelocity1TransmittedSafely());
    velocity_inputs.setVelocity1Valid(application_inputs.getVelocity1Valid());

    builder.setSleepModeInput(application_inputs.getSleepModeInput());
}

inline void ToProto(const sick::datastructure::ApplicationOutputs &application_outputs, ::ApplicationOutputsProto::Builder builder)
{
    // EvaluationPathsOutputs
    builder.initEvaluationPathsOutputs();
    auto evaluation_paths = builder.getEvaluationPathsOutputs();
    const std::size_t n_eval_out = application_outputs.getEvalOutVector().size();
    evaluation_paths.initEvalOut(n_eval_out);
    evaluation_paths.initIsSafe(n_eval_out);
    evaluation_paths.initIsValid(n_eval_out);
    for (std::size_t i = 0; i < n_eval_out; i++)
    {
        evaluation_paths.getEvalOut().set(i, application_outputs.getEvalOutVector()[i]);
        evaluation_paths.getIsSafe().set(i, application_outputs.getEvalOutIsSafeVector()[i]);
        evaluation_paths.getIsValid().set(i, application_outputs.getEvalOutIsValidVector()[i]);
    }

    // MonitoringCaseNumberOutputs
    builder.initMonitoringCaseNumberOutputs();
    auto monitoring_cases = builder.getMonitoringCaseNumberOutputs();
    const std::size_t n_monitoring_cases = application_outputs.getMonitoringCaseVector().size();
    monitoring_cases.initMonitoringCaseNumber(n_monitoring_cases);
    monitoring_cases.initFlags(n_monitoring_cases);
    for (std::size_t i = 0; i < n_monitoring_cases; i++)
    {
        monitoring_cases.getMonitoringCaseNumber().set(i, application_outputs.getMonitoringCaseVector()[i]);
        monitoring_cases.getFlags().set(i, application_outputs.getMonitoringCaseFlagsVector()[i]);
    }

    builder.setSleepModeOutput(application_outputs.getSleepModeOutput());
    builder.setSleepModeOutputValid(application_outputs.getFlagsSleepModeOutputIsValid());

    // ErrorFlags
    builder.initErrorFlags();
    auto error_flags = builder.getErrorFlags();

    error_flags.setContaminationWarning(application_outputs.getHostErrorFlagContaminationWarning());
    error_flags.setContaminationError(application_outputs.getHostErrorFlagContaminationError());
    error_flags.setManipulationError(application_outputs.getHostErrorFlagManipulationError());
    error_flags.setGlare(application_outputs.getHostErrorFlagGlare());
    error_flags.setReferenceContourIntruded(application_outputs.getHostErrorFlagReferenceContourIntruded());
    error_flags.setCriticalError(application_outputs.getHostErrorFlagCriticalError());
    error_flags.setErrorFlagsAreValid(application_outputs.getFlagsHostErrorFlagsAreValid());

    // LinearVelocityInputs
    builder.initLinearVelocityInputs();
    auto linear_velocities = builder.getLinearVelocityInputs();
    linear_velocities.setVelocity0(application_outputs.getVelocity0());
    linear_velocities.setVelocity0TransmittedSafely(application_outputs.getVelocity0TransmittedSafely());
    linear_velocities.setVelocity0Valid(application_outputs.getVelocity0Valid());
    linear_velocities.setVelocity1(application_outputs.getVelocity1());
    linear_velocities.setVelocity1TransmittedSafely(application_outputs.getVelocity1TransmittedSafely());
    linear_velocities.setVelocity1Valid(application_outputs.getVelocity1Valid());

    // ResultingVelocity
    builder.initResultingVelocity();
    auto resulting_velocity = builder.getResultingVelocity();
    const std::size_t n_resulting_velocities = application_outputs.getResultingVelocityVector().size();
    resulting_velocity.initResultingVelocity(n_resulting_velocities);
    resulting_velocity.initFlags(n_resulting_velocities);
    for (std::size_t i = 0; i < n_resulting_velocities; i++)
    {
        resulting_velocity.getResultingVelocity().set(i, application_outputs.getResultingVelocityVector()[i]);
        resulting_velocity.getFlags().set(i, application_outputs.getResultingVelocityIsValidVector()[i]);
    }
}

inline void ToProto(const sick::datastructure::ApplicationData &application_data,
                    ::ApplicationDataProto::Builder builder)
{
    if (!application_data.isEmpty())
    {
        ToProto(application_data.getInputs(), builder.initInputs());
        ToProto(application_data.getOutputs(), builder.initOutputs());
    }
}

inline void ToProto(const sick::datastructure::FieldData &field_data, ::FieldDataProto::Builder builder)
{
    builder.setAngularResolution(field_data.getAngularBeamResolution());
    builder.setProtectiveField(field_data.getIsProtectiveField());
    const std::size_t n_ranges = field_data.getBeamDistances().size();
    builder.initRanges(n_ranges);
    for (std::size_t i = 0; i < n_ranges; i++)
    {
        builder.getRanges().set(i, static_cast<float>(field_data.getBeamDistances()[i]) * 1e-3);
    }
}

inline void ToProto(const sick::datastructure::DerivedValues &derived_values, ::DerivedValuesProto::Builder builder, const float angle_offset)
{
    if (!derived_values.isEmpty())
    {
        builder.setMultiplicationFactor(derived_values.getMultiplicationFactor());
        builder.setScanTime(derived_values.getScanTime());
        builder.setInterbeamPeriod(derived_values.getInterbeamPeriod());
        builder.setNumberOfBeams(derived_values.getNumberOfBeams());
        builder.setStartAngle(DegToRad(derived_values.getStartAngle() + angle_offset));
        builder.setAngularBeamResolution(
            DegToRad(derived_values.getAngularBeamResolution()));
    }
}

inline void ToProto(const sick::datastructure::GeneralSystemState &system_state, ::GeneralSystemStateProto::Builder builder)
{
    if (!system_state.isEmpty())
    {
        builder.setRunModeActive(system_state.getRunModeActive());
        builder.setStandbyModeActive(system_state.getStandbyModeActive());
        builder.setContaminationWarning(system_state.getContaminationWarning());
        builder.setContaminationError(system_state.getContaminationError());
        builder.setReferenceContourStatus(system_state.getReferenceContourStatus());
        builder.setManipulationStatus(system_state.getManipulationStatus());

        const std::size_t n_safe_cut_off_paths = system_state.getSafeCutOffPathVector().size();
        builder.initSafeCutOffPath(n_safe_cut_off_paths);
        for (std::size_t i = 0; i < n_safe_cut_off_paths; i++)
        {
            builder.getSafeCutOffPath().set(i, system_state.getSafeCutOffPathVector()[i]);
        }

        const std::size_t n_non_safe_cut_off_paths = system_state.getNonSafeCutOffPathVector().size();
        builder.initNonSafeCutOffPath(n_non_safe_cut_off_paths);
        for (std::size_t i = 0; i < n_non_safe_cut_off_paths; i++)
        {
            builder.getNonSafeCutOffPath().set(i, system_state.getNonSafeCutOffPathVector()[i]);
        }

        const std::size_t n_reset_required_cut_off_paths = system_state.getResetRequiredCutOffPathVector().size();
        builder.initResetRequiredCutOffPath(n_reset_required_cut_off_paths);
        for (std::size_t i = 0; i < n_reset_required_cut_off_paths; i++)
        {
            builder.getResetRequiredCutOffPath().set(i, system_state.getResetRequiredCutOffPathVector()[i]);
        }

        builder.setCurrentMonitoringcaseNoTable1(system_state.getCurrentMonitoringCaseNoTable1());
        builder.setCurrentMonitoringcaseNoTable2(system_state.getCurrentMonitoringCaseNoTable2());
        builder.setCurrentMonitoringcaseNoTable3(system_state.getCurrentMonitoringCaseNoTable3());
        builder.setCurrentMonitoringcaseNoTable4(system_state.getCurrentMonitoringCaseNoTable4());
        builder.setApplicationError(system_state.getApplicationError());
        builder.setDeviceError(system_state.getDeviceError());
    }
}

inline void ToProto(const sick::datastructure::MeasurementData &measurements, ::MeasurementDataProto::Builder builder, float angle_offset)
{
    if (!measurements.isEmpty())
    {
        std::size_t n_scan_points = measurements.getNumberOfBeams();
        builder.setNumberOfBeams(n_scan_points);
        builder.initScanPoints(n_scan_points);
        for (std::size_t i = 0; i < n_scan_points; i++)
        {
            ToProto(measurements.getScanPointsVector()[i], builder.getScanPoints()[i], angle_offset);
        }
    }
}

inline void ToProto(const sick::datastructure::IntrusionDatum &intrusion, ::IntrusionDatumProto::Builder builder)
{
    const std::size_t n_flags = intrusion.getFlagsVector().size();
    builder.setSize(n_flags);
    builder.initFlags(n_flags);
    for (std::size_t i = 0; i < n_flags; i++)
    {
        builder.getFlags().set(i, intrusion.getFlagsVector()[i]);
    }
}

inline void ToProto(const sick::datastructure::IntrusionData &intrusion, ::IntrusionDataProto::Builder builder)
{
    if (!intrusion.isEmpty())
    {
        const std::size_t n_intrusions = intrusion.getIntrusionDataVector().size();
        builder.initData(n_intrusions);
        for (std::size_t i = 0; i < n_intrusions; i++)
        {
            ToProto(intrusion.getIntrusionDataVector()[i], builder.getData()[i]);
        }
    }
}

inline void ToProto(const sick::datastructure::Data &data, ::SafetyScanProto::Builder builder, float angle_offset)
{
    ToProto(*data.getDataHeaderPtr(), builder.initHeader());
    ToProto(*data.getDerivedValuesPtr(), builder.initDerivedValues(), angle_offset);
    ToProto(*data.getGeneralSystemStatePtr(), builder.initGeneralSystemState());
    ToProto(*data.getMeasurementDataPtr(), builder.initMeasurementData(), angle_offset);
    ToProto(*data.getIntrusionDataPtr(), builder.initIntrusionData());
    ToProto(*data.getApplicationDataPtr(), builder.initApplicationData());
}

} // namespace sick_components
} // namespace isaac

ISAAC_ALICE_REGISTER_PROTO(SafetyScanProto);
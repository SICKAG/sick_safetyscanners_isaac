#####################################################################################
#  Copyright (C) 2020, SICK AG, Waldkirch
#  Copyright (C) 2020, FZI Forschungszentrum Informatik, Karlsruhe, Germany
#
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# \file   safety_scan.capnp
# \author Martin Schulze <schulze@fzi.de>
# \date   2020-04-15
#
#####################################################################################
@0xa0262af24f46b5d9;

struct FieldDataProto {
  ranges @0: List(Float32);
  startAngle @1: Float32;
  angularResolution @2: Float32;
  protectiveField @3: Bool;
}

struct ApplicationInputsProto {
  struct UnsafeInputs {
    # The individual bit flags represent the current state of the static input sources used for monitoring case switching (if monitoring case switching is done via static inputs). Note that with antivalent evaluation, each static input has two input sources.
    inputSources @0: List(Bool);

    # There is one flag (bit) for each static input source. IF the flag is set, the corresponding input source is available to be used by the application.
    flags @1: List(Bool);
  }

  struct MonitoringCaseNumberInputs {
    # If monitoring case switching is done by a case number (e.g. via Assembly 103), this information contains the input signal used by the application. As of today microScan3 product variants support no more than one monitoring case table. Therefore, only the first array element is currently valid.
    monitoringCaseNumber @0: List(UInt16);

    # There is one flag (bit) for each case number. If this flag is set, the corresponding monitoring case number is available to be used by the application.
    monitoringCaseNumberFlags @1: List(Bool);
  }

  struct LinearVelocityInputs {
    # First of two unsafe measured/transmitted linear velocities.
    velocity0 @0: Int16;
    velocity0Valid @1: Bool;
    velocity0TransmittedSafely @2: Bool;

    # Second of two unsafe measured/transmitted linear velocities.
    velocity1 @3: Int16;
    velocity1Valid @4: Bool;
    velocity1TransmittedSafely @5: Bool;
  }

  unsafeInputs @0: UnsafeInputs;
  monitoringCaseNumberInputs @1: MonitoringCaseNumberInputs;
  linearVelocityInputs @2: LinearVelocityInputs;
  sleepModeInput @3: UInt8;
}

struct ApplicationOutputsProto {
  struct EvaluationPathOutputs {
    # Bits 0-19 indicate if the non-safe cut-off path is HIGH or LOW. Bits 20-31 are to be ignored.
    evalOut @0: List(Bool);

    # Indicates if the corresponding cut-off-path is safe.
    isSafe @1: List(Bool);

    # Indicate if the corresponding EvalOut bit is valid.
    isValid @2: List(Bool);
  }

  struct MonitoringCaseNumberOuputs {
    # MonitoringCaseNumber[i]: Currently active monitoring case number. as of today microScan3 product variants support no more than one monitoring case table. Therefore, only the first array element is currently valid.
    monitoringCaseNumber @0: List(UInt16);

    # There is one flag (bit) for each case number. If the flag is HIGH, the corresponding monitoring case number is valid.
    flags @1: List(Bool);
  }

  struct ErrorFlags {
    contaminationWarning @0: Bool;
    contaminationError @1: Bool;
    manipulationError @2: Bool;
    glare @3: Bool;
    referenceContourIntruded @4: Bool;
    criticalError @5: Bool;
    errorFlagsAreValid @6: Bool;
  }

  struct LinearVelocityInputs {
    # First of two unsafe measured/transmitted linear velocities.
    velocity0 @0: Int16;
    velocity0Valid @1: Bool;
    velocity0TransmittedSafely @2: Bool;

    # Second of two unsafe measured/transmitted linear velocities.
    velocity1 @3: Int16;
    velocity1Valid @4: Bool;
    velocity1TransmittedSafely @5: Bool;
  }

  struct ResultingVelocity {
    # ResultingVelocity[i]: Resulting velocity for each monitoring case table. As of today microScan3 product variants support no more than one monitoring case table. Therefore, only the first array element is currently valid.
    resultingVelocity @0: List(Int16);

    # The flags (bits) indicate if the resulting velocity at the same index is valid.
    flags @1: List(Bool);
  }

  evaluationPathsOutputs @0: EvaluationPathOutputs;
  monitoringCaseNumberOutputs @1: MonitoringCaseNumberOuputs;
  sleepModeOutput @2: UInt8;
  errorFlags @3: ErrorFlags;
  linearVelocityInputs @4: LinearVelocityInputs;
  resultingVelocity @5: ResultingVelocity;
  sleepModeOutputValid @6: Bool;
}

struct ApplicationDataProto {
  inputs @0: ApplicationInputsProto;
  outputs @1: ApplicationOutputsProto;
}

struct DataHeaderProto {
  struct Version {
    # Version indicator, capital letter (e.g. 'V' or 'R' for releases). A '\O' character in this field indicates that the remainder of this datastructure is invalid.
    version @0: UInt8;
    # Major version number
    major @1: UInt8;
    # Minor version number
    minor @2: UInt8;
    # Release number
    release @3: UInt8;
  }
  
  struct Timestamp {
    # Days since 1.1.1972 (if a real time clock is present). Otherwise increased with every 24h mark in the system operating time.
    date @0: UInt16;
    # Milliseconds since midnight (if a real time clock is present) or since the last 24h mark in the system operating time.
    time @1: UInt32;
  }

  version @0: Version;
  serialNumberOfDevice @1: UInt32;
  serialNumberOfChannelPlug @2: UInt32;

  # Number of channel this measurement data instance belongs to.
  channelNumber @3: UInt8;

  # Consecutive increasing number within this channel.
  sequenceNumber @4: UInt32;

  # The scan number this measurement data instance belongs to.
  scanNumber @5: UInt32;

  # A timestamp when the measurement data output instance was created.
  timestamp @6: Timestamp;
}

struct DerivedValuesProto {
  # Multiplication factor to be applied to the beam distance values to get the distance in millimeter.
  multiplicationFactor @0: UInt16;

  # The number of beams depends on the configured start angle, stop angle as well as the scan time.
  numberOfBeams @1: UInt16;
  
  # Scan time [ms]
  scanTime @2: UInt16;

  # Devide this value by 4194304.0 to get the (derived) start angle.
  startAngle @3: Float32;

  # Devide this value by 4194304.0 to get the angular beam resolution.
  angularBeamResolution @4: Float32;

  # Approximate time between two consecutive beams [usecs].
  interbeamPeriod @5: UInt32;
}

struct GeneralSystemStateProto {
  runModeActive @0: Bool;
  standbyModeActive @1: Bool;
  contaminationWarning @2: Bool;
  contaminationError @3: Bool;
  referenceContourStatus @4: Bool;
  manipulationStatus @5: Bool;

  safeCutOffPath @6: List(Bool);
  nonSafeCutOffPath @7: List(Bool);
  resetRequiredCutOffPath @8: List(Bool);

  currentMonitoringcaseNoTable1 @9: UInt8;
  currentMonitoringcaseNoTable2 @10: UInt8;
  currentMonitoringcaseNoTable3 @11: UInt8;
  currentMonitoringcaseNoTable4 @12: UInt8;

  applicationError @13: Bool;
  deviceError @14: Bool;
}

struct IntrusionDatumProto {
  # Number of intrusion beam flag bytes
  size @0: UInt32;

  # 0: Beam not violated
  # 1: Beam violated
  flags @1: List(Bool);
}

struct IntrusionDataProto {
  data @0: List(IntrusionDatumProto);
}

struct ScanPointProto  {
  # Reflectivity value and flags of a single beam
  struct Status {
    reflectivity @0: UInt8;
    valid @1: Bool;
    infinite @2: Bool;
    glare @3: Bool;
    reflector @4: Bool;
    contamination @5: Bool;
    contaminationWarning @6: Bool;
  }

  # Angle [rad]
  angle @0: Float32;

  # Distance [mm]
  distance @1: UInt16;

  # Status flags for this scan point (beam) 
  status @2: Status;
}

struct MeasurementDataProto {
  numberOfBeams @0: UInt32;
  scanPoints @1: List(ScanPointProto);
}

struct MonitoringCaseProto {
  monitoringCaseNumber @0: Int32;
  fields @1: List(Int32);
  fieldsValid @2: List(Bool);
}

struct OutputPathsProto {
  status @0: List(Bool);
  isSafe @1: List(Bool);
  isValid @2: List(Bool);
  activeMonitoringCase @3: Int32;
}

struct SafetyScanProto {
  header @0: DataHeaderProto;
  derivedValues @1: DerivedValuesProto;
  generalSystemState @2: GeneralSystemStateProto;
  measurementData @3: MeasurementDataProto;
  intrusionData @4: IntrusionDataProto;
  applicationData @5: ApplicationDataProto;
}

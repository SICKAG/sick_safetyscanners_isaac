#####################################################################################
# Copyright (C) 2020, SICK AG, Waldkirch
# Copyright (C) 2020, FZI Forschungszentrum Informatik, Karlsruhe, Germany
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
# \file   commands.capnp
# \author Martin Schulze <schulze@fzi.de>
# \date   2020-04-15
#
#####################################################################################
@0xab08bfdc1034ee4a;

struct FindMeCommandProto {
    # The time duration to let the sensor's display blink in various colors [seconds].
    blinkTime @0: UInt16; 
}

struct OutputPathProto {
    status @0: List(Bool);
    isSafe @1: List(Bool);
    isValid @2: List(Bool);
    activeMonitoringCase @3: Int32;
}

# struct MonitoringCaseProto {
#     monitoringCaseNumber @0: Int32;
#     fields @1: List(Int32);
#     fieldsValid @2: List(Bool);
# }



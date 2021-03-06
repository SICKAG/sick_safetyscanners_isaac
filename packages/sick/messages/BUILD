"""
Copyright (C) 2020, SICK AG, Waldkirch
Copyright (C) 2020, FZI Forschungszentrum Informatik, Karlsruhe, Germany

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

file   BUILD
author Martin Schulze <schulze@fzi.de>
date   2020-04-15
"""

load("@com_nvidia_isaac//engine/build:isaac.bzl", "isaac_cc_library")
load("//packages/sick/messages:messages.bzl", "create_message_proto_libraries", "message_proto_library_names")

filegroup(
    name = "capnp",
    srcs = glob(["*.capnp"]),
    visibility = ["//visibility:public"],
)

create_message_proto_libraries()

isaac_cc_library(
    name = "safety_scan",
    hdrs = ["safety_scan.hpp"],
    visibility = ["//visibility:public"],
    deps = [
        "@com_nvidia_isaac//messages:proto_registry",
        "safety_scan_proto",
    ]
)

isaac_cc_library(
    name = "commands",
    hdrs = ["commands.hpp"],
    visibility = ["//visibility:public"],
    deps = [
        "@com_nvidia_isaac//messages:proto_registry",
        "commands_proto"
    ]
)
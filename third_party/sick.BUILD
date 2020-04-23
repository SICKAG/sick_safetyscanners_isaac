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
date   2020-04-23
"""

cc_library(
    name = "lib_sick_safetyscanner",
    srcs = glob([
        "src/*.cpp",
        "src/**/*.cpp",
        "src/**/*.hpp"
    ]),
    hdrs = glob([
        "include/sick_safetyscanners_base/*.h",
        "include/sick_safetyscanners_base/**/*.h",
        "include/sick_safetyscanners_base/**/*.hpp",
    ]),
    includes = [
        "include",
        "src",
    ],
    deps = [
        "@boost//:system",
        "@boost//:thread",
        "@boost//:date_time",
        "@boost//:asio",
        "@boost//:lambda",
    ],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
)
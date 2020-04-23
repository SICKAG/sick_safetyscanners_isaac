"""
Copyright (c) 2019, NVIDIA CORPORATION. All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
 * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
 * Neither the name of NVIDIA CORPORATION nor the names of its
   contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
"""

workspace(name = "sick_safetyscanners")

# Point following dependency to Isaac SDK downloaded from https://developer.nvidia.com/isaac/downloads
local_repository(
    name = "com_nvidia_isaac",
    path = "/home/schulze/tmp/sick/isaac",
)

load("@com_nvidia_isaac//third_party:engine.bzl", "isaac_engine_workspace")
load("@com_nvidia_isaac//third_party:packages.bzl", "isaac_packages_workspace")
load("@com_nvidia_isaac//engine/build:isaac.bzl", "isaac_git_repository", "isaac_new_http_archive")

isaac_engine_workspace()

isaac_packages_workspace()

# Loads before boost to override for aarch64 specific config
isaac_new_http_archive(
    name = "org_lzma_lzma",
    build_file = "@com_nvidia_isaac//third_party:lzma.BUILD",
    licenses = ["@org_lzma_lzma//:COPYING"],
    sha256 = "9717ae363760dedf573dad241420c5fea86256b65bc21d2cf71b2b12f0544f4b",
    strip_prefix = "xz-5.2.4",
    type = "tar.xz",
    url = "https://developer.nvidia.com/isaac/download/third_party/xz-5-2-4-tar-xz",
)

# Loads boost c++ library (https://www.boost.org/) and
# custom bazel build support (https://github.com/nelhage/rules_boost/)
# explicitly due to bazel bug: https://github.com/bazelbuild/bazel/issues/1550
isaac_git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "9f9fb8b2f0213989247c9d5c0e814a8451d18d7f",
    licenses = ["@com_github_nelhage_rules_boost//:LICENSE"],
    remote = "https://github.com/nelhage/rules_boost.git",
    shallow_since = "1570056263 -0700",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

boost_deps()

# Configures toolchain
load("@com_nvidia_isaac//engine/build/toolchain:toolchain.bzl", "toolchain_configure")

toolchain_configure(name = "toolchain")

####################################################################################################

# SICK safety scanner specific dependencies
load(":repositories.bzl", "sick_safetyscanners_workspace")

sick_safetyscanners_workspace()

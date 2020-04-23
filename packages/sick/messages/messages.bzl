"""
Copyright (c) 2019, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.

file   messages.bzl
author Martin Schulze <schulze@fzi.de>
date   2020-04-15
"""
load("@com_nvidia_isaac//engine/build:cc_capnp_library.bzl", "cc_capnp_library")

# A list of all capnp files and their capnp dependencies
_protos = [
    ["safety_scan",        []],
    ["commands", []],
]

def _proto_library_name(x):
    ''' The library name for a proto '''
    return x + "_proto"

def create_message_proto_libraries():
    '''
    Creates one C++ library for every capnp file which is part of the message API
    '''
    for entry in _protos:
        cc_capnp_library(
            name = _proto_library_name(entry[0]),
            protos = [entry[0] + ".capnp"],
            deps = [_proto_library_name(x) for x in entry[1]])

def message_proto_library_names():
    '''
    Returns a list with all names of C++ library created by `create_message_proto_libraries`
    '''
    return [_proto_library_name(x[0]) for x in _protos]
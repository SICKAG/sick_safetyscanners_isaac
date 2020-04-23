"""
Copyright (c) 2019, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.

file   BUILD
author Martin Schulze <schulze@fzi.de>
date   2020-04-15
"""

load("@com_nvidia_isaac//engine/build:isaac.bzl", "isaac_cc_module", "isaac_component")


isaac_cc_module(
	name = "sick",
	deps = [
		"//packages/sick/components:sick_safety_scanner", 
		"//packages/sick/components:consumer",
	],
	visibility = ["//visibility:public"],
)
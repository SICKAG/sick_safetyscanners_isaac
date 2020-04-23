# Getting Started

## Prerequisites
- Since this package is an extension to NVIDIA ISAAC SDK, we refer the reader to check their current [prerequisites list](https://docs.nvidia.com/isaac/isaac/doc/setup.html#prerequisites).
  

## Installation

To build NVIDIA's ISAAC along with the driver for SICK AG Safety Scanners:

- Download ISAAC SDK (version 2019.3 or higher) from [https://developer.nvidia.com/isaac/downloads](https://developer.nvidia.com/isaac/downloads).
- Extract the ISAAC SDK release in a local folder.
- Install the [required dependencies](https://docs.nvidia.com/isaac/isaac/doc/setup.html#installing-dependencies-on-the-desktop) for ISAAC.
- Clone this repository into another local folder and open the WORKSPACE file in a text/code editor.
- Change the path '/home/schulze/tmp/sick/isaac' (line 32) to the path of the location where the ISAAC SDK has been extracted to.
- To build ISAAC SDK + Sick Safety Scanner package you can simply execute ```bazel build``` within the root folder of this package.

# Demonstration Apps
## Demo1: Receiving scan data
A consumer component is receiving the safety_scan proto messages and sends periodically a 'findMe' command proto to the sensor which causes the display of the sensor to blink in different colors for a few seconds. 

To run the demo on the desktop platform execute:

```bazel run //packages/sick/apps:sick_safetyscanner_test```

## Demo2: Visualization on websight
This demo launches ISAAC's websight server and transforms the data from flatscan protos to point cloud protos which in turn get visualized in the browser.

To run the demo on the desktop platform execute:

```bazel run //packages/sick/apps:sick_safetyscanner_websight```

Then open ```http://127.0.0.1:3000``` in your webbrowser. Enable the data stream for live-visualization by ticking the 'sick_safetyscanner_websight' checkbox (Channels) on the left side. The scan data is 2-dimensional, so you might need to adjust the camera properly. 

You can navigate in the 3D plot as follows:
- Hold the left mouse-button pressed + movement in any direction will let you look around.
- Hold the left mouse-button pressed + use the arrow keys on your keyboard to move the camera in any direction.

## Deployment on Jetson
This package is tested on Jetson Xavier and can be deployed and executed using the deploy.sh script as follows:

Demo1:
``` ./deploy.sh --remote_user <remote_username> -p //packages/sick/apps:sick_safetyscanner_test-pkg -d jetpack43 -h <jetson_ip_address> --run ```

Demo2:
``` ./deploy.sh --remote_user <remote_username> -p //packages/sick/apps:sick_safetyscanner_websight-pkg -d jetpack43 -h <jetson_ip_address> --run ```

(Open ```http://<jetson_ip_address>:3000``` in your browser)

If you wish not to immediately run the application after deployment, simply skip the ```--run``` flag.

## Inputs
| Proto       | Type               | Description                                             |
| ----------- | ------------------ | ------------------------------------------------------- |
| find_me_cmd | FindMeCommandProto | Find-Me sensor command relay with blink time [seconds]. |

## Outputs
| Name        | Type            | Description                                                                                                                                                                                                                    |
| ----------- | --------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| flatscan    | FlatscanProto   | A flatscan proto containing only the measurement data of the sensor. All angle values are given in [radians].                                                                                                                  |
| safety_scan | SafetyScanProto | Safety scan proto containing raw data from the sensor. Contains measurement data, derived values, the general system state, safety-field intrusion data and application related data. All angle values are given in [radians]. |
| output_path | OutputPathProto | Output paths, containing active monitoring case number, safe/valid flags and status. |



## Parameters

| Parameter                   | Description                                                               | Type        | Default         |
| --------------------------- | ------------------------------------------------------------------------- | ----------- | --------------- |
| sensor_ip                   | The IP4 address of the sensor (COLA2)                                     | std::string | "192.168.1.100" |
| host_ip                     | The IP4 address of the receiver of the sensor stream data                 | std::string | "192.168.1.100" |
| host_udp_port               | UDP port on the receiver of the sensor stream data                        | int         | 0               |
| tcp_port                    | TCP port of the sensor (COLA2)                                            | int         | 2122            |
| channel                     | The channel number used by the sensor                                     | int         | 0               |
| channel_enabled             | Determines whether to set the channel active                              | bool        | true            |
| flatscan_pub_active         | If enabled, flatscan protos are published                                 | bool        | false           |
| safety_pub_active           | If enabled, safety_scan protos are published                              | bool        | true            |
| outputpath_pub_active       | If enabled, outputPath protos are published                               | bool        | false           |
| angle_offset                | Additive offset of the angle (scan beams) [degree]                        | float       | -90.0f          |
| angle_start                 | Start angle (scan beams)                                                  | float       | 0.0f            |
| angle_end                   | End angle (scan beams)                                                    | float       | 0.0f            |
| general_system_state        | If enabled, safety_scan protos will contain this information as sub-proto | bool        | true            |
| derived_settings            | If enabled, safety_scan protos will contain this information as sub-proto | bool        | true            |
| measurement_data            | If enabled, safety_scan protos will contain this information as sub-proto | bool        | true            |
| intrusion_data              | If enabled, safety_scan protos will contain this information as sub-proto | bool        | true            |
| application_io_data         | If enabled, safety_scan protos will contain this information as sub-proto | bool        | true            |
| publishing_frequency_factor | A multiplicative factor to manipulate the publishing rate of the sensor.  | int         | 1               |
| receive_timeout             | Timeout limit on waiting for sensor data [milliseconds]                   | int         | 5000            |

## Maintainer
Martin Schulze

FZI Forschungszentrum Informatik
- [http://www.fzi.de](http://www.fzi.de)

on behalf of SICK AG
- [http://www.sick.com](http://www.sick.com)


## Copyright and License

Copyright (C) 2020, SICK AG, Waldkirch
Copyright (C) 2020, FZI Forschungszentrum Informatik, Karlsruhe, Germany

Licensed under the Apache License, Version 2.0 (the "License");
See the `LICENSE` file.


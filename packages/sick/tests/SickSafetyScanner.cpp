// #include "gtest/gtest.h"
// // #include "packages/sick/messages/safety_scan.capnp.h"
// #include <sick_safetyscanners_base/datastructure/ScanPoint.h>
// #include "packages/sick/messages/safety_scan.hpp"
// #include "capnp/message.h"

// namespace isaac
// {
// namespace sick_components
// {

// TEST(SafetyScan, ScanPointToProto)
// {
//     float angle_deg = 45.0;
//     float angle_offset = 0.0;
//     uint16_t distance = 100;
//     uint8_t reflectivity = 1;
//     bool valid_bit = true;
//     bool infinite_bit = false;
//     bool glare_bit = false;
//     bool reflector_bit = false;
//     bool contamination_bit = false;
//     bool contamination_warning_bit = false;

//     const sick::datastructure::ScanPoint point{
//         angle_deg,
//         distance,
//         reflectivity,
//         valid_bit,
//         infinite_bit,
//         glare_bit,
//         reflector_bit,
//         contamination_bit,
//         contamination_warning_bit};

//     ::capnp::MallocMessageBuilder message;
//     auto builder = message.initRoot<::SafetyScanProto>();

//     // ToProto(point, builder, angle_offset);
// }

// } // namespace sick_components
// } // namespace isaac
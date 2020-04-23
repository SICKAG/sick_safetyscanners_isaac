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
 * \file    Consumer.cpp
 *
 * \author  Martin Schulze <schulze@fzi.de>
 * \date    2020-04-15
 */
//----------------------------------------------------------------------

#include "Consumer.hpp"

#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <iterator>
#include <iostream>

#include <sick_safetyscanners_base/Types.h>

#include "messages/tensor.hpp"

namespace isaac
{
namespace sick_safetyscanners
{

void Consumer::start()
{
    LOG_INFO("Starting Consumer node");
    tickOnMessage(rx_safety_scan());
}

void Consumer::stop()
{
    LOG_INFO("Stopping Consumer node");
}

void Consumer::tick()
{
    // Send FindMeCommand with a blink time of 5 seconds at tick 100 the first time and from thereon every 200 ticks.
    if (getTickCount() % 200 == (0 + 100))
    {
        auto find_me_proto = tx_find_me_cmd().initProto();
        find_me_proto.setBlinkTime(5);
        tx_find_me_cmd().publish();
    }

    auto proto = rx_safety_scan().getProto();
    auto scan_number = proto.getHeader().getScanNumber();
    auto n_beams = proto.getMeasurementData().getNumberOfBeams();
    LOG_INFO("received scan# %d with %d beams", scan_number, n_beams);
}

} // namespace sick_components
} // namespace isaac

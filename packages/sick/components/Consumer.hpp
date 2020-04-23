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
 * \file    Consumer.hpp
 *
 * \author  Martin Schulze <schulze@fzi.de>
 * \date    2020-04-15
 */
//----------------------------------------------------------------------

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "engine/alice/alice.hpp"
#include "engine/alice/alice_codelet.hpp"
#include "messages/messages.hpp"
#include "engine/core/byte.hpp"

#include "messages/messages.hpp"

#include "packages/sick/messages/safety_scan.hpp"
#include "packages/sick/messages/commands.hpp"

namespace isaac
{

namespace sick_safetyscanners
{

class Consumer : public isaac::alice::Codelet
{
public:
    void start() override;
    void tick() override;
    void stop() override;

    ISAAC_PROTO_RX(SafetyScanProto, safety_scan);
    ISAAC_PROTO_TX(FindMeCommandProto, find_me_cmd);
};

} // namespace sick_components
} // namespace isaac

ISAAC_ALICE_REGISTER_CODELET(isaac::sick_safetyscanners::Consumer);

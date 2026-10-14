#pragma once

#include "zone/iw8_zone.h"

#include <cstdint>
#include <string>
#include <vector>

namespace iw8::rawfile
{
void Register(ZoneWriter &writer, const std::string &name, const std::vector<uint8_t> &data);
}

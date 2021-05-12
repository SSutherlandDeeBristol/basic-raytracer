#pragma once

#include <memory>
#include <vector>

namespace bv {
class Geometry;

// Loads the Cornell Box. It is scaled to fill the volume:
// -1 <= x <= +1
// -1 <= y <= +1
// -1 <= z <= +1
std::vector<std::shared_ptr<Geometry>> createCornellBox();
}

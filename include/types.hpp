#ifndef IMPLEMENTATION_TYPES_HPP
#define IMPLEMENTATION_TYPES_HPP

#include <cstdlib>
#include <cinttypes>
#include <functional>

using ElementID = uint16_t;
using size_t = std::size_t;
using Time = uint16_t;
using TimeOffset = uint16_t;
using ProbabilityGenerator = std::function<double()>;

#endif //IMPLEMENTATION_TYPES_HPP

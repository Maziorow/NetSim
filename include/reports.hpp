#ifndef NETSIM_REPORTS_HPP
#define NETSIM_REPORTS_HPP

#include "types.hpp"
#include "factory.hpp"


#include <vector>
#include <map>
#include <string>
#include <istream>
#include <ostream>
#include <iostream>
#include <sstream>

void generate_structure_report(const Factory& f, std::ostream& os);
void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t);

#endif //NETSIM_REPORTS_HPP

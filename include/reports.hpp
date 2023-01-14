#ifndef NETSIM_REPORTS_HPP
#define NETSIM_REPORTS_HPP

#include "types.hpp"
#include "factory.hpp"


#include <vector>
#include <string>
#include <ostream>
#include <sstream>
#include <set>

void generate_structure_report(const Factory& f, std::ostream& os);
void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t);

class IntervalReportNotifier{
public:
    IntervalReportNotifier(TimeOffset to): to_(to) {};
    bool should_generate_report(Time t);
private:
    TimeOffset to_;
};

class SpecificTurnsReportNotifier{
public:
    SpecificTurnsReportNotifier(std::set<Time> turns): turns_(turns) {};
    bool should_generate_report(Time t);
private:
    std::set<Time> turns_;
};

#endif //NETSIM_REPORTS_HPP

#include "simulation.hpp"
#include "factory.hpp"

void simulate(Factory& f, TimeOffset d, std::function<void(Factory&, Time)> rf) {
    for(Time i = 1;i<d;i++){
        f.is_consistent();
        f.do_deliveries(i);
        f.do_package_passing();
        f.do_work(i);
        rf(f,i);
    }
}

#include "types.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"
#include "reports.hpp"
#include "nodes.hpp"
#include "factory.hpp"

#include <vector>
#include <string>
#include <ostream>


void generate_structure_report(const Factory& f, std::ostream& os){
    os << std::endl << "== LOADING RAMPS ==";

    std::vector<ElementID> sorted_ramps_ids;
    for(auto it = f.ramp_cbegin(); it != f.ramp_cend(); ++it) {
        sorted_ramps_ids.push_back(it->get_id());
    }
    std::sort(sorted_ramps_ids.begin(),sorted_ramps_ids.end());
    for(auto el : sorted_ramps_ids){
        os << std::endl << "LOADING RAMP #" << el << std::endl;
        os << "  Delivery interval: " << f.find_ramp_by_id(el)->get_delivery_interval() << std::endl;
        os << "  Receivers:" << std::endl;
        auto pref = f.find_ramp_by_id(el)->receiver_preferences_.get_preferences();
        std::vector<ElementID> sorted_workers_ids;
        std::vector<ElementID> sorted_storehouses_ids;
        for(auto node : pref){
            if(node.first->get_receiver_type() == ReceiverType::WORKER){
                sorted_workers_ids.push_back(node.first->get_id());
            }
            else if(node.first->get_receiver_type() == ReceiverType::STOREHOUSE){
                sorted_storehouses_ids.push_back(node.first->get_id());
            }
        }
        std::sort(sorted_workers_ids.begin(),sorted_workers_ids.end());
        std::sort(sorted_storehouses_ids.begin(),sorted_storehouses_ids.end());
        for(auto sh : sorted_storehouses_ids){
            os << "    storehouse #" << sh << std::endl;
        }
        for(auto wo : sorted_workers_ids){
            os << "    workers #" << wo << std::endl;
        }
    }

    os << std::endl << std::endl << "== WORKERS ==";
    std::vector<ElementID> sorted_workers_ids;
    for(auto it = f.worker_cbegin(); it != f.worker_cend(); ++it) {
        sorted_workers_ids.push_back(it->get_id());
    }
    std::sort(sorted_workers_ids.begin(),sorted_workers_ids.end());
    for(auto el : sorted_workers_ids){
        os << std::endl << "WORKER #" << el << std::endl;
        os << "  Processing time: " << f.find_worker_by_id(el)->get_processing_duration() << std::endl;
        os << "  Queue type: " << ((f.find_worker_by_id(el)->get_queue()->get_queue_type() == PackageQueueType::LIFO) ? "LIFO" : "FIFO") << std::endl;
        os << "  Receivers:" << std::endl;
        auto pref = f.find_ramp_by_id(el)->receiver_preferences_.get_preferences();
        std::vector<ElementID> sorted_workers_ids_inner;
        std::vector<ElementID> sorted_storehouses_ids;
        for(auto node : pref){
            if(node.first->get_receiver_type() == ReceiverType::WORKER){
                sorted_workers_ids_inner.push_back(node.first->get_id());
            }
            else if(node.first->get_receiver_type() == ReceiverType::STOREHOUSE){
                sorted_storehouses_ids.push_back(node.first->get_id());
            }
        }
        std::sort(sorted_workers_ids.begin(),sorted_workers_ids.end());
        std::sort(sorted_storehouses_ids.begin(),sorted_storehouses_ids.end());
        for(auto sh : sorted_storehouses_ids){
            os << "    storehouse #" << sh << std::endl;
        }
        for(auto wo : sorted_workers_ids){
            os << "    workers #" << wo << std::endl;
        }
    }

    os << std::endl << std::endl << "== STOREHOUSES ==";
    std::vector<ElementID> sorted_storehouses_ids;
    for(auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); ++it) {
        sorted_storehouses_ids.push_back(it->get_id());
    }
    std::sort(sorted_workers_ids.begin(),sorted_workers_ids.end());
    for(auto el : sorted_storehouses_ids){
        os << std::endl << "STOREHOUSE #" << el;
    }
    os << std::endl;
}

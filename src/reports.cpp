#include "reports.hpp"
#include "types.hpp"
#include "factory.hpp"


#include <vector>
#include <map>
#include <string>
#include <istream>
#include <ostream>
#include <iostream>
#include <sstream>
#include <set>


void generate_structure_report(const Factory& f, std::ostream& os){
    os << std::endl << "== LOADING RAMPS ==" << std::endl;

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
            os << "    worker #" << wo << std::endl;
        }
    }
    os << std::endl;

    os << std::endl << "== WORKERS ==" << std::endl;
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
        auto pref = f.find_worker_by_id(el)->receiver_preferences_.get_preferences();
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
        std::sort(sorted_workers_ids_inner.begin(),sorted_workers_ids_inner.end());
        std::sort(sorted_storehouses_ids.begin(),sorted_storehouses_ids.end());
        for(auto sh : sorted_storehouses_ids){
            os << "    storehouse #" << sh << std::endl;
        }
        for(auto wo : sorted_workers_ids_inner){
            os << "    worker #" << wo << std::endl;
        }
    }

    os << std::endl << std::endl << "== STOREHOUSES ==" << std::endl;
    std::vector<ElementID> sorted_storehouses_ids;
    for(auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); ++it) {
        sorted_storehouses_ids.push_back(it->get_id());
    }
    std::sort(sorted_workers_ids.begin(),sorted_workers_ids.end());
    for(auto el : sorted_storehouses_ids){
        os << std::endl << "STOREHOUSE #" << el << std::endl;
    }
    os << std::endl;
}

void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t){
    os << "=== [ Turn: " << t << " ] ===" << std::endl << std::endl;
    os << "== WORKERS ==" << std::endl;

    std::vector<ElementID> sorted_workers_ids;
    for(auto it = f.worker_cbegin(); it != f.worker_cend(); ++it) {
        sorted_workers_ids.push_back(it->get_id());
    }
    std::sort(sorted_workers_ids.begin(),sorted_workers_ids.end());

    for(auto el : sorted_workers_ids){
        os << std::endl << "WORKER #" << el << std::endl;
        std::string buffer = "";
        if(f.find_worker_by_id(el)->get_processing_buffer()){
            buffer = "#" + std::to_string(f.find_worker_by_id(el)->get_processing_buffer()->get_id()) + " (pt = " + std::to_string(f.find_worker_by_id(el)->get_processing_time()) + ")";
        }else{
            buffer = "(empty)";
        }
        os << "  PBuffer: " << buffer << std::endl;
        std::string queue = "";
        if(f.find_worker_by_id(el)->get_queue()->size() == 0){
            queue = " (empty)";
        }else{
            std::set<ElementID> queue_ids;
            for(auto&id : *f.find_worker_by_id(el)->get_queue())
                queue_ids.insert(id.get_id());
            for(auto id : queue_ids){
                queue += (" #" + std::to_string(id));
                if (id != *queue_ids.rbegin())
                    queue += ",";
            }
        }
        os << "  Queue:" << queue << std::endl;

        std::string sbuffer = "";
        if(f.find_worker_by_id(el)->buffer.has_value()){
            sbuffer = ("#" + std::to_string(f.find_worker_by_id(el)->buffer->get_id()));
        }else{
            sbuffer = "(empty)";
        }
        os << "  SBuffer: " << sbuffer << std::endl;
    }

    os << std::endl << std::endl << "== STOREHOUSES ==" << std::endl;
    std::vector<ElementID> sorted_storehouses_ids;
    for(auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); ++it) {
        sorted_storehouses_ids.push_back(it->get_id());
    }

    std::sort(sorted_storehouses_ids.begin(),sorted_storehouses_ids.end());
    for(auto el : sorted_storehouses_ids){
        os << std::endl << "STOREHOUSE #" << el << std::endl;
        std::set<ElementID> stock_ids;
        std::string stock = "";
        for(auto& id : *f.find_storehouse_by_id(el)->get_stockpile()){
            stock_ids.insert(id.get_id());
        }
        if(stock_ids.empty()){
            stock = " (empty)";
        }else{
            for(auto id : stock_ids) {
                stock += (" #" + std::to_string(id));
                if (id != *stock_ids.rbegin())
                    stock += ",";
            }
        }
        os << "  Stock:" << stock << std::endl;
    }
    os << std::endl;
}

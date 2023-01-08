#include "factory.hpp"
#include "storage_types.hpp"

#include <algorithm>
#include <map>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <vector>


template <class T>
void NodeCollection<T>::remove_by_id(ElementID id){
    iterator to_be_removed = std::find_if(node_.begin(),node_.end(), [&] (T & p) {return p.get_id() ==id;});
    node_.erase(to_be_removed);
}

template <class T>
typename std::vector<T>::iterator NodeCollection<T>::find_by_id(ElementID id) {
    iterator found = std::find_if(node_.begin(),node_.end(), [&] (T & p) {return p.get_id() ==id;});
    return found;
}


template <class T>
typename std::vector<T>::const_iterator NodeCollection<T>::find_by_id(ElementID id) const {
    const_iterator found = std::find_if(node_.begin(),node_.end(), [&] (T const& p) {return p.get_id() ==id;});
    return found;
}


enum class NodeColor { UNVISITED, VISITED, VERIFIED };


bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors){
    if (node_colors[sender] == NodeColor::VERIFIED)
        return true;
    node_colors[sender] = NodeColor::VISITED;

    if(sender->receiver_preferences_.preferences_.empty())
        throw std::logic_error("No receivers");

    bool non_self_receivers = false;

    for(auto&el : sender->receiver_preferences_.preferences_){
        if (el.first->get_receiver_type() == ReceiverType::STOREHOUSE){
            non_self_receivers = true;
        }
        else if (el.first->get_receiver_type() == ReceiverType::WORKER){
            IPackageReceiver* receiver_ptr = dynamic_cast<IPackageReceiver*>(el.first);
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            if(sendrecv_ptr == sender){
                continue;
            }
            non_self_receivers = true;

            if(node_colors[sendrecv_ptr] == NodeColor::UNVISITED){
                has_reachable_storehouse(sendrecv_ptr,node_colors);
            }
        }
    }

    node_colors[sender] = NodeColor::VERIFIED;
    if(non_self_receivers){
        return true;
    }
    else{
        throw std::logic_error("No receivers");
    }

}


bool Factory::is_consistent(){
    std::map<const PackageSender*, NodeColor> color;
    for(auto& el : ramps_){
        PackageSender* up_cast = dynamic_cast<PackageSender*>(&el);
        color[up_cast] = NodeColor::UNVISITED;
    }
    for(auto& el : workers_){
        PackageSender* up_cast = dynamic_cast<PackageSender*>(&el);
        color[up_cast] = NodeColor::UNVISITED;
    }
    try{
        for(auto& el : ramps_)
        {
            PackageSender* up_cast = dynamic_cast<PackageSender*>(&el);
            has_reachable_storehouse(up_cast, color);
        }
        return true;
    }
    catch (const std::logic_error& e){
        std::cout << e.what() << '\n';
        return false;
    }

}


void Factory::do_deliveries(Time t) {
    for(auto& el: ramps_){
        el.deliver_goods(t);
    }
}


void Factory::do_package_passing() {
    for(auto& el: ramps_){
        el.send_package();
    }
    for(auto& el: workers_){
        el.send_package();
    }
}


void Factory::do_work(Time t) {
    for(auto& el: workers_){
        el.do_work(t);
    }
}

template<class T>
void Factory::remove_receiver(NodeCollection<T>& collection, ElementID id) {
     if constexpr (!std::is_same_v<T, Ramp>) {
         auto type = collection.find_by_id(id)->get_receiver_type();
         for (auto& el: ramps_) {
             auto pref = el.receiver_preferences_.get_preferences();
             for (auto& pref_r: pref) {
                 if (pref_r.first->get_receiver_type() == type && pref_r.first->get_id() == id) {
                     el.receiver_preferences_.remove_receiver(pref_r.first);
                 }
             }
         }
         for (auto& el: workers_) {
             auto pref = el.receiver_preferences_.get_preferences();
             for (auto& pref_w: pref) {
                 if (pref_w.first->get_receiver_type() == type && pref_w.first->get_id() == id) {
                     el.receiver_preferences_.remove_receiver(pref_w.first);
                 }
             }
         }
     }
     collection.remove_by_id(id);
}



ParsedLineData parse_line(std::string line){
    std::vector<std::string> keys = {"LOADING_RAMP","WORKER","STOREHOUSE","LINK"};
    std::vector<std::string> tokens;
    std::string token;
    std::string key;
    ParsedLineData parsed;

    char delimiter = ' ';

    std::istringstream token_stream(line);
    while (std::getline(token_stream, token, delimiter)){
        tokens.push_back(token);
    }
    if(std::find(keys.begin(),keys.end(), tokens[0]) != keys.end()){
        throw std::logic_error("Invalid key while loading from file");
    }
    key = tokens[0];
    tokens.erase(tokens.begin());

    parsed.element_type = key;
    for(auto el : tokens){
        std::string element;
        std::vector<std::string> elements;
        //std::basic_istringstream el_stream(el);
        while(std::getline(el_stream,element,'=')){
            elements.push_back(element);
        }
        parsed.parameters.insert(std::pair<std::string,std::string>(elements[0],elements[1]));
    }
    return parsed;
}


Factory load_factory_structure(std::istream& is){

    Factory factory;

    std::string line;
    //std::map<std::string, int> type_map {{"LOADING_RAMP",0}, {"WORKER",1}, {"STOREHOUSE",2}, {"LINK",3}};

    while (std::getline(is, line)) {
        if (line[0] != ';' && line[0] != ' ') {
            ParsedLineData parsed_line = parse_line(line);

            /**
                    switch (type_map[parsed_line.element_type]){
                        case 0: //LOADING_RAMP
                            factory.add_ramp(Ramp(std::stoi(parsed_line.parameters["id"]), std::stoi(parsed_line.parameters["delivery-interval"])));
                            break;
                        case 1: //WORKER
                            ElementID id = std::stoi(parsed_line.parameters["id"]);
                            TimeOffset processing_time = std::stoi(parsed_line.parameters["processing-time"]);
                            if(parsed_line.parameters["queue-type"] == "FIFO") {
                                Worker worker_ = Worker(id, processing_time,std::make_unique<PackageQueue>(PackageQueueType::FIFO));
                            }
                            if(parsed_line.parameters["queue-type"] == "LIFO"){
                                Worker worker_ = Worker(id, processing_time,std::make_unique<PackageQueue>(PackageQueueType::LIFO));
                            }
                            break;
                        case 2: //STOREHOUSE
                            factory.add_storehouse(Storehouse(std::stoi(parsed_line.parameters["id"])));
                            break;
                        case 3: //LINK
                            break;

                    }
            **/

            
        }
    }
}


void save_factory_structure(Factory& factory, std::ostream os){

    os << "; == LOADING RAMPS ==" << std::endl << std::endl;
    for(auto it = factory.ramp_begin(); it != factory.ramp_cend(); it++)
        os << "LOADING_RAMP id=" << it->get_id()<< " delivery-interval="<< it->get_delivery_interval() << std::endl;


    os << std::endl << "; == WORKERS ==" << std::endl << std::endl;
    for(auto it = factory.worker_begin(); it != factory.worker_cend(); it++)
        os <<"WORKER id=" << it->get_id()<< " processing-time="<< it->get_processing_duration() <<" queue-type="<< it->get_queue()<< std::endl;


    os << std::endl << "; == STOREHOUSES ==" << std::endl << std::endl;
    for(auto it = factory.storehouse_begin(); it != factory.storehouse_cend(); it++)
        os <<"STOREHOUSE id=" << it->get_id() << std::endl;


    os << std::endl << "; == LINKS ==" << std::endl << std::endl;


    //std::for_each();



    os.flush();


}

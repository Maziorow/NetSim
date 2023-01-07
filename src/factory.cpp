#include "factory.hpp"

#include <algorithm>
#include <map>
#include <stdexcept>
#include <iostream>


template <class T>
void NodeCollection<T>::remove_by_id(ElementID id){
    iterator to_be_removed = std::find_if(node_.cbegin(),node_.cend(), [&] (T const& p) {return p.get_id() ==id;});
    node_.erase(to_be_removed);
}

template <class T>
typename std::vector<T>::iterator NodeCollection<T>::find_by_id(ElementID id) {
    iterator found = std::find_if(node_.cbegin(),node_.cend(), [&] (T const& p) {return p.get_id() ==id;});
    return found;
}

template <class T>
typename std::vector<T>::const_iterator NodeCollection<T>::find_by_id(ElementID id) const {
    iterator found = std::find_if(node_.cbegin(),node_.cend(), [&] (T const& p) {return p.get_id() ==id;});
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
            PackageSender* const up_cast = dynamic_cast<PackageSender* const>(&el);
            has_reachable_storehouse(up_cast, color);
        }


    }

    catch (const std::logic_error& e){
        std::cout << e.what() << '\n';
        return false;
    }


}

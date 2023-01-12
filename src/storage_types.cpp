#include "storage_types.hpp"

Package PackageQueue::pop(){
    Package elem;
    switch(queue_type_){
        case PackageQueueType::LIFO:
            Package::freed_IDs.insert(elem.get_id());
            Package::assigned_IDs.erase(elem.get_id());
            elem = std::move(*queue_.end());
            queue_.pop_back();
            break;
        case PackageQueueType::FIFO:
            Package::freed_IDs.insert(elem.get_id());
            Package::assigned_IDs.erase(elem.get_id());
            elem = std::move(*queue_.begin());
            queue_.pop_front();
            break;
        default:

            break;//TODO
    }
    return elem;
}
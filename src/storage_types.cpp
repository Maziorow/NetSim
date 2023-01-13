#include "storage_types.hpp"

Package PackageQueue::pop(){
    Package elem;
    Package::freed_IDs.insert(elem.get_id());
    Package::assigned_IDs.erase(elem.get_id());
    switch(queue_type_){
        case PackageQueueType::LIFO:
            elem = std::move(*queue_.end());
            queue_.pop_back();
            break;
        case PackageQueueType::FIFO:
            elem = std::move(*queue_.begin());
            queue_.pop_front();
            break;
        default:

            break;//TODO
    }
    return elem;
}
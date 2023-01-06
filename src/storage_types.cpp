#include "storage_types.hpp"

Package PackageQueue::pop(){
    Package elem;
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
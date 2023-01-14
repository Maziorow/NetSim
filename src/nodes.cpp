#include "nodes.hpp"
#include <cmath>
#include <iterator>
#include <memory>


void ReceiverPreferences::add_receiver(IPackageReceiver* r){
    preferences_.insert(std::pair<IPackageReceiver*, double>(r,1.0));
    int size = preferences_.size();
    double newProbability = 1.0/size;
    for(auto&el : preferences_){
        el.second = newProbability;
    }

}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    if(!preferences_.empty()){
        if(preferences_.find(r)!=preferences_.end()){
            preferences_.erase(preferences_.find(r));
        }
        if(!preferences_.empty()){
            int size = preferences_.size();
            double newProbability = 1.0/size;
            for(auto&el : preferences_){
                el.second = newProbability;
            }
        }
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver(){
    double rand = rand_function();
    rand *= preferences_.size();
    int offset = floor(rand);
    const_iterator new_iter = preferences_.cbegin();
    std::advance(new_iter,offset);
    return (*new_iter).first;
}





void Ramp::deliver_goods(Time t){
    /**
    if(t == 1 || t - lastSent_ >= di_)
    {
        lastSent_ = t;
        push_package(Package());
    }
     **/
     if((t-1)%di_ == 0){
         push_package(Package());
     }
}

void Worker::do_work(Time t) {
    /**
    if (t - t_ >= pd_ && not q_->empty()){
        t_ = t;
        push_package(q_->pop());
    }
}
     **/
     if(now_processed.has_value() || !(q_->empty())){
        if(actual_processing_time_ == 1 ){
            processing_start_time_ = t;
            actual_processing_time_ = t + pd_ - 1;
            now_processed.emplace(q_->pop());
        }if(actual_processing_time_ == t){
            actual_processing_time_ = 1;
            push_package(std::move(now_processed.value()));
            now_processed = std::nullopt;
        }
     }
}

void PackageSender::push_package(Package&& p) {
        buffer = std::move(p);
}

void PackageSender::send_package() {
    if(buffer){
        IPackageReceiver* receiver = receiver_preferences_.choose_receiver();
        receiver->receive_package(std::move(buffer.value()));
        buffer = std::nullopt;
    }
}



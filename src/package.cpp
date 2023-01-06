#include "package.hpp"
#include "types.hpp"
#include <set>
#include <algorithm>
#include <iostream>

std::set<ElementID> Package::assigned_IDs;
std::set<ElementID> Package::freed_IDs;

Package::Package(){
    /**
    if(!freed_IDs.empty()){
        ElementID newID = *freed_IDs.rbegin();
        freed_IDs.erase(newID);
        assigned_IDs.insert(newID);
        id_ = newID;
    }
    else{
        if(assigned_IDs.empty()) {
            assigned_IDs.insert(1);
            id_ = 1;
        }else{
            ElementID newID = *assigned_IDs.rend();
            //newID++; //next highest index after all already assigned
            assigned_IDs.insert(newID);
            id_ = newID;
        }
    }
    **/
    if(!(freed_IDs.empty())){
        ElementID minID = *freed_IDs.begin();
        id_ = minID;
        freed_IDs.erase(minID);
        assigned_IDs.insert(minID);
    }
    else{
        if(assigned_IDs.empty()){
            id_ = 1;
            assigned_IDs.insert(1);
        }
        else{
            ElementID maxID = *std::max_element(assigned_IDs.begin(),assigned_IDs.end());
            id_ = maxID++;
            assigned_IDs.insert(maxID++);
        }
    }
    std::cout << id_ << std::endl;
}

Package::Package(Package&& other) noexcept {
    id_ = other.id_;
}

Package::~Package(){
    assigned_IDs.erase(id_);
    freed_IDs.insert(id_);
 // TODO: może destruktor
}

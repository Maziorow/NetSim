#include "package.hpp"
#include "types.hpp"
#include <set>
#include <algorithm>

std::set<ElementID> Package::assigned_IDs;
std::set<ElementID> Package::freed_IDs;

Package::Package(){
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
}

Package::Package(Package&& other) noexcept {
    id_ = other.id_;
}

Package::~Package(){
    assigned_IDs.erase(id_);
    freed_IDs.insert(id_);
}

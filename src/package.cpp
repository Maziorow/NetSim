#include "package.hpp"
#include "types.hpp"
#include <set>
#include <algorithm>

std::set<ElementID> Package::assigned_IDs;
std::set<ElementID> Package::freed_IDs;

Package::Package(){
    if(!freed_IDs.empty()){
        ElementID newID = *std::min_element(freed_IDs.begin(),freed_IDs.end());
        freed_IDs.erase(newID);
        assigned_IDs.insert(newID);
        id_ = newID;
    }
    else{
        if(!assigned_IDs.empty()) {
            assigned_IDs.insert(0);
            id_ = 0;
        }else{
            ElementID newID = *std::max_element(assigned_IDs.begin(),assigned_IDs.end());
            assigned_IDs.insert(newID + 1);
            id_ = newID + 1;
        }
    }
}

Package::~Package(){
    assigned_IDs.erase(id_);
    freed_IDs.insert(id_);
 // TODO: może destruktor
}

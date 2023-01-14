#ifndef IMPLEMENTATION_PACKAGE_HPP
#define IMPLEMENTATION_PACKAGE_HPP

#include "types.hpp"

#include <set>

class Package{
public:
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;

    Package();
    Package(ElementID id): id_(id) {};
    Package(Package&& other) noexcept;
    ~Package();

    ElementID get_id() const { return id_; }

    Package& operator=(Package&&) = default;

private:
    ElementID id_;
};


#endif //IMPLEMENTATION_PACKAGE_HPP


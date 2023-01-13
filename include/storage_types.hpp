#ifndef IMPLEMENTATION_STORAGE_TYPES_HPP
#define IMPLEMENTATION_STORAGE_TYPES_HPP

#include "package.hpp"
#include "types.hpp"
#include <list>
#include <algorithm>

enum class PackageQueueType{
    FIFO,
    LIFO
};

class IPackageStockpile {
public:
    using iterator= std::list<Package>::iterator;
    using const_iterator= std::list<Package>::const_iterator;

    virtual void push(Package&&) = 0;
    virtual bool empty() = 0;
    virtual size_t size() = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;
    virtual iterator begin() = 0;
    virtual iterator end() = 0;

    virtual ~IPackageStockpile() {}

};


class IPackageQueue : public IPackageStockpile{
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() = 0;

    virtual ~IPackageQueue() {}
};

class PackageQueue : public IPackageQueue{
public:
    PackageQueue(PackageQueueType queue_type) { queue_type_ = queue_type; };

    void push(Package&& package) override { queue_.push_back(std::move(package)); }
    bool empty() override { return queue_.empty(); }
    size_t size() override { return std::size(queue_); }
    Package pop() override;
    PackageQueueType get_queue_type() override { return queue_type_; }
    const_iterator cbegin() const override {return queue_.cbegin(); }
    const_iterator cend() const override {return queue_.cend(); }
    iterator begin() override {return queue_.begin(); }
    iterator end() override {return queue_.end(); }

private:
    PackageQueueType queue_type_;
    std::list<Package> queue_;
};

#endif //IMPLEMENTATION_STORAGE_TYPES_HPP
#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include <map>
#include <memory>
#include <optional>

#include "types.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"

enum class ReceiverType{
    WORKER,
    STOREHOUSE
};


class IPackageReceiver{
public:
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;
    virtual ReceiverType get_receiver_type() const = 0;
    virtual ~IPackageReceiver() {}

};


class ReceiverPreferences{
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences() = default;
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();
    preferences_t& get_preferences() {return preferences_;}
    preferences_t preferences_;
    ProbabilityGenerator rand_function = probability_generator;

    const_iterator cbegin() {return preferences_.cbegin(); }
    const_iterator cend() {return preferences_.cend(); }
    const_iterator begin() {return preferences_.begin(); }
    const_iterator end() {return preferences_.end(); }
};


class PackageSender{
public:
    PackageSender() = default;
    PackageSender(PackageSender&& p) = default;
    void send_package();
    std::optional<Package>& get_sending_buffer() {return buffer;}

    ReceiverPreferences receiver_preferences_ = ReceiverPreferences();
    std::optional<Package> buffer = std::nullopt;
protected:
    void push_package(Package&& p);
};


class Ramp: public PackageSender{
public:
    Ramp(ElementID id, TimeOffset di): id_(id), di_(di) {};
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() const {return di_; }
    ElementID get_id() const { return id_; }

private:
    ElementID id_;
    TimeOffset di_;
    Time lastSent_ = 0;
};


class Worker : public PackageSender, public IPackageReceiver{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q): id_(id), pd_(pd), q_(std::move(q)) {};

    void do_work(Time t);
    TimeOffset get_processing_duration() const { return pd_; }
    Time get_package_processing_start_time() const { return t_; }
    ElementID get_id() const override {return id_; }
    ReceiverType get_receiver_type() const override { return ReceiverType::WORKER; }
    void receive_package(Package&& p) override { q_->push(std::move(p)); }

private:
    ElementID id_;
    TimeOffset pd_;
    Time t_ = 0;
    std::unique_ptr<IPackageQueue> q_;
};


class Storehouse : public IPackageReceiver{
public:
    Storehouse(ElementID id): id_(id), d_(std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::FIFO))) {};
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d): id_(id), d_(std::move(d)) {};

    ElementID get_id() const override {return id_; }
    ReceiverType get_receiver_type() const override { return ReceiverType::STOREHOUSE; }
    void receive_package(Package&& p) override { d_->push(std::move(p)); }

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};


#endif //NETSIM_NODES_HPP


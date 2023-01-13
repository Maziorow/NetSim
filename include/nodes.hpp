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
    using iterator = preferences_t::iterator;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences() = default;
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();
    //preferences_t& get_preferences() {return preferences_;}
    const preferences_t& get_preferences() const {return preferences_;}
    preferences_t preferences_;
    ProbabilityGenerator rand_function = probability_generator;

    const_iterator cbegin() const {return preferences_.cbegin(); }
    const_iterator cend() const {return preferences_.cend(); }
    iterator begin() {return preferences_.begin(); }
    iterator end() {return preferences_.end(); }
};


class PackageSender{
public:
    PackageSender() = default;
    PackageSender& operator=(PackageSender&&) = default;
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
    using iterator= std::list<Package>::iterator;
    using const_iterator= std::list<Package>::const_iterator;
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q): id_(id), pd_(pd), q_(std::move(q)) {};

    void do_work(Time t);
    const std::optional<Package>& get_processing_buffer() const { return now_processed; }
    TimeOffset get_processing_duration() const { return pd_; }
    Time get_processing_time() const {return actual_processing_time_-processing_start_time_; }
    Time get_package_processing_start_time() const { return t_; }
    ElementID get_id() const override {return id_; }
    ReceiverType get_receiver_type() const override { return ReceiverType::WORKER; }
    void receive_package(Package&& p) override { q_->push(std::move(p)); }
    IPackageQueue* get_queue() const { return q_.get(); }

    const_iterator cbegin() const {return q_->cbegin(); }
    const_iterator cend() const {return q_->cend(); }
    iterator begin() {return q_->begin(); }
    iterator end() {return q_->end(); }

private:
    ElementID id_;
    TimeOffset pd_;
    Time t_ = 0;
    Time processing_start_time_ = 1;
    Time actual_processing_time_ = 1;
    std::unique_ptr<IPackageQueue> q_;
    std::optional<Package> now_processed;
};


class Storehouse : public IPackageReceiver{
public:
    using iterator= std::list<Package>::iterator;
    using const_iterator= std::list<Package>::const_iterator;
    Storehouse(ElementID id): id_(id), d_(std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::FIFO))) {};
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d): id_(id), d_(std::move(d)) {};

    ElementID get_id() const override {return id_; }
    ReceiverType get_receiver_type() const override { return ReceiverType::STOREHOUSE; }
    void receive_package(Package&& p) override { d_->push(std::move(p)); }
    IPackageStockpile* get_stockpile() const { return d_.get(); }

    const_iterator cbegin() const {return d_->cbegin(); }
    const_iterator cend() const {return d_->cend(); }
    iterator begin() {return d_->begin(); }
    iterator end() {return d_->end(); }

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};


#endif //NETSIM_NODES_HPP


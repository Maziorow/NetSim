#ifndef NETSIM_FACTORY_HPP
#define NETSIM_FACTORY_HPP

#include "nodes.hpp"

#include <vector>
#include <map>

template <class T>
        class NodeCollection{
public:

    using container_t = typename std::vector<T>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    NodeCollection() = default;
    void add(T&& node){ node_.push_back(std::move(node)); }
    void remove_by_id(ElementID id);
    iterator find_by_id(ElementID id);
    const_iterator find_by_id(ElementID id) const;
    iterator begin() {return node_.begin();}
    iterator end() {return node_.end();}
    const_iterator cbegin() const {return node_.cbegin();}
    const_iterator cend() const {return node_.cend();}

    std::vector<T> node_;
};


enum class NodeColor;
bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors);


class Factory{
public:
    void add_ramp(Ramp&& ramp) { ramps_.add(std::move(ramp)); }
    void remove_ramp(ElementID id) { ramps_.remove_by_id(id); }
    NodeCollection<Ramp>::iterator       find_ramp_by_id(ElementID id) { ramps_.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const { ramps_.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const {return ramps_.cbegin(); }
    NodeCollection<Ramp>::const_iterator ramp_cend()   const {return ramps_.cend(); }
    NodeCollection<Ramp>::iterator ramp_begin() {return ramps_.begin(); }
    NodeCollection<Ramp>::iterator ramp_end() {return ramps_.end(); }

    void add_worker(Worker&& worker) { workers_.add(std::move(worker)); }
    void remove_worker(ElementID id) { workers_.remove_by_id(id); }
    NodeCollection<Worker>::iterator       find_worker_by_id(ElementID id) { workers_.find_by_id(id); }
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const{ workers_.find_by_id(id); }
    NodeCollection<Worker>::const_iterator worker_cbegin() const {return workers_.cbegin(); }
    NodeCollection<Worker>::const_iterator worker_cend()   const {return workers_.cend(); }
    NodeCollection<Worker>::iterator worker_begin()  {return workers_.begin(); }
    NodeCollection<Worker>::iterator worker_end() {return workers_.end(); }

    void add_storehouse(Storehouse&& storehouse) { storehouses_.add(std::move(storehouse)); }
    void remove_storehouse(ElementID id) {storehouses_.remove_by_id(id); }
    NodeCollection<Storehouse>::iterator       find_storehouse_by_id(ElementID id) { storehouses_.find_by_id(id); }
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const { storehouses_.find_by_id(id); }
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const {return storehouses_.cbegin(); }
    NodeCollection<Storehouse>::const_iterator storehouse_cend()   const {return storehouses_.cend(); }
    NodeCollection<Storehouse>::iterator storehouse_begin() {return storehouses_.begin(); }
    NodeCollection<Storehouse>::iterator storehouse_end()  {return storehouses_.end(); }


    bool is_consistent();
    void do_deliveries(Time t);
    void do_package_passing();
    void do_work(Time t);

private:
    template<typename T>
    void remove_receiver(NodeCollection<T>& collection, ElementID id);

    NodeCollection<Ramp> ramps_;
    NodeCollection<Worker> workers_;
    NodeCollection<Storehouse> storehouses_;
};



#endif //NETSIM_FACTORY_HPP

#ifndef NETSIM_FACTORY_HPP
#define NETSIM_FACTORY_HPP

#include "nodes.hpp"
#include "storage_types.hpp"

#include <vector>
#include <map>
#include <string>
#include <ostream>
#include <iostream>
#include <sstream>

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


enum class NodeColor { UNVISITED, VISITED, VERIFIED };
bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors);



class Factory{
public:
    void add_ramp(Ramp&& ramp) { ramps_.add(std::move(ramp)); }
    void remove_ramp(ElementID id) { remove_receiver(ramps_, id); }
    NodeCollection<Ramp>::iterator       find_ramp_by_id(ElementID id) { return ramps_.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const { return ramps_.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const {return ramps_.cbegin(); }
    NodeCollection<Ramp>::const_iterator ramp_cend()   const {return ramps_.cend(); }
    NodeCollection<Ramp>::iterator ramp_begin() {return ramps_.begin(); }
    NodeCollection<Ramp>::iterator ramp_end() {return ramps_.end(); }

    void add_worker(Worker&& worker) { workers_.add(std::move(worker)); }
    void remove_worker(ElementID id) { remove_receiver(workers_, id); }
    NodeCollection<Worker>::iterator       find_worker_by_id(ElementID id) { return workers_.find_by_id(id); }
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const{ return workers_.find_by_id(id); }
    NodeCollection<Worker>::const_iterator worker_cbegin() const {return workers_.cbegin(); }
    NodeCollection<Worker>::const_iterator worker_cend()   const {return workers_.cend(); }
    NodeCollection<Worker>::iterator worker_begin()  {return workers_.begin(); }
    NodeCollection<Worker>::iterator worker_end() {return workers_.end(); }

    void add_storehouse(Storehouse&& storehouse) { storehouses_.add(std::move(storehouse)); }
    void remove_storehouse(ElementID id) {remove_receiver(storehouses_, id); }
    NodeCollection<Storehouse>::iterator       find_storehouse_by_id(ElementID id) { return storehouses_.find_by_id(id); }
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const { return storehouses_.find_by_id(id); }
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

struct ParsedLineData{
    std::string element_type;
    std::map<std::string,std::string> parameters;
};


ParsedLineData parse_line(std::string line);

Factory load_factory_structure(std::istream& is);
void save_factory_structure(Factory& factory, std::ostream& os);

template<class T>
void Factory::remove_receiver(NodeCollection<T>& collection, ElementID id) {
    if constexpr (!std::is_same_v<T, Ramp>) {
        auto type = collection.find_by_id(id)->get_receiver_type();
        for (auto& el: ramps_) {
            auto pref = el.receiver_preferences_.get_preferences();
            for (auto& pref_r: pref) {
                if (pref_r.first->get_receiver_type() == type && pref_r.first->get_id() == id) {
                    el.receiver_preferences_.remove_receiver(pref_r.first);
                }
            }
        }
        for (auto& el: workers_) {
            auto pref = el.receiver_preferences_.get_preferences();
            for (auto& pref_w: pref) {
                if (pref_w.first->get_receiver_type() == type && pref_w.first->get_id() == id) {
                    el.receiver_preferences_.remove_receiver(pref_w.first);
                }
            }
        }
    }
    collection.remove_by_id(id);
}

template <class T>
void NodeCollection<T>::remove_by_id(ElementID id){
    iterator to_be_removed = std::find_if(node_.begin(),node_.end(), [&] (T & p) {return p.get_id() ==id;});
    node_.erase(to_be_removed);
}

template <class T>
typename std::vector<T>::iterator NodeCollection<T>::find_by_id(ElementID id) {
    iterator found = std::find_if(node_.begin(),node_.end(), [&] (T & p) {return p.get_id() ==id;});
    return found;
}

template <class T>
typename std::vector<T>::const_iterator NodeCollection<T>::find_by_id(ElementID id) const {
    const_iterator found = std::find_if(node_.begin(),node_.end(), [&] (T const& p) {return p.get_id() ==id;});
    return found;
}

#endif //NETSIM_FACTORY_HPP

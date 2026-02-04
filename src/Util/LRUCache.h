/* 
 * LRU»º´æ
 * File:   lrucache.hpp
 * Author: Alexander Ponomarev
 *
 * Created on June 20, 2013, 5:09 PM
 * revised by acedtang on 2021/09/12
 */

#ifndef _LRUCACHE_HPP_INCLUDED_
#define	_LRUCACHE_HPP_INCLUDED_

#include <unordered_map>
#include <list>
#include <cstddef>
#include <stdexcept>

namespace Sun{

class default_deleter {
public:
	template<typename _T>
	void operator()(_T& t) {

	}
};

template<typename key_t, typename value_t,typename deleter = default_deleter>
class TLRUCache {
public:
	typedef typename std::pair<key_t, value_t> key_value_pair_t;
	typedef typename std::list<key_value_pair_t>::iterator list_iterator_t;

	using iterator = list_iterator_t;

	TLRUCache(size_t max_size) :
		_max_size(max_size) {
	
	}

	~TLRUCache() {
		for (auto& it : _cache_items_list) {
			_deleter(it.second);
		}
	}
	
	void put(const key_t& key, const value_t& value) {
		auto it = _cache_items_map.find(key);
		_cache_items_list.push_front(key_value_pair_t(key, value));
		if (it != _cache_items_map.end()) {
			_cache_items_list.erase(it->second);
			_cache_items_map.erase(it);
		}
		_cache_items_map[key] = _cache_items_list.begin();
		
		if (_cache_items_map.size() > _max_size) {
			auto last = _cache_items_list.end();
			last--;
			_deleter(last->second);
			_cache_items_map.erase(last->first);
			_cache_items_list.pop_back();
		}
	}

	iterator find(const key_t& key) {
		auto it = _cache_items_map.find(key);
		if (it == _cache_items_map.end()) {
			return _cache_items_list.end();
		}
		else {
			return it->second;
		}
		return _cache_items_list.end();
	}
	
	const value_t& get(const key_t& key) {
		auto it = _cache_items_map.find(key);
		if (it == _cache_items_map.end()) {
			throw std::range_error("There is no such key in cache");
		} else {
			_cache_items_list.splice(_cache_items_list.begin(), _cache_items_list, it->second);
			return it->second->second;
		}
	}
	
	bool exists(const key_t& key) const {
		return _cache_items_map.find(key) != _cache_items_map.end();
	}
	
	size_t size() const {
		return _cache_items_map.size();
	}

	size_t maxSize() const {
		return _max_size;
	}

	iterator begin() {
		return _cache_items_list.begin();
	}

	iterator end() {
		return _cache_items_list.end();
	}
	
private:
	std::list<key_value_pair_t> _cache_items_list;
	std::unordered_map<key_t, list_iterator_t> _cache_items_map;
	size_t _max_size;
	deleter _deleter;
};

} // namespace Sun

#endif	/* _LRUCACHE_HPP_INCLUDED_ */
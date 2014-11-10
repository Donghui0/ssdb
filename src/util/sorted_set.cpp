/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include "sorted_set.h"

int SortedSet::size() const{
	return (int)sorted_set.size();
}

int SortedSet::add(const std::string &key, int64_t score){
	int ret;
	std::map<std::string, std::set<Item>::iterator>::iterator it;
	
	it = existed.find(key);
	if(it == existed.end()){
		// new item
		ret = 1;
	}else{
		ret = 0;
		std::set<Item>::iterator it2 = it->second;
		const Item &item = *it2;
		if(item.score == score){
			// not updated
			return 0;
		}
		// remove existing item
		sorted_set.erase(it2);
	}
	
	Item item;
	item.key = key;
	item.score = score;
	
	std::pair<std::set<Item>::iterator, bool> p = sorted_set.insert(item);
	existed[key] = p.first;
	
	return ret;
}

int SortedSet::del(const std::string &key){
	int ret;
	std::map<std::string, std::set<Item>::iterator>::iterator it;
	
	it = existed.find(key);
	if(it == existed.end()){
		// new item
		ret = 0;
	}else{
		ret = 1;
		sorted_set.erase(it->second);
		existed.erase(it);
	}
	return ret;
}

int SortedSet::front(const std::string **key, int64_t *score) const{
	std::set<Item>::iterator it2 = sorted_set.begin();
	if(it2 == sorted_set.end()){
		return 0;
	}
	const Item &item = *it2;
	*key = &item.key;
	if(score){
		*score = item.score;
	}
	return 1;
}

int SortedSet::front(std::string *key, int64_t *score) const{
	const std::string *ptr;
	if(front(&ptr, score)){
		// string copy
		*key = *ptr;
		return 1;
	}
	return 0;
}

int SortedSet::pop_front(){
	std::set<Item>::iterator it2 = sorted_set.begin();
	if(it2 == sorted_set.end()){
		return 0;
	}
	const Item &item = *it2;
	existed.erase(item.key);
	sorted_set.erase(it2);
	return 1;
}

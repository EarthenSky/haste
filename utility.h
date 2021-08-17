#pragma once

#include <unordered_map>
#include <utility>

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

typedef std::pair<int, int> int_pair;
typedef std::unordered_map<int_pair, bool, pair_hash> int_pair_to_bool_map;
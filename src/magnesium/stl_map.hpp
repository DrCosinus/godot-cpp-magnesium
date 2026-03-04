#pragma once

#include "godot_allocator.hpp"

#include <map>

template <typename Key, typename T, typename Compare = std::less<Key>, typename Allocator = GodotAllocator<std::pair<const Key, T>>>
using StlMap = std::map<Key, T, Compare, Allocator>;

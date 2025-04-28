#pragma once

// External includes
#define BETTER_ENUMS_STRICT_CONVERSION
#include <better-enums/enum.h>

#include <algorithm>
#include <cmath>
#include <chrono>
#include <filesystem>
#include <functional>
#include <fstream>
#include <map>
#include <memory.h>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <random>

namespace MD {

using Path = std::filesystem::path;
using DirectoryEntry = std::filesystem::directory_entry;
using FileStatus = std::filesystem::file_status;
using FileType = std::filesystem::file_type;


using String = std::string;

using SStream = std::stringstream;


using FStream = std::fstream;

using IFStream = std::ifstream;

using OFStream = std::ofstream;

using StreamSize = std::streamsize;


template<typename KEY,
         typename T,
         typename B = std::less<KEY>,
         typename A = std::allocator<std::pair<const KEY, T>>>
using Map = std::map<KEY, T, B, A>;

template<typename Key,
         typename Value,
         typename Hash = std::hash<Key>,
         typename KeyEqual = std::equal_to<Key>,
         typename Allocator = std::allocator<std::pair<const Key, Value>>>
using UnorderedMap = std::unordered_map<Key, Value, Hash, KeyEqual, Allocator>;

template<typename T, typename A = std::allocator<T>>
using Vector = std::vector<T, A>;

template<typename T>
using SPtr = std::shared_ptr<T>;

template<typename T>
using UPtr = std::unique_ptr<T>;

template<typename T>
using WPtr = std::weak_ptr<T>;


struct Utils {

  template<typename ... Args>
  static String format(const String& format, Args ... args) {

    const char* formatCstr = format.c_str();

    int32_t size_s = std::snprintf(nullptr, 0, formatCstr, std::forward<Args>(args)...) + 1; // Extra space for '\0'
    if (size_s <= 0) { throw::std::runtime_error("Error during formatting."); }
    auto size = static_cast<size_t>(size_s);
    auto buf = std::make_unique<char[]>(size);
    std::snprintf(buf.get(), size, formatCstr, std::forward<Args>(args)...);
    return String(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
  }

  template <typename T>
  static bool
  isNearSame(T a, T b, T threshold = 0.0000001f) {
    return std::abs(a - b) <= threshold;
  }

  static float
  getRandom(const float& maxValue, const float& minValue = 0) {
    return static_cast<float>((std::rand() % static_cast<int64_t>(maxValue)) + minValue);
  }
 
  static 
  String trim(const String& str, char trimBeg = ' ', char trimEnd = ' ') {
    size_t start = str.find_first_not_of(trimBeg);
    size_t end = str.find_last_not_of(trimEnd);
    return str.substr(start, end - start + 1);
  }

  static 
  Vector<String> split(const String& str, char delim) {
    Vector <String> tokens;
    size_t start = 0;
    size_t end = str.find(delim);
    while (end != String::npos) {
      tokens.push_back(trim(str.substr(start, end - start)));
      start = end + 1;
      end = str.find(delim, start);
    }
    tokens.push_back(str.substr(start, end));
    return tokens;
  }

};



}

#define SAFE_RELEASE(p) { if (p) { p->Release(); p = nullptr; } }

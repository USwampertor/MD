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
#include <memory.h>
#include <sstream>
#include <string>
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
  getRandom(const uint32_t& maxValue) {
    std::srand(std::time(nullptr));
    return std::rand() % maxValue;
  }

};




}

#define SAFE_RELEASE(p) { if (p) { p->Release(); p = nullptr; } }

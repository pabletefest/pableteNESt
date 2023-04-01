#pragma once

#include <memory>
#include <cstdint>

using u8 = std::uint8_t;
using s8 = std::int8_t;
using i8 = s8;
using u16 = std::uint16_t;
using s16 = std::int16_t;
using i16 = s16;
using u32 = std::uint32_t;
using s32 = std::int32_t;
using i32 = s32;
using u64 = std::uint64_t;
using s64 = std::int64_t;
using i64 = s64;

using vu8 = volatile std::uint8_t;
using vs8 = volatile std::int8_t;
using vi8 = vs8;
using vu16 = volatile std::uint16_t;
using vs16 = volatile std::int16_t;
using vi16 = vs16;
using vu32 = volatile std::uint32_t;
using vs32 = volatile std::int32_t;
using vi32 = vs32;
using vu64 = volatile std::uint64_t;
using vs64 = volatile std::int64_t;
using vi64 = vs64;

template<typename T> using Scope = std::unique_ptr<T>;
template<typename T> using Unique = std::unique_ptr<T>;
template<typename T> using Ref = std::shared_ptr<T>;
template<typename T> using Shared = std::shared_ptr<T>;

#define CONVERT_KB_TO_BYTES(KB) (KB * 1024)

#ifdef _WIN32
 #define INLINE __forceinline
#else
 #define INLINE inline
#endif

constexpr INLINE u64 convertKBToBytes(u32 KB) { return static_cast<u64>(KB) * 1024; }

template<typename T>
constexpr INLINE T convertKBToBytes(u32 KB) { return static_cast<T>(KB) * 1024; }
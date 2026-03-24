#pragma once

#include <cstdint>

namespace gg {

	using size = uint16_t;
	using ptr = uint16_t;

	struct MemoryRange { 
		ptr begin;
		ptr end;
		[[nodiscard]] constexpr size size() const { return end - begin; }
		[[nodiscard]] constexpr bool contains(ptr address) const { return address >= begin && address < end; }
		[[nodiscard]] constexpr bool overlaps(MemoryRange other) const { return other.begin < end && other.end > begin; }
	};

} // namespace gg

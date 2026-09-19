#pragma once

#include "rgl/core/types.h"

namespace rgl::internal{
	inline constexpr uint32_t page_metadata_magic = 0x52474C50;
	inline constexpr uint64_t rgl_magic = 0x52474C5F424947ULL;
	
	inline constexpr size_t pageSize = 65536;
	inline constexpr size_t chunkPages = 8;
	inline constexpr size_t chunkSize = pageSize * chunkPages;

	inline constexpr const uint16_t SizeList[] = {
	    0, 8, 16, 24, 32, 48, 64, 80, 96, 112, 128,
	    160, 184, 208, 256, 312, 368, 448, 512, 576,
	    704, 896, 1024, 1152, 1408, 1792, 2048, 2304,
	    2688, 3456, 4096, 4736, 6144, 8192
	};
	inline constexpr size_t NumClasses = sizeof(SizeList) / sizeof(SizeList[0]);
	inline constexpr size_t MaxAllocSize = SizeList[NumClasses - 1];

	template <size_t MaxSize>
	struct LutWrapper {
		uint8_t data[MaxSize + 1];
	};

	extern const LutWrapper<MaxAllocSize> SizeLutWrapper;
	extern const uint8_t* Lut;
	inline uint8_t get_class(size_t size){ return Lut[size]; }
}//namespace rgl::internal
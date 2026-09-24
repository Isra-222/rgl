#include "rgl/memory/regular/manager.h"
#include "rgl/memory/thread/stack_global.h"
#include "rgl/io/iostream.h"

namespace rgl{

	ostream out;
    ostream err(2);
    istream in;

	namespace internal{
		template <size_t MaxSize>
		constexpr LutWrapper<MaxSize> generate_size_lut() {
		   LutWrapper<MaxSize> wrapper{};
		    for (size_t bytes = 0; bytes <= MaxSize; ++bytes) {
		        size_t selected_class = 0;
		        for (size_t i = 0; i < NumClasses; ++i) {
		            if (SizeList[i] >= bytes) {
		                selected_class = i;
		                break;
		            }
		        }
		        wrapper.data[bytes] = static_cast<uint8_t>(selected_class);
		    }
		    return wrapper;
		}
		const LutWrapper<MaxAllocSize> SizeLutWrapper = generate_size_lut<MaxAllocSize>();
		const uint8_t* Lut = SizeLutWrapper.data;

		thread_global GLOBAL_CENTRAL_CACHE;
		stack_global GLOBAL_STACK_CACHE;
	}//namespace internal
}//namespace rgl


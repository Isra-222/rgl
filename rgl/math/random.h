//----------------------------------------------------------------------------------------
//
//	Xoshiro-cpp
//	Xoshiro PRNG wrapper library for C++17 / C++20
//
//	Copyright (C) 2020 Ryo Suzuki <reputeless@gmail.com>
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files(the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions :
//	
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//	
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.
//
//----------------------------------------------------------------------------------------
// =======================================================================================
/**
* Fair maps to intervals without division.
* Reference : http://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/
*
* (c) Daniel Lemire
* Apache License 2.0
* 
*/
// =======================================================================================



/*
 * Regular Library (rgl)
 * Copyright (C) 2026 [Isra-222]
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see
 * <https://www.gnu.org/licenses/>.
*/


# pragma once

#include "rgl/core/types.h"
#include "rgl/containers/array.h"
#include "rgl/core/limits.h"
#include "rgl/time/anemo.h"

# if __has_cpp_attribute(nodiscard) >= 201907L
#	define XOSHIROCPP_NODISCARD_CXX20 [[nodiscard]]
# else
#	define XOSHIROCPP_NODISCARD_CXX20
# endif

namespace rgl::XoshiroCpp {
	// A default seed value for the generators
	inline constexpr rgl::uint64_t DefaultSeed = 1234567890ULL;

	// Converts given uint32 value `i` into a 32-bit floating
	// point value in the range of [0.0f, 1.0f)
	template <class Uint32, rgl::enable_if_t<rgl::is_same_v<Uint32, rgl::uint32_t>>* = nullptr>
	[[nodiscard]]
	inline constexpr float FloatFromBits(Uint32 i) noexcept;

	// Converts given uint64 value `i` into a 64-bit floating
	// point value in the range of [0.0, 1.0)
	template <class Uint64, rgl::enable_if_t<rgl::is_same_v<Uint64, rgl::uint64_t>>* = nullptr>
	[[nodiscard]]
	inline constexpr double DoubleFromBits(Uint64 i) noexcept;

	// SplitMix64
	// Output: 64 bits
	// Period: 2^64
	// Footprint: 8 bytes
	// Original implementation: http://prng.di.unimi.it/splitmix64.c
	class SplitMix64
	{
	public:

		using state_type	= rgl::uint64_t;	
		using result_type	= rgl::uint64_t;
		
		XOSHIROCPP_NODISCARD_CXX20
		explicit constexpr SplitMix64(state_type state = DefaultSeed) noexcept;

		constexpr result_type operator()() noexcept;

		template <rgl::size_t N>
		[[nodiscard]]
		constexpr rgl::array<rgl::uint64_t, N> generateSeedSequence() noexcept;

		[[nodiscard]]
		static constexpr result_type min() noexcept;

		[[nodiscard]]
		static constexpr result_type max() noexcept;

		[[nodiscard]]
		constexpr state_type serialize() const noexcept;

		constexpr void deserialize(state_type state) noexcept;

		[[nodiscard]]
		friend bool operator ==(const SplitMix64& lhs, const SplitMix64& rhs) noexcept
		{
			return (lhs.m_state == rhs.m_state);
		}

		[[nodiscard]]
		friend bool operator !=(const SplitMix64& lhs, const SplitMix64& rhs) noexcept
		{
			return (lhs.m_state != rhs.m_state);
		}
	
	private:

		state_type m_state;
	};

	// xoshiro256**
	// Output: 64 bits
	// Period: 2^256 - 1
	// Footprint: 32 bytes
	// Original implementation: http://prng.di.unimi.it/xoshiro256starstar.c
	// Version: 1.0
	class Xoshiro256StarStar
	{
	public:

		using state_type	= rgl::array<rgl::uint64_t, 4>;
		using result_type	= rgl::uint64_t;

		XOSHIROCPP_NODISCARD_CXX20
		explicit constexpr Xoshiro256StarStar(rgl::uint64_t seed = DefaultSeed) noexcept;

		XOSHIROCPP_NODISCARD_CXX20
		explicit constexpr Xoshiro256StarStar(state_type state) noexcept;

		constexpr result_type operator()() noexcept;

		// This is the jump function for the generator. It is equivalent
		// to 2^128 calls to next(); it can be used to generate 2^128
		// non-overlapping subsequences for parallel computations.
		constexpr void jump() noexcept;

		// This is the long-jump function for the generator. It is equivalent to
		// 2^192 calls to next(); it can be used to generate 2^64 starting points,
		// from each of which jump() will generate 2^64 non-overlapping
		// subsequences for parallel distributed computations.
		constexpr void longJump() noexcept;

		[[nodiscard]]
		static constexpr result_type min() noexcept;

		[[nodiscard]]
		static constexpr result_type max() noexcept;

		[[nodiscard]]
		constexpr state_type serialize() const noexcept;

		constexpr void deserialize(state_type state) noexcept;

		[[nodiscard]]
		friend bool operator ==(const Xoshiro256StarStar& lhs, const Xoshiro256StarStar& rhs) noexcept
		{
			return (lhs.m_state == rhs.m_state);
		}

		[[nodiscard]]
		friend bool operator !=(const Xoshiro256StarStar& lhs, const Xoshiro256StarStar& rhs) noexcept
		{
			return (lhs.m_state != rhs.m_state);
		}

	private:

		state_type m_state;
	};

	
}

////////////////////////////////////////////////////////////////

namespace rgl::XoshiroCpp
{
	template <class Uint32, rgl::enable_if_t<rgl::is_same_v<Uint32, rgl::uint32_t>>*>
	inline constexpr float FloatFromBits(const Uint32 i) noexcept
	{
		return (i >> 8) * 0x1.0p-24f;
	}

	template <class Uint64, rgl::enable_if_t<rgl::is_same_v<Uint64, rgl::uint64_t>>*>
	inline constexpr double DoubleFromBits(const Uint64 i) noexcept
	{
		return (i >> 11) * 0x1.0p-53;
	}

	namespace detail
	{
		[[nodiscard]]
		static constexpr rgl::uint64_t RotL(const rgl::uint64_t x, const int s) noexcept
		{
			return (x << s) | (x >> (64 - s));
		}

		[[nodiscard]]
		static constexpr rgl::uint32_t RotL(const rgl::uint32_t x, const int s) noexcept
		{
			return (x << s) | (x >> (32 - s));
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	SplitMix64
	//
	inline constexpr SplitMix64::SplitMix64(const state_type state) noexcept
		: m_state(state) {}

	inline constexpr SplitMix64::result_type SplitMix64::operator()() noexcept
	{
		rgl::uint64_t z = (m_state += 0x9e3779b97f4a7c15);
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
		z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
		return z ^ (z >> 31);
	}

	template <rgl::size_t N>
	inline constexpr rgl::array<rgl::uint64_t, N> SplitMix64::generateSeedSequence() noexcept
	{
		rgl::array<rgl::uint64_t, N> seeds = {};

		for (auto& seed : seeds)
		{
			seed = operator()();
		}

		return seeds;
	}

	inline constexpr SplitMix64::result_type SplitMix64::min() noexcept
	{
		return rgl::numeric_limits<result_type>::lowest();
	}

	inline constexpr SplitMix64::result_type SplitMix64::max() noexcept
	{
		return rgl::numeric_limits<result_type>::max();
	}

	inline constexpr SplitMix64::state_type SplitMix64::serialize() const noexcept
	{
		return m_state;
	}

	inline constexpr void SplitMix64::deserialize(const state_type state) noexcept
	{
		m_state = state;
	}


	////////////////////////////////////////////////////////////////
	//
	//	xoshiro256**
	//
	inline constexpr Xoshiro256StarStar::Xoshiro256StarStar(const rgl::uint64_t seed) noexcept
		: m_state(SplitMix64{ seed }.generateSeedSequence<4>()) {}

	inline constexpr Xoshiro256StarStar::Xoshiro256StarStar(const state_type state) noexcept
		: m_state(state) {}

	inline constexpr Xoshiro256StarStar::result_type Xoshiro256StarStar::operator()() noexcept
	{
		const rgl::uint64_t result = detail::RotL(m_state[1] * 5, 7) * 9;
		const rgl::uint64_t t = m_state[1] << 17;
		m_state[2] ^= m_state[0];
		m_state[3] ^= m_state[1];
		m_state[1] ^= m_state[2];
		m_state[0] ^= m_state[3];
		m_state[2] ^= t;
		m_state[3] = detail::RotL(m_state[3], 45);
		return result;
	}

	inline constexpr void Xoshiro256StarStar::jump() noexcept
	{
		constexpr rgl::uint64_t JUMP[] = { 0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c };

		rgl::uint64_t s0 = 0;
		rgl::uint64_t s1 = 0;
		rgl::uint64_t s2 = 0;
		rgl::uint64_t s3 = 0;

		for (rgl::uint64_t jump : JUMP)
		{
			for (int b = 0; b < 64; ++b)
			{
				if (jump & UINT64_C(1) << b)
				{
					s0 ^= m_state[0];
					s1 ^= m_state[1];
					s2 ^= m_state[2];
					s3 ^= m_state[3];
				}
				operator()();
			}
		}

		m_state[0] = s0;
		m_state[1] = s1;
		m_state[2] = s2;
		m_state[3] = s3;
	}

	inline constexpr void Xoshiro256StarStar::longJump() noexcept
	{
		constexpr rgl::uint64_t LONG_JUMP[] = { 0x76e15d3efefdcbbf, 0xc5004e441c522fb3, 0x77710069854ee241, 0x39109bb02acbe635 };

		rgl::uint64_t s0 = 0;
		rgl::uint64_t s1 = 0;
		rgl::uint64_t s2 = 0;
		rgl::uint64_t s3 = 0;

		for (rgl::uint64_t jump : LONG_JUMP)
		{
			for (int b = 0; b < 64; ++b)
			{
				if (jump & UINT64_C(1) << b)
				{
					s0 ^= m_state[0];
					s1 ^= m_state[1];
					s2 ^= m_state[2];
					s3 ^= m_state[3];
				}
				operator()();
			}
		}

		m_state[0] = s0;
		m_state[1] = s1;
		m_state[2] = s2;
		m_state[3] = s3;
	}

	inline constexpr Xoshiro256StarStar::result_type Xoshiro256StarStar::min() noexcept
	{
		return rgl::numeric_limits<result_type>::lowest();
	}

	inline constexpr Xoshiro256StarStar::result_type Xoshiro256StarStar::max() noexcept
	{
		return rgl::numeric_limits<result_type>::max();
	}

	inline constexpr Xoshiro256StarStar::state_type Xoshiro256StarStar::serialize() const noexcept
	{
		return m_state;
	}

	inline constexpr void Xoshiro256StarStar::deserialize(const state_type state) noexcept
	{
		m_state = state;
	}

}
// distribution
namespace rgl::lamire {
	/**
	* Given a value "word", produces an integer in [0,p) without division.
	* The function is as fair as possible in the sense that if you iterate
	* through all possible values of "word", then you will generate all
	* possible outputs as uniformly as possible.
	*/
	static inline uint32_t fastrange32(uint32_t word, uint32_t p) {
		return (uint32_t)(((uint64_t)word * (uint64_t)p) >> 32);
	}

	#if defined(_MSC_VER) && defined (_WIN64)
		#include <intrin.h>// should be part of all recent Visual Studio
		#pragma intrinsic(_umul128)
	#endif // defined(_MSC_VER) && defined (_WIN64)


	/**
	* Given a value "word", produces an integer in [0,p) without division.
	* The function is as fair as possible in the sense that if you iterate
	* through all possible values of "word", then you will generate all
	* possible outputs as uniformly as possible.
	*/
	static inline uint64_t fastrange64(uint64_t word, uint64_t p) {
	#ifdef __SIZEOF_INT128__ // then we know we have a 128-bit int
		return (uint64_t)(((__uint128_t)word * (__uint128_t)p) >> 64);
	#elif defined(_MSC_VER) && defined(_WIN64)
		// supported in Visual Studio 2005 and better
		uint64_t highProduct;
		_umul128(word, p, &highProduct); // ignore low product
		return highProduct;
	#else
		return word % p; // fallback
	#endif // __SIZEOF_INT128__
	}


	#ifndef UINT32_MAX
		#define UINT32_MAX  (0xffffffff)
	#endif // UINT32_MAX

	/**
	* Given a value "word", produces an integer in [0,p) without division.
	* The function is as fair as possible in the sense that if you iterate
	* through all possible values of "word", then you will generate all
	* possible outputs as uniformly as possible.
	*/
	static inline size_t fastrangesize(size_t word, size_t p) {
	#if (SIZE_MAX == UINT32_MAX)
		return (size_t)fastrange32(word, p);
	#else // assume 64-bit
		return (size_t)fastrange64(word, p);
	#endif // SIZE_MAX == UINT32_MAX
	}

	/**
	* Given a value "word", produces an integer in [0,p) without division.
	* The function is as fair as possible in the sense that if you iterate
	* through all possible values of "word", then you will generate all
	* possible outputs as uniformly as possible.
	*/
	static inline int fastrangeint(int word, int p) {
	#if (SIZE_MAX == UINT32_MAX)
		return (int)fastrange32(word, p);
	#else // assume 64-bit
		return (int)fastrange64(word, p);
	#endif // (SIZE_MAX == UINT32_MAX)
	}

}

//----------
namespace rgl {

	template<typename T = int>
	class uniform_int_distribution{
		T min_val;
		T max_val;
	public:
		constexpr explicit uniform_int_distribution(T min, T max) : min_val(min), max_val(max) {}
		template<typename Engine>
		constexpr T operator()(Engine& eng){
			auto word = eng();

			uint64_t range = 0;
			if constexpr (is_signed_v<T>){
				if (min_val < 0 && max_val > 0) {
                    range = static_cast<uint64_t>(max_val) + static_cast<uint64_t>(-min_val) + 1;
                } else {
                    range = static_cast<uint64_t>(max_val - min_val) + 1;
                }
			}else{
				range = static_cast<uint64_t>(max_val - min_val) + 1;
			}
			uint64_t offset = 0;
			if(range <= 0xFFFFFFFFULL)
				offset = lamire::fastrange32(static_cast<uint32_t>(word), static_cast<uint32_t>(range));
			else
				offset = lamire::fastrange64(static_cast<uint64_t>(word), range);
			
			return static_cast<T>(static_cast<uint64_t>(min_val) + offset);
		}
		constexpr T min() const { return min_val; }
        constexpr T max() const { return max_val; }
	};

	template<typename T = float>
	class uniform_real_distribution{
		T min_val;
		T max_val;
	public:
		constexpr explicit uniform_real_distribution(T min, T max) : min_val(min), max_val(max) {}

		template<typename Engine>
		constexpr T operator()(Engine& eng){
			if constexpr(sizeof(T) <= 4){
				float f = XoshiroCpp::FloatFromBits(static_cast<uint32_t>(eng()));
				return min_val + static_cast<T>(f * (max_val - min_val));
			}else{
				double d = XoshiroCpp::DoubleFromBits(eng());
				return min_val + static_cast<T>(d * (max_val - min_val));
			}

		}

		constexpr T min() const { return min_val; }
        constexpr T max() const { return max_val; }
	};
	using uniform_float_distribution  = uniform_real_distribution<float>;
    using uniform_double_distribution = uniform_real_distribution<double>;

    inline uint64_t random_device(){
    	auto now = rgl::anemo::steady_clock::now();
    	uint64_t seed = static_cast<uint64_t>(now.time_since_epoch().count());
    	uint64_t stack_entropy = reinterpret_cast<uint64_t>(&seed);

    	seed ^= stack_entropy + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
    	return seed;
    }

    using randomEngine = XoshiroCpp::Xoshiro256StarStar;

    inline randomEngine& getGlobalEngine() {
        static randomEngine global_rng(random_device());
        return global_rng;
    }

    template <typename T>
    inline T random(T min, T max) {
		uniform_int_distribution<T> dist(min, max);
        return dist(getGlobalEngine());
    }

    inline float random(float min, float max) {
        uniform_float_distribution dist(min, max);
        return dist(getGlobalEngine());
    }
    inline double random(double min, double max){
    	uniform_double_distribution dist(min, max);
    	return dist(getGlobalEngine());
    }

    template <typename T, rgl::size_t N>
    void shuffle(T (&arr)[N], randomEngine& rng) {
        for (rgl::size_t i = N - 1; i > 0; --i) {
        	uniform_int_distribution<rgl::size_t> dist(0, i);
            rgl::size_t j = dist(rng);
            T temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    template <typename T, rgl::size_t N>
    void shuffle(T (&arr)[N]) {
        shuffle(arr, getGlobalEngine());
    }
    inline bool coin_flip() {
    	return (getGlobalEngine()() & 1ULL) != 0;
    }
    inline bool coin_flip(float probability) { //probability of true
    	if (probability <= 0.0f) return false;
        if (probability >= 1.0f) return true;

        uniform_float_distribution dist(0.0f, 1.0f);
        return dist(getGlobalEngine()) < probability;
    }

}
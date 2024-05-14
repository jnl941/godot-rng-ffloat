#pragma once
/*************************************************************************/
/*  random_pcg.h                                                         */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#ifndef RANDOM_H
#define RANDOM_H

#include <math.h>

#include "core/math/math_defs.h"

#include "core/typedefs.h"


#if defined(__GNUC__) || (_llvm_has_builtin(__builtin_clz))
#define CLZ32(x) __builtin_clz(x)
#elif defined(_MSC_VER)
#include "intrin.h"
static int __bsr_clz32(uint32_t x) {
	unsigned long index;
	_BitScanReverse(&index, x);
	return 31 - index;
}
#define CLZ32(x) __bsr_clz32(x)
#else
#endif

#if defined(__GNUC__) || (_llvm_has_builtin(__builtin_ldexp) && _llvm_has_builtin(__builtin_ldexpf))
#define LDEXP(s, e) __builtin_ldexp(s, e)
#define LDEXPF(s, e) __builtin_ldexpf(s, e)
#else
#include "math.h"
#define LDEXP(s, e) ldexp(s, e)
#define LDEXPF(s, e) ldexp(s, e)
#endif
#include <thirdparty/misc/pcg.h>
#include <core/os/memory.h>

class _NO_DISCARD_CLASS_ Random {
public:
	enum ALGORITHMS {
		PCG,
		XORSHIFT128,
		XOROSHIRO128,
		SPLITMIX64
	};
	static const uint64_t DEFAULT_SEED = 12047754176567800795U;
	static const uint64_t DEFAULT_INC = PCG_DEFAULT_INC_64;
	Random(uint64_t p_seed, uint64_t p_inc);
	Random();

protected:
	uint64_t current_seed; // The seed the current generator state started from.
	uint64_t current_inc;

public:

	_FORCE_INLINE_ void seed(uint64_t p_seed) {
		current_seed = p_seed;
	};
	_FORCE_INLINE_ uint64_t get_seed() {
		return current_seed;
	};
	_FORCE_INLINE_ void inc(uint64_t p_inc) {
		current_inc = p_inc;
	};
	_FORCE_INLINE_ uint64_t get_inc() {
		return current_inc;
	};
	_FORCE_INLINE_ virtual void set_state(uint64_t p_state) = 0;
	_FORCE_INLINE_ virtual uint64_t get_state() const = 0;

	void randomize();
	_FORCE_INLINE_ virtual uint32_t rand() = 0;

	_FORCE_INLINE_ virtual double randd() = 0;
	_FORCE_INLINE_ virtual float randf() = 0;

	_FORCE_INLINE_ double randfn(double p_mean, double p_deviation) {
		return p_mean + p_deviation * (cos(Math_TAU * randd()) * sqrt(-2.0 * log(randd())));
	};
	_FORCE_INLINE_ float randfn(float p_mean, float p_deviation) {
		return p_mean + p_deviation * (cos(Math_TAU * randf()) * sqrt(-2.0 * log(randf())));
	};
	double random(double p_from, double p_to);
	float random(float p_from, float p_to);
	real_t random(int p_from, int p_to) { return (real_t)random((real_t)p_from, (real_t)p_to); };
};
#endif

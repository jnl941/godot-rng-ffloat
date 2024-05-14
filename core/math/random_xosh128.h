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
#ifndef RANDOM_XOSH128_H
#define RANDOM_XOSH128_H

#include "core/math/random.h"
#include <thirdparty/randomization/xorshift/xorshift.hpp>

class RandomXOSH128 : public virtual Random {

	Xoroshiro128* xoroshiro;

public:
	RandomXOSH128(uint64_t p_seed = DEFAULT_SEED, uint64_t p_inc = DEFAULT_INC);

	_FORCE_INLINE_ virtual void seed(uint64_t p_seed){
		xoroshiro = memnew(Xoroshiro128(std::array<uint64_t, 2>{ p_seed, p_seed }));
		current_seed = p_seed;
	};
	_FORCE_INLINE_ virtual void set_state(uint64_t p_state) {
		seed(p_state);
	};
	_FORCE_INLINE_ virtual uint64_t get_state() const {
		return current_seed;
	};

	_FORCE_INLINE_ virtual uint32_t rand() {
		return static_cast<uint32_t>(xoroshiro->Next());
	};

	_FORCE_INLINE_ virtual double randd() {
		return static_cast<double>(xoroshiro->Next());
	};
	_FORCE_INLINE_ virtual float randf() {
		return static_cast<float>(xoroshiro->Next());
	};

};
#endif

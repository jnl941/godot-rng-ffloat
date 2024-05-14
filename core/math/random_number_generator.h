/*************************************************************************/
/*  random_number_generator.h                                            */
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

#ifndef RANDOM_NUMBER_GENERATOR_H
#define RANDOM_NUMBER_GENERATOR_H

#include "core/math/random_pcg.h"
#include "core/math/random.h"
#include "core/math/random_xsh128.h"
#include "core/math/random_xosh128.h"
#include "core/math/random_split64.h"
#include "core/reference.h"

class RandomNumberGenerator : public Reference {
	GDCLASS(RandomNumberGenerator, Reference);

private:

	RandomPCG *p_randbase_pcg = memnew(RandomPCG);
	RandomXOSH128* p_randbase_xosh128 = memnew(RandomXOSH128);
	RandomXSH128* p_randbase_xsh128 = memnew(RandomXSH128);
	RandomSPLIT64* p_randbase_split64 = memnew(RandomSPLIT64);
	uint8_t const algorithm_size = 4;

	union {
		uint8_t chosen_algorithm;
		uint8_t cycling_steps;
		uint8_t is_cycling;
	} cycling;

	Random &randbase = *p_randbase_pcg;

protected:

	static void _bind_methods();

public:
	enum ALGORITHMS {
		PCG,
		XORSHIFT128,
		XOROSHIRO128,
		SPLITMIX64
	};
	_FORCE_INLINE_ void set_seed(uint64_t seed) { randbase.seed(seed); }

	_FORCE_INLINE_ uint64_t get_seed() { return randbase.get_seed(); }

	_FORCE_INLINE_ void set_state(uint64_t p_state) { randbase.set_state(p_state); }

	_FORCE_INLINE_ uint64_t get_state() const { return randbase.get_state(); }

	_FORCE_INLINE_ void randomize() {
		//randbase.randomize();
		switch (get_chosen_algorithm()) {
			case PCG:
				p_randbase_pcg->randomize();
				break;
			case XORSHIFT128:
				p_randbase_xsh128->randomize();
				break;
			case XOROSHIRO128:
				p_randbase_xosh128->randomize();
				break;
			case SPLITMIX64:
				p_randbase_split64->randomize();
				break;
		}
		cycle();
	}

	_FORCE_INLINE_ uint32_t randi() {
		//real_t result = randbase.rand();
		real_t result;
		switch (get_chosen_algorithm()) {
			case PCG:
				result = p_randbase_pcg->rand();
				break;
			case XORSHIFT128:
				result = p_randbase_xsh128->rand();
				break;
			case XOROSHIRO128:
				result = p_randbase_xosh128->rand();
				break;
			case SPLITMIX64:
				result = p_randbase_split64->rand();
				break;
		}
		cycle();
		return result;
	}

	_FORCE_INLINE_ real_t randf() {
		//real_t result = randbase.randf();
		real_t result;
		switch (get_chosen_algorithm()) {
			case PCG:
				result = p_randbase_pcg->randf();
				break;
			case XORSHIFT128:
				result = p_randbase_xsh128->randf();
				break;
			case XOROSHIRO128:
				result = p_randbase_xosh128->randf();
				break;
			case SPLITMIX64:
				result = p_randbase_split64->randf();
				break;
		}
		cycle();
		return result;
	}

	_FORCE_INLINE_ real_t randf_range(real_t from, real_t to) {
		//real_t result = randbase.random(from, to);
		real_t result;
		switch (get_chosen_algorithm()) {
			case PCG:
				result = p_randbase_pcg->random(from,to);
				break;
			case XORSHIFT128:
				result = p_randbase_xsh128->random(from, to);
				break;
			case XOROSHIRO128:
				result = p_randbase_xosh128->random(from, to);
				break;
			case SPLITMIX64:
				result = p_randbase_split64->random(from, to);
				break;
		}
		cycle();
		return result;
	}

	_FORCE_INLINE_ real_t randfn(real_t mean = 0.0, real_t deviation = 1.0) {
		//real_t result = randbase.randfn(mean, deviation);
		real_t result;
		switch (get_chosen_algorithm()) {
			case PCG:
				result = p_randbase_pcg->randfn(mean, deviation);
				break;
			case XORSHIFT128:
				result = p_randbase_xsh128->randfn(mean, deviation);
				break;
			case XOROSHIRO128:
				result = p_randbase_xosh128->randfn(mean, deviation);
				break;
			case SPLITMIX64:
				result = p_randbase_split64->randfn(mean, deviation);
				break;
		}
		cycle();
		return result;
	}

	_FORCE_INLINE_ void cycle() {
		if (is_cycling()) {
			set_algo((get_chosen_algorithm() + get_cycling_steps()) % algorithm_size);
		}
	}
	_FORCE_INLINE_ bool is_cycling(){
		return (cycling.is_cycling & 0b10000000) == 0b10000000;
	};
	_FORCE_INLINE_ void set_cycling(bool is_cycling) {
		this->cycling.is_cycling &= 0b10000000;
		this->cycling.is_cycling |= is_cycling << 7;
	}
	_FORCE_INLINE_ void set_cycling_steps(uint8_t cycling_steps) {
		this->cycling.cycling_steps &= 0b10000011;
		this->cycling.cycling_steps |= 0b01111100 & (cycling_steps << 2);
	}
	_FORCE_INLINE_ uint8_t get_cycling_steps() {
		return (cycling.cycling_steps & 0b01111100) >> 2;
	}

	_FORCE_INLINE_ uint8_t get_chosen_algorithm() {
		return (cycling.cycling_steps & 0b00000011);
	}

	_FORCE_INLINE_ int randi_range(int from, int to) {
		unsigned int ret = randbase.rand();
		int result;
		
		if (to < from) {
			result = ret % (from - to + 1) + to;
		} else {
			result = ret % (to - from + 1) + from;
		}

		cycle();
		return result;
	};
	
void set_algo(uint8_t algo);
//void operator=(const RandomNumberGenerator p_rng);
RandomNumberGenerator();
RandomNumberGenerator(uint8_t algo);
RandomNumberGenerator(const RandomNumberGenerator &rng);
};

#endif // RANDOM_NUMBER_GENERATOR_H

#include "GarrysMod/Lua/Interface.h"
#include <iostream>
#include <cassert>
#include <chrono>
#include <random>
#include <thread>
#include <simdpp/simd.h>
using namespace simdpp;

static std::mt19937 rd(std::random_device{ }());
static unsigned short vecsize;

static void test_result() {
	const unsigned short arch = static_cast<unsigned short>(simdpp::this_compile_arch());
	switch (arch)
	{
		case 94:
		case 350:
		case 478:	vecsize = 8; break;
		case 30942:
		case 31198: vecsize = 16; break;
		default:	vecsize = 4; std::cout << "Unaccounted for arch: " << arch << '\n';
	}
	switch (vecsize)
	{
		case 4:	std::cout << "GSIMD: Attempting to use 128 bit SIMD." << '\n'; break;
		case 8:	std::cout << "GSIMD: Attempting to use 256 bit SIMD." << '\n'; break;
		case 16: std::cout << "GSIMD: Attempting to use 512 bit SIMD. Nice cpu you got there!" << '\n'; break;
	}
	std::cout << "GSIMD: Testing CPU support on your system... If you see an 'Illegal instruction' crash after this, you will need to download a less advanced version of this dll." << '\n';

	const unsigned short SIZE = 32768; //add this number^2 elements

	float* vec_a = new float[SIZE];
	float* vec_b = new float[SIZE];
	float* result = new float[SIZE]();

	for (unsigned short i = 0; i < SIZE; i++) {
		vec_a[i] = rd();
		vec_b[i] = rd();
	}

	auto start = std::chrono::high_resolution_clock::now();

	switch (vecsize)
	{
	case 4:
		for (unsigned short i = 0; i < SIZE; i += 4) {
			store(result + i, add((float32x4)load(vec_a + i), (float32x4)load(vec_b + i)));
		}
		break;
	case 8:
		for (unsigned short i = 0; i < SIZE; i += 8) {
			store(result + i, add((float32x8)load(vec_a + i), (float32x8)load(vec_b + i)));
		}
		break;
	case 16:
		for (unsigned short i = 0; i < SIZE; i += 16) {
			store(result + i, add((float32<16>)load(vec_a + i), (float32<16>)load(vec_b + i)));
		}
		break;
	}

	auto stop = std::chrono::high_resolution_clock::now();

	for (unsigned short i = 0; i < SIZE; i++) {
		assert(result[i] == vec_a[i] + vec_b[i]);
	}

	std::chrono::duration<float, std::micro> dur = stop - start;
	std::cout << "GSIMD: Successful! Took " << dur.count() << " microseconds to sum " << SIZE * SIZE << " elements using SIMD." << '\n';
}
std::thread t1(test_result);

LUA_FUNCTION( Add )
{
	if (LUA->IsType(-1,8)) {
		std::cout << "is table" << '\n';
	}
	return 0;
}
LUA_FUNCTION( Sub )
{
	if (LUA->IsType(-1, 8)) {
		std::cout << "is table" << '\n';
	}
	return 0;
}
LUA_FUNCTION( Mul )
{
	if (LUA->IsType(-1, 8)) {
		std::cout << "is table" << '\n';
	}
	return 0;
}
LUA_FUNCTION( Div )
{
	if (LUA->IsType(-1, 8)) {
		std::cout << "is table" << '\n';
	}
	return 0;
}
GMOD_MODULE_OPEN()
{
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
	LUA->CreateTable();
	LUA->PushNumber(0);
	LUA->SetField( -2, "version" );
	LUA->PushNumber(0);//replace_build_number_here_automatic!
	LUA->SetField( -2, "build" );
	LUA->PushCFunction(Add);
	LUA->SetField( -2, "Add" );
	LUA->PushCFunction(Sub);
	LUA->SetField( -2, "Sub" );
	LUA->PushCFunction(Mul);
	LUA->SetField( -2, "Mul" );
	LUA->PushCFunction(Div);
	LUA->SetField(-2, "Div");
	LUA->SetField( -2, "gsimd" );
	LUA->Pop(1);
	return 0;
}
GMOD_MODULE_CLOSE()
{
	return 0;
}
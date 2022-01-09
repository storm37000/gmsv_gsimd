#include "GarrysMod/Lua/Interface.h"
#include <iostream>
#include <cassert>
#include <chrono>
#include <random>
#include <simdpp/simd.h>
using namespace simdpp;

static std::mt19937 rd(std::random_device{ }());
static unsigned short vecsize = 4;

static void test_result() {
	const unsigned short SIZE = 32768; //add this number^2 elements of random numbers

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
	std::cout << "GSIMD: Successful! Took " << dur.count() << " microseconds to sum " << SIZE * SIZE << " elements using SIMD." << std::endl;
}

LUA_FUNCTION( Add )
{
	if (LUA->Top() < 1) { LUA->ArgError(1, "2 tables of equal size expected, got nil"); }
	if (LUA->Top() < 2) { LUA->ArgError(2, "2 tables of equal size expected, got nil"); }
	if (!LUA->IsType(1, GarrysMod::Lua::Type::Table)) { LUA->ArgError(1, "2 tables of equal size expected"); }
	if (!LUA->IsType(2, GarrysMod::Lua::Type::Table)) { LUA->ArgError(2, "2 tables of equal size expected"); }
	//LUA->GetTable(1);
	//LUA->GetTable(2);
	LUA->PushNil();
	return 1;
}
LUA_FUNCTION( Sub )
{
	if (LUA->Top() < 1) { LUA->ArgError(1, "2 tables of equal size expected, got nil"); }
	if (LUA->Top() < 2) { LUA->ArgError(2, "2 tables of equal size expected, got nil"); }
	if (!LUA->IsType(1, GarrysMod::Lua::Type::Table)) { LUA->ArgError(1, "2 tables of equal size expected"); }
	if (!LUA->IsType(2, GarrysMod::Lua::Type::Table)) { LUA->ArgError(2, "2 tables of equal size expected"); }
	LUA->PushNil();
	return 1;
}
LUA_FUNCTION( Mul )
{
	if (LUA->Top() < 1) { LUA->ArgError(1, "2 tables of equal size expected, got nil"); }
	if (LUA->Top() < 2) { LUA->ArgError(2, "2 tables of equal size expected, got nil"); }
	if (!LUA->IsType(1, GarrysMod::Lua::Type::Table)) { LUA->ArgError(1, "2 tables of equal size expected"); }
	if (!LUA->IsType(2, GarrysMod::Lua::Type::Table)) { LUA->ArgError(2, "2 tables of equal size expected"); }
	LUA->PushNil();
	return 1;
}
LUA_FUNCTION( Div )
{
	if (LUA->Top() < 1) { LUA->ArgError(1, "2 tables of equal size expected, got nil"); }
	if (LUA->Top() < 2) { LUA->ArgError(2, "2 tables of equal size expected, got nil"); }
	if (!LUA->IsType(1, GarrysMod::Lua::Type::Table)) { LUA->ArgError(1, "2 tables of equal size expected"); }
	if (!LUA->IsType(2, GarrysMod::Lua::Type::Table)) { LUA->ArgError(2, "2 tables of equal size expected"); }
	LUA->PushNil();
	return 1;
}
GMOD_MODULE_OPEN()
{
	std::cout << std::endl;
	const unsigned short arch = static_cast<unsigned short>(simdpp::this_compile_arch());
	switch (arch)
	{
		case 126:	vecsize = 4; break;
		case 510:	vecsize = 8; break;
		case 31230: vecsize = 16; break;
		default:	std::cout << "Unaccounted for arch: " << arch << std::endl;
	}
	switch (vecsize)
	{
		case 4:	std::cout << "GSIMD: Attempting to use 128 bit SIMD." << std::endl; break;
		case 8:	std::cout << "GSIMD: Attempting to use 256 bit SIMD." << std::endl; break;
		case 16: std::cout << "GSIMD: Attempting to use 512 bit SIMD. Nice cpu you got there!" << std::endl; break;
	}
	std::cout << "GSIMD: Testing CPU support on your system... If you see an 'Illegal instruction' crash after this, you will need to download a less advanced version of this dll." << std::endl;
	test_result();

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
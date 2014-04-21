#include <iostream>
#include <vector>
#include <memory>
#include <assert.h>
#include <string>
#include <array>

#include "particles.h"
#include "Tests.h"

void runTests(std::vector<std::unique_ptr<Test>> &tests, size_t count, size_t gens, size_t updates)
{
	for (auto &t : tests)
	{
		t->run(count, gens, updates);
	}
}

void printTests(const std::vector<std::unique_ptr<Test>> &tests)
{
	for (const auto &t : tests)
	{
		std::cout << std::endl << t->name() << " -----" << std::endl;
		std::cout << "memory:          " << t->getMemory() << " kb" << std::endl;
		std::cout << "creation time:   " << t->getCreationTime() << " milisec" << std::endl;
		std::cout << "generation time: " << t->getGenerationTime() << " milisec" << std::endl;
		std::cout << "updates time:    " << t->getUpdateTime() << " milisec" << std::endl;
	}

	std::cout << std::endl;
	for (const auto &t : tests)
		std::cout << t->getCreationTime() << ";";
	std::cout << std::endl;
	for (const auto &t : tests)
		std::cout << t->getGenerationTime() << ";";
	std::cout << std::endl;
	for (const auto &t : tests)
		std::cout << t->getUpdateTime() << ";";

	std::cout << std::endl;
}

int main(int argc, char *argv[])
{
	const size_t count = argc > 1 ? atoi(argv[1]) : 200000;
	const size_t gens = argc > 2 ? atoi(argv[2]) : 200;
	const size_t updates = argc > 3 ? atoi(argv[3]) : 2000;

	std::cout << "count: " << count << ", updates: " << updates << ", gens: " << gens << std::endl;
	std::cout << "sizeof(Particle): " << sizeof(Particle) << std::endl;

	std::vector<std::unique_ptr<Test>> tests;
	tests.emplace_back(new TestVectorOfObjects());
	tests.emplace_back(new TestVectorOfPointers());
	tests.emplace_back(new TestParticleData());

	runTests(tests, count, gens, updates);
	printTests(tests);

	return 0;
}

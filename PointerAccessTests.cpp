#include <iostream>
#include <vector>
#include <memory>
#include <assert.h>
#include <string>
#include <array>

#include "particles.h"
#include "Tests.h"

void runTests(std::vector<std::unique_ptr<Test>> &tests, size_t count, size_t updates)
{
	for (auto t = tests.begin(); t != tests.end(); ++t)
	{
		(*t)->run(count, updates);
	}
}

void printTests(const std::vector<std::unique_ptr<Test>> &tests)
{
	for (auto t = tests.begin(); t != tests.end(); ++t)
	{
		std::cout << std::endl << (*t)->name() << " -----" << std::endl;
		std::cout << "memory:          " << (*t)->getMemory() << " kb" << std::endl;
		std::cout << "creation time:   " << (*t)->getCreationTime() << " milisec" << std::endl;
		std::cout << "updates time:    " << (*t)->getUpdateTime() << " milisec" << std::endl;
	}

	std::cout << std::endl;
	for (auto t = tests.cbegin(); t != tests.cend(); ++t)
		std::cout << (*t)->getCreationTime() << ";";
	std::cout << std::endl;
	for (auto t = tests.cbegin(); t != tests.cend(); ++t)
		std::cout << (*t)->getUpdateTime() << ";";

	std::cout << std::endl;
}

int main(int argc, char *argv[])
{
	const size_t count = argc > 1 ? atoi(argv[1]) : 20000;
	const size_t updates = argc > 2 ? atoi(argv[2]) : 20000;

	std::cout << "count: " << count << ", updates: " << updates << std::endl;
	std::cout << "sizeof(Particle): " << sizeof(Particle) << std::endl;

	std::vector<std::unique_ptr<Test>> tests;
	tests.emplace_back(new TestVectorOfObjects());
	tests.emplace_back(new TestVectorOfPointers());
	tests.emplace_back(new TestParticleData());

	runTests(tests, count, updates);
	printTests(tests);

	return 0;
}

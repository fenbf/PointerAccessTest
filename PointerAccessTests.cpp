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

void printTests(const std::vector<std::unique_ptr<Test>> &tests, bool onlyShort, bool creation, bool updates)
{
	if (!onlyShort)
	{
		for (auto t = tests.begin(); t != tests.end(); ++t)
		{
			std::cout << std::endl << (*t)->name() << " -----" << std::endl;
			std::cout << "memory:          " << (*t)->getMemory() << " kb" << std::endl;
			std::cout << "creation time:   " << (*t)->getCreationTime() << " milisec" << std::endl;
			std::cout << "updates time:    " << (*t)->getUpdateTime() << " milisec" << std::endl;
		}
		std::cout << std::endl;
	}

	if (creation)
	{
		for (auto t = tests.cbegin(); t != tests.cend(); ++t)
			std::cout << (*t)->getCreationTime() << ";";
	}

	if (updates)
	{
		for (auto t = tests.cbegin(); t != tests.cend(); ++t)
			std::cout << (*t)->getUpdateTime() << ";";
	}

	std::cout << std::endl;
}

int main(int argc, char *argv[])
{
	bool showCreation = true;
	bool showUpdates = true;
	if (argc > 1)
	{
		std::string cmd = argv[1];
		showCreation = cmd.find('c') != std::string::npos;
		showUpdates = cmd.find('u') != std::string::npos;
	}

	//std::cout << "count: " << count << ", updates: " << updates << std::endl;
	//std::cout << "sizeof(Particle): " << sizeof(Particle) << std::endl;

	std::vector<std::unique_ptr<Test>> tests;
	tests.emplace_back(new TestVectorOfObjects());
	tests.emplace_back(new TestVectorOfPointers());
	//tests.emplace_back(new TestParticleData());

	const size_t countStart = 1000;
	const size_t countEnd = 51000;
	const size_t countSteps = 10;
	const size_t countDelta = (countEnd - countStart) / countSteps;

	const size_t updateStart = 1000;
	const size_t updateEnd = 21000;
	const size_t updateSteps = 10;
	const size_t updateDelta = (updateEnd - updateStart) / updateSteps;

	if (showCreation)
	{
		for (size_t count = countStart; count <= countEnd; count += countDelta)
		{
			std::cout << count << ";";
			runTests(tests, count, updateStart);
			printTests(tests, true, showCreation, showUpdates);
		}
	}

	if (showUpdates)
	{
		for (size_t updates = updateStart; updates <= updateEnd; updates += updateDelta)
		{
			std::cout << updates << ";";
			runTests(tests, countStart, updates);
			printTests(tests, true, showCreation, showUpdates);
		}
	}

	return 0;
}

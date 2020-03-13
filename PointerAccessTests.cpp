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

void printTests(const std::vector<std::unique_ptr<Test>> &tests, bool onlyShort, bool creation, bool updates, bool sorts)
{
	if (!onlyShort)
	{
		for (auto t = tests.begin(); t != tests.end(); ++t)
		{
			std::cout << std::endl << (*t)->name() << " -----" << std::endl;
			std::cout << "memory:          " << (*t)->getMemory() << " kb" << std::endl;
			std::cout << "creation time:   " << (*t)->getCreationTime() << " milisec" << std::endl;
			std::cout << "updates time:    " << (*t)->getUpdateTime() << " milisec" << std::endl;
			std::cout << "sort time:    " << (*t)->getSortTime() << " milisec" << std::endl;
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

	if (sorts)
	{
		for (auto t = tests.cbegin(); t != tests.cend(); ++t)
			std::cout << (*t)->getSortTime() << ";";
	}

	std::cout << std::endl;
}

int main(int argc, char *argv[])
{
	bool showCreation = true;
	bool showUpdates = true;
	bool showSorts = true;
	bool runCounts = true;
	bool runUpdates = true;
	bool onlyShortInfo = false;

	if (argc > 1)
	{
		std::string cmd = argv[1];
		if (cmd.size() >= 2)
		{
			showCreation = cmd[0] == 'c' || cmd[0] == 'x';
			showUpdates = cmd[0] == 'u' || cmd[0] == 'x';
			runCounts = cmd[1] == 'c' || cmd[1] == 'x';
			runUpdates = cmd[1] == 'u' || cmd[1] == 'x';
		}
		if (argc > 2)
		{
			cmd = argv[2];
			onlyShortInfo = cmd == "short";
		}
	}

	if (showCreation)
		std::cout << "will print creation time in miliseconds..." << std::endl;
	if (showUpdates)
		std::cout << "will print update time in miliseconds..." << std::endl;

	std::cout << "sizeof Particle: " << sizeof (Particle) << std::endl;

	std::vector<std::unique_ptr<Test>> tests;
	tests.emplace_back(new TestVectorOfObjects());
	tests.emplace_back(new TestVectorOfPointers());

	const size_t countStart = 1000;
	const size_t countEnd = 71000;
	const size_t countSteps = 20;
	const size_t countDelta = (countEnd - countStart) / countSteps;

	const size_t updateStart = 1000;
	const size_t updateEnd = 41000;
	const size_t updateSteps = 20;
	const size_t updateDelta = (updateEnd - updateStart) / updateSteps;

	if (runCounts)
	{
		std::cout << "elem count from " << countStart << " (fixed updates at " << updateStart << ")" << std::endl;
		std::cout << "count;";
		for (auto t = tests.begin(); t != tests.end(); ++t)
		{
			std::cout << (*t)->name() << ",";
			(*t)->resetTimes();
		}
		std::cout << std::endl;
		for (size_t count = countStart; count <= countEnd; count += countDelta)
		{
			std::cout << count << ";";
			runTests(tests, count, updateStart);
			printTests(tests, onlyShortInfo, showCreation, showUpdates, showSorts);
		}
	}

	if (runUpdates)
	{
		std::cout << "updates count from " << updateStart << " (fixed elem count at " << countStart << ")" << std::endl;	
		std::cout << "updates;";
		for (auto t = tests.begin(); t != tests.end(); ++t)
		{
			std::cout << (*t)->name() << ",";
			(*t)->resetTimes();
		}
		std::cout << std::endl;
		for (size_t updates = updateStart; updates <= updateEnd; updates += updateDelta)
		{
			std::cout << updates << ";";
			runTests(tests, countStart, updates);
			printTests(tests, onlyShortInfo, showCreation, showUpdates, showSorts);
		}
	}

	return 0;
}

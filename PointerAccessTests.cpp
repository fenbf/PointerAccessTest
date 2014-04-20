#include <iostream>
#include <vector>
#include <memory>
#include <Windows.h>
#include <assert.h>
#include <string>
#include <array>

#include "particles.h"

class PerfTimer
{
private:
	LARGE_INTEGER _freq;
	LARGE_INTEGER _startTime;
	LARGE_INTEGER _endTime;
public:
	explicit PerfTimer() 
	{
		QueryPerformanceFrequency(&_freq);		
	}

	void start()
	{
		QueryPerformanceCounter(&_startTime);
	}

	void stop(double *microseconds)
	{
		QueryPerformanceCounter(&_endTime);
		double micro;
		micro = static_cast<double>(_endTime.QuadPart - _startTime.QuadPart);
		micro *= 1000;
		micro /= static_cast<double>(_freq.QuadPart);
		if (microseconds)
			*microseconds += micro;
	}
};

class Test
{
protected:
	double _creationTime{ 0.0 };
	double _generationTime{ 0.0 };
	double _updatesTime{ 0.0 };
	double _memoryKb{ 0.0 };
public:
	virtual ~Test() { }

	virtual std::string name() const = 0;
	virtual void run(size_t count, size_t gens, size_t updates) = 0;

	double getCreationTime() const { return _creationTime; }
	double getGenerationTime() const { return _generationTime; }
	double getUpdateTime() const { return _updatesTime; }
	double getMemory() const { return _memoryKb; }
};

class TestVectorOfPointers : public Test
{
public:
	virtual std::string name() const { return "vector of shared_ptr"; }

	virtual void run(size_t count, size_t gens, size_t updates)
	{
		PerfTimer perf;

		perf.start();
		std::vector<std::shared_ptr<Particle>> particles(count);
		for (auto &p : particles)
		{
			p = std::make_shared<Particle>();
		}
		perf.stop(&_creationTime);

		_memoryKb = (particles.capacity()*sizeof(Particle))/1024.0;

		perf.start();
		for (size_t u = 0; u < gens; ++u)
		{
			for (auto &p : particles)
				p->generate();
		}
		perf.stop(&_generationTime);

		perf.start();
		for (size_t u = 0; u < updates; ++u)
		{
			for (auto &p : particles)
				p->update(1.0f);
		}
		perf.stop(&_updatesTime);
	}
};

class TestVectorOfObjects : public Test
{
public:
	virtual std::string name() const { return "vector of objects"; }

	virtual void run(size_t count, size_t gens, size_t updates)
	{
		PerfTimer perf;

		perf.start();
		std::vector<Particle> particles(count);
		perf.stop(&_creationTime);

		_memoryKb = (particles.capacity()*sizeof(Particle)) / 1024.0;

		perf.start();
		for (size_t u = 0; u < gens; ++u)
		{
			for (auto &p : particles)
				p.generate();
		}
		perf.stop(&_generationTime);

		perf.start();
		for (size_t u = 0; u < updates; ++u)
		{
			for (auto &p : particles)
				p.update(1.0f);
		}
		perf.stop(&_updatesTime);
	}
};

class TestParticleData : public Test
{
public:
	virtual std::string name() const { return "particle data"; }

	virtual void run(size_t count, size_t gens, size_t updates)
	{
		PerfTimer perf;

		perf.start();
		ParticleData particles(count);
		perf.stop(&_creationTime);
		
		_memoryKb = ParticleData::memoryUsed(&particles) / 1024.0;
		
		perf.start();
		for (size_t u = 0; u < gens; ++u)
			ParticleData::generate(&particles);
		perf.stop(&_generationTime);

		perf.start();
		for (size_t u = 0; u < updates; ++u)
			ParticleData::update(&particles, 1.0f);
		perf.stop(&_updatesTime);
	}
};

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

#include "Tests.h"
#include "PerfTimer.h"
#include "particles.h"
#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>

static const float DELTA_TIME = 1.0f / 60.0f;

constexpr size_t SharedPrtExtraSize = sizeof(int*) * 4; // one pointer for the object, one for the control blog, ref count, weak count, skipped deleter

namespace {
	template <typename T>
	void ShuffleVector(std::vector<T>& vec)	{
		static std::random_device rd;
		static std::mt19937 g(rd());

		std::shuffle(vec.begin(), vec.end(), g);
	}
}

void TestVectorOfPointers::run(size_t count, size_t updates)
{
	PerfTimer perf;

	perf.start();
	std::vector<std::shared_ptr<Particle>> particles(count);
	for (auto& p : particles)
		p = std::make_shared<Particle>();

	perf.stop(&_creationTime);

	ShuffleVector(particles);

	/*for (int i = 0; i < 10; ++i)
	{
		std::cout << (unsigned long)particles[i].get() << std::endl;
	}*/

	_memoryKb = (particles.capacity()*(sizeof(Particle) + SharedPrtExtraSize)) / 1024.0;

	for (auto& p : particles)
		p->generate();

	perf.start();
	for (size_t u = 0; u < updates; ++u)
	{
		for (auto& p : particles)
			p->update(DELTA_TIME);
	}
	perf.stop(&_updatesTime);

	perf.start();
	for (size_t u = 0; u < updates; ++u)
	{
		std::sort(std::begin(particles), std::end(particles),
			[](const std::shared_ptr<Particle>& a, const std::shared_ptr<Particle>& b) {
				return a->pos[0] < b->pos[0];
			}
		);
	}
	perf.stop(&_sortTime);
}

void TestVectorOfUniquePointers::run(size_t count, size_t updates)
{
	PerfTimer perf;

	perf.start();
	std::vector<std::unique_ptr<Particle>> particles(count);
	for (auto& p : particles)
		p = std::make_unique<Particle>();

	perf.stop(&_creationTime);

	ShuffleVector(particles);

	/*for (int i = 0; i < 10; ++i)
	{
		std::cout << (unsigned long)particles[i].get() << std::endl;
	}*/

	_memoryKb = (particles.capacity() * (sizeof(Particle)+sizeof(int*))) / 1024.0;

	for (auto& p : particles)
		p->generate();

	perf.start();
	for (size_t u = 0; u < updates; ++u)
	{
		for (auto& p : particles)
			p->update(DELTA_TIME);
	}
	perf.stop(&_updatesTime);

	perf.start();
	for (size_t u = 0; u < updates; ++u)
	{
		std::sort(std::begin(particles), std::end(particles),
			[](const std::unique_ptr<Particle>& a, const std::unique_ptr<Particle>& b) {
				return a->pos[0] < b->pos[0];
			}
		);
	}
	perf.stop(&_sortTime);
}

void TestVectorOfObjects::run(size_t count, size_t updates)
{
	PerfTimer perf;

	perf.start();
	std::vector<Particle> particles(count);
	perf.stop(&_creationTime);

	_memoryKb = (particles.capacity()*sizeof(Particle)) / 1024.0;

	for (auto& p : particles)
		p.generate();

	perf.start();
	for (size_t u = 0; u < updates; ++u)
	{
		for (auto& p : particles)
			p.update(DELTA_TIME);
	}
	perf.stop(&_updatesTime);

	perf.start();
	for (size_t u = 0; u < updates; ++u)
	{
		std::sort(std::begin(particles), std::end(particles),
			[](const Particle& a, const Particle& b) {
				return a.pos[0] < b.pos[0];
			}
		);
	}
	perf.stop(&_sortTime);
}

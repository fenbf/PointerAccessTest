#include "Tests.h"
#include "PerfTimer.h"
#include "particles.h"
#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>

static const float DELTA_TIME = 1.0f / 60.0f;

void TestVectorOfPointers::run(size_t count, size_t updates)
{
	PerfTimer perf;

	perf.start();
	std::vector<std::shared_ptr<Particle>> particles(count);
	for (auto p = particles.begin(); p != particles.end(); ++p)
	{
		*p = std::make_shared<Particle>();
	}

	perf.stop(&_creationTime);

	// randomize to simulate 
	for (size_t i = 0; i < count / 2; ++i)
	{
		int a = rand() % count;
		int b = rand() % count;
		if (a != b)
			std::swap(particles[a], particles[b]);
	}

	/*for (int i = 0; i < 10; ++i)
	{
		std::cout << (unsigned long)particles[i].get() << std::endl;
	}*/

	_memoryKb = (particles.capacity()*sizeof(Particle)) / 1024.0;

	for (auto p = particles.begin(); p != particles.end(); ++p)
		(*p)->generate();

	perf.start();
	for (size_t u = 0; u < updates; ++u)
	{
		for (auto p = particles.begin(); p != particles.end(); ++p)
			(*p)->update(DELTA_TIME);
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

void TestVectorOfObjects::run(size_t count, size_t updates)
{
	PerfTimer perf;

	perf.start();
	std::vector<Particle> particles(count);
	perf.stop(&_creationTime);

	// randomize: no sense in this case...
	/*for (size_t i = 0; i < count / 2; ++i)
	{
		int a = rand() % count;
		int b = rand() % count;
		std::swap(particles[a], particles[b]);
	}*/

	_memoryKb = (particles.capacity()*sizeof(Particle)) / 1024.0;

	for (auto p = particles.begin(); p != particles.end(); ++p)
		p->generate();

	perf.start();
	for (size_t u = 0; u < updates; ++u)
	{
		for (auto p = particles.begin(); p != particles.end(); ++p)
			p->update(DELTA_TIME);
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

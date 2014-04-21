#include "Tests.h"
#include "PerfTimer.h"
#include "particles.h"
#include <memory>
#include <vector>

void TestVectorOfPointers::run(size_t count, size_t gens, size_t updates)
{
	PerfTimer perf;

	perf.start();
	std::vector<std::shared_ptr<Particle>> particles(count);
	for (auto &p : particles)
	{
		p = std::make_shared<Particle>();
	}
	perf.stop(&_creationTime);

	_memoryKb = (particles.capacity()*sizeof(Particle)) / 1024.0;

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

void TestVectorOfObjects::run(size_t count, size_t gens, size_t updates)
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

void TestParticleData::run(size_t count, size_t gens, size_t updates)
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
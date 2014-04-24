#include "Tests.h"
#include "PerfTimer.h"
#include "particles.h"
#include <memory>
#include <vector>

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
}

void TestVectorOfObjects::run(size_t count, size_t updates)
{
	PerfTimer perf;

	perf.start();
	std::vector<Particle> particles(count);
	perf.stop(&_creationTime);

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
}

void TestParticleData::run(size_t count, size_t updates)
{
	PerfTimer perf;

	perf.start();
	ParticleData particles(count);
	perf.stop(&_creationTime);

	_memoryKb = ParticleData::memoryUsed(&particles) / 1024.0;

	ParticleData::generate(&particles);

	perf.start();
	for (size_t u = 0; u < updates; ++u)
		ParticleData::update(&particles, DELTA_TIME);
	perf.stop(&_updatesTime);
}
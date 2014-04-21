#pragma once

#include <string>

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

	virtual void run(size_t count, size_t gens, size_t updates);
};

class TestVectorOfObjects : public Test
{
public:
	virtual std::string name() const { return "vector of objects"; }

	virtual void run(size_t count, size_t gens, size_t updates);
};

class TestParticleData : public Test
{
public:
	virtual std::string name() const { return "particle data"; }

	virtual void run(size_t count, size_t gens, size_t updates);
};
#pragma once

#include <string>

class Test
{
protected:
	double _creationTime;
	double _updatesTime;
	double _memoryKb;
public:
	Test() : _creationTime(0.0), _updatesTime(0.0) { }
	virtual ~Test() { }

	virtual std::string name() const = 0;
	virtual void run(size_t count, size_t updates) = 0;

	void resetTimes() { _creationTime = _updatesTime = 0.0; }
	double getCreationTime() const { return _creationTime; }
	double getUpdateTime() const { return _updatesTime; }
	double getMemory() const { return _memoryKb; }
};

class TestVectorOfPointers : public Test
{
public:
	virtual std::string name() const override { return "vector of shared_ptr"; }

	virtual void run(size_t count, size_t updates) override;
};

class TestVectorOfObjects : public Test
{
public:
	virtual std::string name() const override { return "vector of objects"; }

	virtual void run(size_t count, size_t updates) override;
};

class TestParticleData : public Test
{
public:
	virtual std::string name() const override { return "particle data"; }

	virtual void run(size_t count, size_t updates) override;
};
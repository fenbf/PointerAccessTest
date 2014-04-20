#pragma once

#include <memory>

static int randSeed = 10;
inline float randF() { return 0.01f*(float)((randSeed++) % 100); }// (float)rand() / (float)RAND_MAX; }

class Particle
{
private:
	float pos[4];
	float acc[4];
	float vel[4];
	float col[4];
	float rot;
	float time;
public:

	virtual void generate()
	{
		acc[0] = randF();
		acc[1] = randF();
		acc[2] = randF();
		pos[0] = pos[1] = pos[2] = 0.0f;
		vel[0] = randF();
		vel[1] = randF();
		vel[2] = randF();
		vel[3] = vel[1] + vel[2];
		rot = 0.0f;
		time = 1.0f+randF();
	}

	virtual void update(float dt)
	{
		if (time > 0.0f)
		{
			vel[0] += acc[0] * dt;
			vel[1] += acc[1] * dt;
			vel[2] += acc[2] * dt;
			pos[0] += vel[0] * dt;
			pos[1] += vel[1] * dt;
			pos[2] += vel[2] * dt;
			col[0] = pos[0] * 0.001f;
			col[1] = pos[1] * 0.001f;
			col[2] = pos[2] * 0.001f;
			rot += vel[3] * dt;
			time -= dt;
		}
	}
};

class ParticleData
{
public:
	const size_t _count;
	std::unique_ptr<float[]> _data;
	float *pos;
	float *acc;
	float *vel;
	float *col;
	float *rot;
	float *time;
	/*std::unique_ptr<float[]> pos;
	std::unique_ptr<float[]> vel;
	std::unique_ptr<float[]> col;
	std::unique_ptr<float[]> rot;
	std::unique_ptr<float[]> time;*/

public:
	explicit ParticleData(size_t count) :
		_count(count),
		_data(new float[(4 + 4 + 4 + 4 + 1 + 1) * count])
	{
		pos = _data.get() + 0;
		acc = _data.get() + count * 4;
		vel = _data.get() + count * (4 + 4);
		col = _data.get() + count * (4 + 4 + 4);
		rot = _data.get() + count * (4 + 4 + 4 + 4);
		time = _data.get() + count * (4 + 4 + 4 + 4 +1);
	}

	static void generate(ParticleData *p)
	{
		const size_t count = p->_count;

		for (size_t i = 0; i < count; ++i)
		{
			p->acc[i * 4 + 0] = randF();
			p->acc[i * 4 + 1] = randF();
			p->acc[i * 4 + 2] = randF();
		}

		for (size_t i = 0; i < count; ++i)
			p->pos[i * 4 + 0] = p->pos[i * 4 + 1] = p->pos[i * 4 + 2] = 0.0f;		

		for (size_t i = 0; i < count; ++i)
		{
			p->vel[i * 4 + 0] = randF();
			p->vel[i * 4 + 1] = randF();
			p->vel[i * 4 + 2] = randF();
			p->vel[i * 4 + 3] = p->vel[i * 4 + 1] + p->vel[i * 4 + 2];
		}

		for (size_t i = 0; i < count; ++i)
			p->rot[i] = 0.0f;

		for (size_t i = 0; i < count; ++i)
			p->time[i] = 1.0f + randF();
	}

	static void update(ParticleData *p, float dt)
	{
		for (size_t i = 0; i < p->_count; ++i)
		{
			if (p->time[i] > 0.0f)
			{
				p->vel[i * 4 + 0] += p->acc[i * 4 + 0] * dt;
				p->vel[i * 4 + 1] += p->acc[i * 4 + 1] * dt;
				p->vel[i * 4 + 2] += p->acc[i * 4 + 2] * dt;
				p->pos[i * 4 + 0] += p->vel[i * 4 + 0] * dt;
				p->pos[i * 4 + 1] += p->vel[i * 4 + 1] * dt;
				p->pos[i * 4 + 2] += p->vel[i * 4 + 2] * dt;
				p->col[i * 4 + 0] = p->pos[i * 4 + 0] * 0.001f;
				p->col[i * 4 + 1] = p->pos[i * 4 + 1] * 0.001f;
				p->col[i * 4 + 2] = p->pos[i * 4 + 2] * 0.001f;
				p->rot[i] += p->vel[i * 4 + 3] * dt;
				p->time[i] -= dt;
			}
		}
	}

	static size_t memoryUsed(ParticleData *p)
	{
		return p->_count*sizeof(float)*(4 + 4 + 4 + 4 + 1 + 1);
	}
};
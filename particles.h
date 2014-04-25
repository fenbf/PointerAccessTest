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
		acc[3] = randF();
		pos[0] = pos[1] = pos[2] = pos[3] = 0.0f;
		vel[0] = randF();
		vel[1] = randF();
		vel[2] = randF();
		vel[3] = vel[1] + vel[2];
		rot = 0.0f;
		time = 2.0f+randF();
	}

	virtual void update(float dt)
	{
		vel[0] += acc[0] * dt;
		vel[1] += acc[1] * dt;
		vel[2] += acc[2] * dt;
		vel[3] += acc[3] * dt;
		pos[0] += vel[0] * dt;
		pos[1] += vel[1] * dt;
		pos[2] += vel[2] * dt;
		pos[3] += vel[3] * dt;
		col[0] = pos[0] * 0.001f;
		col[1] = pos[1] * 0.001f;
		col[2] = pos[2] * 0.001f;
		col[3] = pos[3] * 0.001f;
		rot += vel[3] * dt;
		time -= dt;

		if (time < 0.0f)
			generate();
	}
};

class ParticleData
{
public:
	const size_t _count;
	//std::unique_ptr<float[]> _data;
	//float *pos;
	//float *acc;
	//float *vel;
	//float *col;
	//float *rot;
	//float *time;
	std::unique_ptr<float[]> pos;
	std::unique_ptr<float[]> acc;
	std::unique_ptr<float[]> vel;
	std::unique_ptr<float[]> col;
	std::unique_ptr<float[]> rot;
	std::unique_ptr<float[]> time;

public:
	explicit ParticleData(size_t count) :
		_count(count),
		//_data(new float[(4 + 4 + 4 + 4 + 1 + 1) * count])
		pos(new float[count * 4]),
		acc(new float[count * 4]),
		vel(new float[count * 4]),
		col(new float[count * 4]),
		rot(new float[count]),
		time(new float[count])
	{
		/*pos = _data.get() + 0;
		acc = _data.get() + count * 4;
		vel = _data.get() + count * (4 + 4);
		col = _data.get() + count * (4 + 4 + 4);
		rot = _data.get() + count * (4 + 4 + 4 + 4);
		time = _data.get() + count * (4 + 4 + 4 + 4 +1);*/
	}

	static void generate(ParticleData *p, int id)
	{
		p->acc[id * 4 + 0] = randF();
		p->acc[id * 4 + 1] = randF();
		p->acc[id * 4 + 2] = randF();
		p->acc[id * 4 + 3] = randF();
		p->pos[id * 4 + 0] = p->pos[id * 4 + 1] = p->pos[id * 4 + 2] = p->pos[id * 4 + 2] = 0.0f;
		p->vel[id * 4 + 0] = randF();
		p->vel[id * 4 + 1] = randF();
		p->vel[id * 4 + 2] = randF();
		p->vel[id * 4 + 3] = p->vel[id * 4 + 1] + p->vel[id * 4 + 2];
		p->rot[id] = 0.0f;
		p->time[id] = 2.0f + randF();
	}

	static void generate(ParticleData *p)
	{
		const size_t count = p->_count;

		for (size_t i = 0; i < count; ++i)
		{
			p->acc[i * 4 + 0] = randF();
			p->acc[i * 4 + 1] = randF();
			p->acc[i * 4 + 2] = randF();
			p->acc[i * 4 + 3] = randF();
		}

		for (size_t i = 0; i < count; ++i)
			p->pos[i * 4 + 0] = p->pos[i * 4 + 1] = p->pos[i * 4 + 2] = p->pos[i * 4 + 3] = 0.0f;

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
			p->time[i] = 2.0f + randF();
	}

	static void update(ParticleData *p, float dt)
	{
		for (size_t i = 0; i < p->_count; ++i)
		{
			p->vel[i * 4 + 0] += p->acc[i * 4 + 0] * dt;
			p->vel[i * 4 + 1] += p->acc[i * 4 + 1] * dt;
			p->vel[i * 4 + 2] += p->acc[i * 4 + 2] * dt;
			p->vel[i * 4 + 3] += p->acc[i * 4 + 3] * dt;
			p->pos[i * 4 + 0] += p->vel[i * 4 + 0] * dt;
			p->pos[i * 4 + 1] += p->vel[i * 4 + 1] * dt;
			p->pos[i * 4 + 2] += p->vel[i * 4 + 2] * dt;
			p->pos[i * 4 + 3] += p->vel[i * 4 + 3] * dt;
		}

		for (size_t i = 0; i < p->_count; ++i)
		{
			p->col[i * 4 + 0] = p->pos[i * 4 + 0] * 0.001f;
			p->col[i * 4 + 1] = p->pos[i * 4 + 1] * 0.001f;
			p->col[i * 4 + 2] = p->pos[i * 4 + 2] * 0.001f;
			p->col[i * 4 + 3] = p->pos[i * 4 + 3] * 0.001f;
		}

		for (size_t i = 0; i < p->_count; ++i)
		{
			p->rot[i] += p->vel[i * 4 + 3] * dt;
			p->time[i] -= dt;
		}

		for (size_t i = 0; i < p->_count; ++i)
		{
			if (p->time[i] < 0.0f)
				generate(p, i);
		}
	}

	static size_t memoryUsed(ParticleData *p)
	{
		return p->_count*sizeof(float)*(4 + 4 + 4 + 4 + 1 + 1);
	}
};
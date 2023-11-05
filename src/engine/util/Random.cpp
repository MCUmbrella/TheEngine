//
// Created by mcumbrella on 23-10-27.
//

#include "Random.h"
#include "CommonUtil.h"

Random::Random()
{
    r = mt19937_64(CommonUtil::currentTimeNanos());
}

Random::Random(const uint64_t& seed)
{
    r = mt19937_64(seed);
}

int Random::nextInt()
{
    return r();
}

int Random::nextInt(const int& n)
{
    return r() % n;
}

long Random::nextLong()
{
    return r();
}

long Random::nextLong(const long& n)
{
    return r() % n;
}

double Random::nextDouble(const double& n)
{
    std::uniform_real_distribution<double> unif(0, n);
    return unif(r);
}

bool Random::nextBool()
{
    return r() & 1;
}

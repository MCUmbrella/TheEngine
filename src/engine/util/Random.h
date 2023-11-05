//
// Created by mcumbrella on 23-10-18.
//

#ifndef THEENGINE_RANDOM_H
#define THEENGINE_RANDOM_H

#include <random>

using std::mt19937_64;

class Random
{
private:
    mt19937_64 r;
public:
    Random();

    Random(const uint64_t& seed);

    /**
     * Return a random int value in [INT_MIN, INT_MAX]
     */
    int nextInt();

    /**
     * Return a random int value in [0, n).
     */
    int nextInt(const int& n);

    long nextLong();

    long nextLong(const long& n);

    /**
     * Return a random double value in [0, n).
     */
    double nextDouble(const double& n);

    /**
     * Return a random boolean value.
     */
    bool nextBool();
};

#endif //THEENGINE_RANDOM_H

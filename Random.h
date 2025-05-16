#pragma once
#include <random>
#include <chrono>

class CRandom {
public:
    CRandom()
        : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {}

    // Returns a random integer in [min, max]
    int GetInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }

    // Returns a random float in [min, max)
    float GetFloat(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(rng);
    }

    // Returns a random double in [min, max)
    double GetDouble(double min, double max) {
        std::uniform_real_distribution<double> dist(min, max);
        return dist(rng);
    }

private:
    std::mt19937 rng;
};

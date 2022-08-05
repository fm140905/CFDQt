/**
 * @file rng.h
 * @author Ming Fang
 * @brief Define random number generators
 * @version 0.1
 * @date 2022-08-04
 * 
 */
#pragma once


#include <random>

/**
 * @brief Global random number generator.
 * 
 */
class GlobalUniformRandNumGenerator 
{
private:
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution;
    GlobalUniformRandNumGenerator() = default;

    // Delete copy/move so extra instances can't be created/moved.
    GlobalUniformRandNumGenerator(const GlobalUniformRandNumGenerator&) = delete;
    GlobalUniformRandNumGenerator& operator=(const GlobalUniformRandNumGenerator&) = delete;
    GlobalUniformRandNumGenerator(GlobalUniformRandNumGenerator&&) = delete;
    GlobalUniformRandNumGenerator& operator=(GlobalUniformRandNumGenerator&&) = delete;
public:
    static GlobalUniformRandNumGenerator& GetInstance() {
        // Allocate with `new` in case Singleton is not trivially destructible.
        static GlobalUniformRandNumGenerator* rng = new GlobalUniformRandNumGenerator();
        return *rng;
    }

    /**
     * @brief Generate random real number between 0 and 1.
     * 
     * @return double 
     */
    double generateDouble() {
        return distribution(generator);
    }
};

/**
 * @brief Normal distribution random generator.
 * 
 */
class NormalRandNumGenerator
{
private:
    std::default_random_engine generator;
    std::normal_distribution<double> distribution;
public:
    /**
     * @brief Construct a new Normal Random Number Generator object
     * 
     * @param mean Mean of the normal distribution
     * @param stddev Standard deviation of the normal distribution
     */
    NormalRandNumGenerator(double mean, double stddev)
        : distribution(mean, stddev)
    {}
    /**
     * @brief Construct a new Normal Random Number Generator object
     * 
     * @param mean Mean of the normal distribution
     * @param stddev Standard deviation of the normal distribution
     * @param seed Seed used to initialize the random generator
     */
    NormalRandNumGenerator(double mean, double stddev, int seed)
        : generator(seed), distribution(mean, stddev) 
    {}

    /**
     * @brief Generate a random number
     * 
     * @return double 
     */
    double generateDouble() {return distribution(generator);}
};
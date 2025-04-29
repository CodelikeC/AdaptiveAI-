#include <iostream> 
using namespace std; 

#include <algorithm>
#include "utils/logger.h"
#include "Evolution.h"
#include <random> 
#include <chrono> 

namespace adaptive_ai 
{
    Evolution :: Evolution()
    {
        // Khoi tao the he dau tien .. // 
        current_generation = 0; 
        best_score = -1e9;
        initialize_population(); 
    }

    void Evolution :: initialize_population()
    {
        population.clear();
        for (int i = 0 ; i < POPULATION_SIZE ; ++i)
        {
            Candidate c; 
            c.learning_rate = random_double(0.0001, 0.01) ;
            c.policy_strength = random_double(0.5 , 2.0); 
            c.adaptation_threshold = random_double(0.1, 0.9); 
            population.push_back(c);
        }
        Logger :: info("Population intialized with", POPULATION_SIZE, "candidates");

    }

    void Evolution :: evaluate_population(const Metrics& metrics)
    {
        // Gia lap : fitness dua tren performence thuc te .. // 
        for (auto &candidate : population)
        {
            candidate.fitness = evaluate_candidate(candidate, metrics);
        }
    }

    double Evolution :: evaluate_candidate(const Candidate &c, const Metrics &metrics)
    {
        return metrics.detection_rate *2.0 - metrics.false_positive_rate *1.5 - c.learning_rate *0.1;
        // Mot ham fitness vi du : *ban co the cai tien theo thoi gian
    }

    void Evolution :: evolve()
    {
        ++current_generation; 
        Logger :: info("Evolving to generation", current_generation); 

        // chon nhung ca the tot nhat .. // 
        sort (population.begin(), population.end() , [](const Candidate &a, const Candidate &b)
        {
            return a.fitness > b.fitness;
        });

        // Cap nhat best score.. // 
        if (population.front().fitness > best_score)
        {
            best_score = population.front().fitness; 
            best_candidate = population.front(); 
            Logger :: info("new best fitness", best_score); 
        }
        // sinh the he moi tu nhung ca the tot nhat .. // 
        vector<Candidate> new_population; 

        // Giu lai top 10% elite .. / /
        int elite_count = POPULATION_SIZE/ 10; 
        for (int i = 0; i < elite_count ; ++i)
        {
            new_population.push_back(population[i]);
        }

        // sinh moi 90 % con lai bang cach mutate . .// 
        while(new_population.size() < POPULATION_SIZE)
        {
            Candidate parent = population[rand() % elite_count]; 
            Candidate offspring = mutate(parent); 
            new_population.push_back(offspring); 
        }

        population = new_population ;
    }

    Candidate Evolution :: mutate(const Candidate &parent)
    {
        Candidate child = parent;

        // Tao ngau nhien su thay doi nho ..// 
        child.learning_rate += random_double(-0.0005, 0.0005); 
        child.policy_strength += random_double(-0.1, 0.1); 
        child.adaptation_threshold += random_double(-0.05, 0.05);

        // Gioi han lai trong pham vi hop le .. // 
        child.learning_rate = clamp(child.learning_rate, 0.0001 ,0.01);
        child.policy_strength = std::clamp(child.policy_strength, 0.5, 2.0);
        child.adaptation_threshold = std::clamp(child.adaptation_threshold, 0.1, 0.9);

        return child;
    }

    double Evolution :: random_double (double min, double max)
    {
        static mt19937 rng(static_cast<unsigned int> (chrono::steady_clock::now().time_since_epoch().count()));
        uniform_real_distribution <double> dist(min, max); 
        return dist(rng);
    }

    const Candidate &Evolution :: get_best_candidate() const 
    {
        return best_candidate; 
    }
}; 

adaptive_ai :: Evolution evo; 


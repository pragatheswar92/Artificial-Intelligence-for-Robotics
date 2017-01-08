/*
 * Rubanraj Ravichandran
 * Pragatheswar Nagarajan
 * Aritificial Intelligence for Robotics
 * WS 2016
 * Assignment 8
 *
 * agent.hpp
 * */
#ifndef Agent_H_
#define Agent_H_

#include <vector>
#include <fstream>
#include <environment.hpp>

using namespace std;

class Agent {
    private:
        vector<City> cities;
        
        vector<City> findRoute(vector<City> cities,double temperature);
        void simulated_annealing_hill_climb();
        
        

    public:
        Agent(vector<City> Cities);
        ~Agent();
        void run();
        void print_cities(vector<City>&);
        void save_cities_image(vector<City>& cities, int file_counter);
        double get_total_distance(vector<City> cities);
        vector<City> shuffle_cities(vector<City> cities);
        vector<City> generate_neighbour(vector<City> cities);
        bool acceptRoute(double currentDistance,double neighbourDistance,double temperature);
        vector<int> permute();
};

#endif // Agent_H_

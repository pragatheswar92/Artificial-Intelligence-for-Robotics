/*
 * Rubanraj Ravichandran
 * Pragatheswar Nagarajan
 * Aritificial Intelligence for Robotics
 * WS 2016
 * Assignment 7
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
        
        vector<City> hillClimb(vector<City> cities);
        void random_restart_hill_climb();
        
        

    public:
        Agent(vector<City> Cities);
        ~Agent();
        void run();
        void print_cities(vector<City>&);
        void save_cities_image(vector<City>& cities, int file_counter);
        double get_total_distance(vector<City> cities);
        vector<City> shuffle_cities(vector<City> cities);
        vector<City> flip_cities(vector<City> cities);
};

#endif // Agent_H_

/*
 * Rubanraj Ravichandran
 * Pragatheswar Nagarajan
 * Aritificial Intelligence for Robotics
 * WS 2016
 * Assignment 8
 *
 * agent.cpp
 * */
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <agent.hpp>
#include <matplotlibcpp.h>

#include <chrono>
#include <thread>
#include <time.h>

#define rate_of_cooling 0.00005555
#define initial_temperature 999999999999999
#define minimum_temperature 0.001

Agent::Agent(vector<City> cities):cities(cities)
{
	
    
}

Agent::~Agent()
{

}

void Agent::run()
{
	std::srand ( unsigned (std::time(0)));
    simulated_annealing_hill_climb();
}

void Agent::print_cities(vector<City>& cities) {
    
    std::vector<double> x, y;
    for (int i = 0; i < cities.size(); i++) {
         x.push_back(cities[i].get_x());
         y.push_back(cities[i].get_y());
    }
    
    // Plot line from given x and y data. 
    matplotlibcpp::plot(x, y);
    matplotlibcpp::show();
}

void Agent::save_cities_image(vector<City>& cities, int file_counter) {

    std::vector<double> x, y;
    for (int i = 0; i < cities.size(); i++) {
         x.push_back(cities[i].get_x());
         y.push_back(cities[i].get_y());
    }
    
    // Plot line from given x and y data. 
    matplotlibcpp::plot(x, y);
    string file = "salesman_" + to_string(file_counter) + ".png";
    matplotlibcpp::save(file);
}


double Agent::get_total_distance(vector<City> cities) {
	double total_distance = 0;

	//calculating the total distance by summing up all the distances from initial city to final city
	for(int i=0; i < cities.size();i++){
		if(i < cities.size()-1){
			total_distance += cities[i].measureDistance(cities[i+1]);
		}
	}

	return total_distance + cities[cities.size()-1].measureDistance(cities[0]) ;
}

//this function is used to generate neighbour
vector<City> Agent::generate_neighbour(vector<City> cities){
	
	vector<City> result;
	int range = rand() % 300 + 1;
	for(int i = (cities.size() - range);i<cities.size();i++){
		result.push_back(cities[i]);
	}

	for(int i = range;i<(cities.size() -range);i++){
		result.push_back(cities[i]);
	}

	for(int i = 0;i<range;i++){
		result.push_back(cities[i]);
	}
	return result;

}


//this function is used to find the neighbour path and check its total distance is greater or smaller with current cities.

vector<City> Agent::findRoute(vector<City> cities,double temperature) {

	vector<City> neighbour;
	double elapsed_secs = 0.0;
	time_t start1;
	time_t end1;
	time (&start1);
	//using this "minutes" variable we can set the time limit
	int minutes = 10;
	do{
		neighbour = generate_neighbour(cities);
		if(acceptRoute(get_total_distance(cities),get_total_distance(neighbour),temperature)){
			cities = neighbour;
		}
		temperature *= 1-rate_of_cooling;
		time (&end1);
		elapsed_secs = difftime (end1,start1);
	}while(temperature > minimum_temperature && elapsed_secs < minutes*60);
	return cities;
}

//this acceptRoute function returns true if the neighbour cities are valid, else it returns false
bool Agent::acceptRoute(double currentDistance,double neighbourDistance,double temperature) {
	bool acceptance_flag = false;
	double acceptance_probability = 1.0;

	if(neighbourDistance >= currentDistance){
		acceptance_probability = exp(-(neighbourDistance - currentDistance)/temperature);
	}

	double random_number = ((double)rand() / RAND_MAX);
	if(acceptance_probability >= random_number){
		acceptance_flag = true;
	}
	return acceptance_flag;
}


void Agent::simulated_annealing_hill_climb() {

	vector<City> hill_climb;
	vector<City> shortest_path;
	
	double shortest_distance = get_total_distance(cities);
	std::srand ( unsigned (std::time(0)));
	
	hill_climb = findRoute(cities,initial_temperature);
	shortest_path = hill_climb;

	cout << "Shortest distance : " << get_total_distance(shortest_path) << " KM" <<endl;
	print_cities(shortest_path);
	save_cities_image(shortest_path,5);  
}

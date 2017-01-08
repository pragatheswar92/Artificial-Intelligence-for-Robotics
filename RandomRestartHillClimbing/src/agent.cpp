/*
 * Rubanraj Ravichandran
 * Pragatheswar Nagarajan
 * Aritificial Intelligence for Robotics
 * WS 2016
 * Assignment 7
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

Agent::Agent(vector<City> cities):cities(cities)
{
	
    
}

Agent::~Agent()
{

}

void Agent::run()
{
    random_restart_hill_climb();
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

vector<City> Agent::shuffle_cities(vector<City> cities){
//suffling the cities for random restart
	std::srand ( unsigned (std::time(0)));
	random_shuffle(cities.begin(), cities.end());
	return cities;

}

vector<City> Agent::flip_cities(vector<City> cities){

	int first = 0;
	int second = 0;
	while(first == second){
		first = rand() % (cities.size() - 1);
		second = rand() % (cities.size() - 1);
	}
	swap(cities[first], cities[second]);
	return cities;
}

vector<City> Agent::hillClimb(vector<City> cities) {
	int total_no_of_iterations = 500000;
	int counter = 0;
	vector<City> neighbour;
	while(counter < total_no_of_iterations){
		neighbour = flip_cities(cities);
		if(get_total_distance(neighbour) <= get_total_distance(cities)){
			cities = neighbour;
			counter = 0;
		}else{
			counter++;
		}
	}
	return cities;
}

void Agent::random_restart_hill_climb() {

	vector<City> shortest_path;
	vector<City> hill_climb;

	double shortest_distance = get_total_distance(cities);

	//restarting hill climb algorithm
	for(int i=0; i<5; i++){
		hill_climb = hillClimb(shuffle_cities(cities));
		if(get_total_distance(hill_climb) < shortest_distance){
			shortest_distance =get_total_distance(hill_climb);
			shortest_path = hill_climb;
		}

		cout << "Shortest distance for iteration " << i+1 << ": "<< get_total_distance(hill_climb) << " KM" <<endl;
	}

	print_cities(shortest_path);
	cout << "Final shortest distance: " << shortest_distance <<endl;
	save_cities_image(shortest_path, 1);
        
}

/*
 * Rubanraj Ravichandran
 * Pragatheswar Nagarajan
 * Aritificial Intelligence for Robotics
 * WS 2016
 * Assignment 7
 *
 * environment.cpp
 * */

#include <environment.hpp>
#include <string>
#include <fstream> 
#include <iostream>
#include <agent.hpp>

#include <cmath>
#define pi 3.14159265358979323846
#define earthRadiusKm 6371.0

City::City (string name, float x, float y):name(name), x(x), y(y) {
  
}


City::~City()
{

}

string City::get_name() {
    return name;
}

void City::set_name(string name_) {
    name = name_;
}

double City::get_x() {
    return x;
}

void City::set_x(float x_) {
    x = x_;
}

double City::get_y() {
    return y;
}

void City::set_y(float y_) {
    y = y_;
}

//this function is used to measure the distance between two cities based on given longitude and latitude of corresponding cities
double City::measureDistance(City city_){
	double a;
	double change_in_longitude = (city_.get_x() * (pi/180)) - (get_x() * (pi/180));
	double change_in_latitude = (city_.get_y() * (pi/180)) - (get_y() * (pi/180));
	a = pow((sin(change_in_latitude/2)),2) + cos(get_y()*(pi/180)) * cos(city_.get_y()*(pi/180)) * pow((sin(change_in_longitude/2)),2);
	return 2.0 * earthRadiusKm * atan2(sqrt(abs(a)),sqrt(abs(1-a)));
}



void Environment::run()
{
	
	Agent anAgent(read_file());
	anAgent.run();
}

std::vector<City> Environment::read_file()
{
	//ifstream file("./input/cities_full.txt");
	ifstream file("./input/50_cities.txt"); //test file
	string line;
    string delimiter(",");
    vector<string> token;
    vector<City> cities;
    string cityName;
    float x, y;
    int number = -1;

    while (getline(file, line)) {
        number++;
        size_t pos = line.find(delimiter);
        do {
            token.push_back(line.substr(0, pos));
            line.erase(0, pos + delimiter.length());
            pos = line.find(delimiter);

        } while (pos != string::npos);

        token.push_back(line);

        if (number != 0) { //First line of the file is a comment
            cityName = token[0];
            x = atof(token[1].c_str());
            y = atof(token[2].c_str());
            City newCity(cityName, x, y);
            cities.push_back(newCity);        
        }
        token.clear();
    }
    file.close();
    return cities;
}

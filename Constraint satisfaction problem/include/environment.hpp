/*
 * Rubanraj Ravichandran
 * Pragatheswar Nagarajan
 * Aritificial Intelligence for Robotics
 * WS 2016
 * Assignment 9
 *
 * environment.cpp
 * */

#ifndef ENV_H_
#define ENV_H_

#include <string>
#include <vector>


using namespace std;

class Location {
    private:
        float x;
        float y;
        float deadline;

    public:
        Location(float x, float y);
        Location(float x, float y, float deadline);
        ~Location();

        float get_x();
        void set_x(float coord);
        float get_y();
        void set_y(float coord);
        float get_deadline();
};

class Environment {
public:
	Environment()
	{
		
	}
	~Environment() {
	}
	void run();
	vector<Location> read_file(string filename);

private:
	vector<Location> cities;

};
#endif /* ENV_H_ */

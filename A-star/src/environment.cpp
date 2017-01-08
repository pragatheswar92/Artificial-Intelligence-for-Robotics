/*
 * Rubanraj Ravichandran
 * Pragatheswar Nagarajan
 * Aritificial Intelligence for Robotics
 * WS 2016
 * Assignment 6
 *
 * env.cpp
 * */

#include "environment.hpp"
#include "agent.hpp"

#include <iostream>
#include <stdlib.h>
#include <map>
#include <time.h>

Environment::Environment() :
		puzzle(8, std::vector<int>(8, 0)),goal(8, std::vector<int>(8, 0)) {

}



void Environment::run() {


	string ans = "1";
	string solver = "1";
	string heuristic = "1";
	Solver theSolver;
	Heuristic theHeuristic;

	while(ans == "1")
	{
		cout << "Select algorithm: " << endl;
		cout << "(1)Greedy (2)A*" << endl;
		cin >> solver;
		
		if(solver =="1")
			theSolver = Solver::GREEDY;
		else
			theSolver = Solver::A_STAR;
		
		cout << "Select heuristic: " << endl;
		cout << "(1) Misplaced tiles (2) Manhattan distance" << endl;
		cin >> heuristic;
		
		if(heuristic == "1")
			theHeuristic = Heuristic::MISPLACED_TILES;
		else
			theHeuristic = Heuristic::MANHATTAN_DISTANCE;
		
		
		generate_puzzle();
		print_puzzle();
		Agent anAgent(puzzle,goal, theSolver, theHeuristic);
		anAgent.run();

		cout << "New search ?: " << endl;
		cout << "(1) Yes (2) No " << endl;
		cin >> ans;
	}
	

}

void Environment::generate_puzzle() {
	std::map<int, int> numbers;
	int rand_number;
	bool inserted = false;
	srand((unsigned) time(NULL));

	int array[] = { 8,7,6,2,0,3,5,1,4};
	int goal_array[] = { 8,7,6,5,4,3,2,1,0 };

	int index = 0;
	int index_1 = 0;
	for (int rows = 0; rows < NUM_ROWS; rows++) {
		for (int cols = 0; cols < NUM_COLS; cols++) {
			inserted = false;
			puzzle[rows][cols] = array[index++];
			goal[rows][cols] = goal_array[index_1++];
		}
	}
	print_puzzle();

}

void Environment::print_puzzle() {
	for (int rows = 0; rows < NUM_ROWS; rows++) {
		for (int cols = 0; cols < NUM_COLS; cols++) {
			std::cout << "|" << puzzle[rows][cols];
		}
		std::cout << "|" << std::endl;
	}
}


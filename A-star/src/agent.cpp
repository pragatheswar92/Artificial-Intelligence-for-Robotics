/*
 * Rubanraj Ravichandran
 * Pragatheswar Nagarajan
 * Aritificial Intelligence for Robotics
 * WS 2016
 * Assignment 6
 *
 * agent.cpp
 * */
#include "agent.hpp"
#include <iostream>
#include <queue>
#include <algorithm>
#include <vector>
#include <map>


#include <chrono>
#include <ctime>
#include <ratio>


using namespace std;


int visited_nodes =0;
int path_cost = 0;


Agent::Agent(Puzzle puzzle, Goal goal, Solver solver, Heuristic heuristic) :
		puzzle(puzzle), goal(goal), solver(solver), heuristic(heuristic) {

}

Agent::~Agent() {

}

multimap<int,vector<vector<int>>> open;
multimap<int,vector<vector<int>>> close;

multimap<int,pair<vector<vector<int>>,int>> open_A_star;

int dx[]={-1,+1,0,0};
int dy[]={0,0,-1,+1};


void Agent::run() {

	if (solver == Solver::GREEDY) {
		cout << "Solver: Greedy Search" << endl;
		if (heuristic == Heuristic::MISPLACED_TILES) {
			cout << "Heuristic: Misplaced tiles" << endl;
		}

		else {
			cout << "Heuristic: Manhattan distance" << endl;
		}
		greedy_search (heuristic);
	}
	if (solver == Solver::A_STAR) {
		cout << "Solver: A*" << endl;
		if (heuristic == Heuristic::MISPLACED_TILES) {
			cout << "Heuristic: Misplaced tiles" << endl;
		}

		else {
			cout << "Heuristic: Manhattan distance" << endl;
		}
		a_star(heuristic);
	}
}

int Agent::misplaced_tiles(Puzzle puzzle) {
	int count = 0;
	//Calculating number of misplaced tiles
	for(int i = 0 ; i < NUM_ROWS ; i++){
		for(int j = 0 ; j < NUM_COLS ; j++){
			if(goal[i][j] !=0 && goal[i][j] != puzzle[i][j]){
				count ++;
			}
		}
	}
	return count;
}

int Agent::manhattan_distance(Puzzle puzzle) {
	int dist=0;
	//Calculating manhatten distance
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(puzzle[i][j]!=0)
			{
				for(int k=0;k<3;k++)
				{
					for(int l=0;l<3;l++)
					{
						if(puzzle[i][j]==goal[k][l])
							dist+=abs(i-k)+abs(j-l);
					}
				}
			}
		}
	}

	return dist;
}

void Agent::greedy_search(Heuristic heuristic) {
	//Record the start time
	chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

		bool goal_flag = true;

		//insert the initial puzzle to open list
		if (heuristic == Heuristic::MISPLACED_TILES) {
		open.insert(make_pair(misplaced_tiles(puzzle), puzzle));
		}else{
			open.insert(make_pair(manhattan_distance(puzzle), puzzle));
		}

		//Checking the initial position is equal to goal position
		if(check_goal(puzzle)){
			goal_flag = false;
		}

		vector<vector<int>> from_open;
		pair<int,int> zero_position;

		//Iterating the open list until the open_list is empty and finding the goal
		while(open.size()>0 && goal_flag == true){
		from_open = open.begin() -> second;
		open.erase(open.begin());
		if (heuristic == Heuristic::MISPLACED_TILES) {
			close.insert(make_pair(misplaced_tiles(from_open), from_open));
		}else{
			close.insert(make_pair(manhattan_distance(from_open), from_open));
		}

		// identify the empty space(position of zero)
		for(int i=0;i<3;i++)
		{
			for(int j = 0 ; j < NUM_COLS ; j++){
			if(from_open[i][j]==0)
				{
					zero_position.first=i;
					zero_position.second=j;
					break;
				}
			}
			}

		// Generating possible childrens and checking the children is goal or not
		for(int k=0;k<4;k++)
		{
			int cx = zero_position.first;
			int cy = zero_position.second;
			vector<vector<int> > children = from_open;
			if(safe(cx+dx[k],cy+dy[k]))
			{
				swap(children[cx+dx[k]][cy+dy[k]],children[cx][cy]);
				if(!check_explored(children)){
					if(check_goal(children)){
						goal_flag = false;
						cout << "goal found" << endl;

						//Calculating the visited nodes
						visited_nodes = close.size();

						//Calculating the path cost
						for (multimap<int,vector<vector<int>>>::iterator it=close.begin(); it!=close.end(); ++it){
							path_cost += it -> first;
						}
						open.clear();
						close.clear();
						print_puzzle(children);
						break;
					}else{
						if(!check_open(children)){
							if (heuristic == Heuristic::MISPLACED_TILES) {
								open.insert(make_pair(misplaced_tiles(children), children));
							}else{
								open.insert(make_pair(manhattan_distance(children), children));
							}

						}
					}
				}

			}
		}

	}
	//Record the end time and calculate the execution time
	chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
	chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
	cout << "Time Consumed: " << time_span.count() << " seconds.";
}

void Agent::a_star(Heuristic heuristic) {
	//Record the start time
	chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

		bool goal_flag = true;
		int g_of_n = 0;

		//insert the initial puzzle to open list
		if (heuristic == Heuristic::MISPLACED_TILES) {
		open_A_star.insert(make_pair(misplaced_tiles(puzzle)+g_of_n, make_pair(puzzle,g_of_n)));
		}else{
		open_A_star.insert(make_pair(manhattan_distance(puzzle)+g_of_n, make_pair(puzzle,g_of_n)));
		}

		//Checking the initial position is equal to goal position
		if(check_goal(puzzle)){
			goal_flag = false;
		}

		pair<vector<vector<int>>,int> from_open;
		int from_open_function_value=0;
		pair<int,int> zero_position;

		//Iterating the open list until the open_list is empty and finding the goal
		while(open_A_star.size()>0 && goal_flag == true ){

		from_open_function_value = open_A_star.begin() -> first;
		from_open = open_A_star.begin() -> second;
		open_A_star.erase(open_A_star.begin());
		close.insert(make_pair(from_open_function_value, from_open.first));

		// identify the empty space(position of zero)
		for(int i=0;i<3;i++)
		{
			for(int j = 0 ; j < NUM_COLS ; j++){
			if(from_open.first[i][j]==0)
				{
					zero_position.first=i;
					zero_position.second=j;
					break;
				}
			}
			}

		// Generating possible childrens and checking the children is goal or not
		for(int k=0;k<4;k++)
		{
			int cx = zero_position.first;
			int cy = zero_position.second;
			vector<vector<int> > children = from_open.first;
			if(safe(cx+dx[k],cy+dy[k]))
			{
				swap(children[cx+dx[k]][cy+dy[k]],children[cx][cy]);
				if(!check_explored(children)){
					if(check_goal(children)){
						goal_flag = false;
						cout << "goal found" << endl;

						//Calculating the visited nodes
						visited_nodes = close.size();

						//Calculating the path cost
						for (multimap<int,vector<vector<int>>>::iterator it=close.begin(); it!=close.end(); ++it){
							path_cost += it -> first;
						}
						open_A_star.clear();
						close.clear();
						print_puzzle(children);
						break;
					}else{

						int current_children_function_value = 0;
						if (heuristic == Heuristic::MISPLACED_TILES) {
							current_children_function_value = misplaced_tiles(children)+from_open.second+1;
						}else{
							current_children_function_value = manhattan_distance(children)+from_open.second+1;
						}

						if(!check_open_A_star(children,current_children_function_value)){
							if (heuristic == Heuristic::MISPLACED_TILES) {
							open_A_star.insert(make_pair(misplaced_tiles(children)+from_open.second+1, make_pair(children,from_open.second+1)));
							}else{
								open_A_star.insert(make_pair(manhattan_distance(children)+from_open.second+1, make_pair(children,from_open.second+1)));
							}
						}
					}
				}
			}
		}
	}

	//Record the end time and calculate the execution time
	chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
	chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
	cout << "Time Consumed: " << time_span.count() << " seconds.";
}

//function for checking a children is already present in close list
bool Agent::check_explored(vector<vector<int>> children) {
	vector<vector<int>> temp;
	int count = 0;
	 for (multimap<int,vector<vector<int>>>::iterator it=close.begin(); it!=close.end(); ++it){
		    temp = it->second;
			for(int j = 0; j<NUM_ROWS; j++){
				if(temp[j] == children[j]){
					++ count;
				}
			}
			if(count == 3){
				return true;
			}else
			{
				count = 0;
			}
	 }
	 return false;
}

//function for checking a children is already present in open list
bool Agent::check_open(vector<vector<int>> children) {
	vector<vector<int>> temp;
	int count = 0;
	 for (multimap<int,vector<vector<int>>>::iterator it=open.begin(); it!=open.end(); ++it){
		    temp = it->second;
			for(int j = 0; j<NUM_ROWS; j++){
				if(temp[j] == children[j]){
					++ count;
				}
			}
			if(count == 3){
				return true;
			}else
			{
				count = 0;
			}
	 }
	 return false;
}

//function for checking a children is already present in A star close list
bool Agent::check_open_A_star(vector<vector<int>> children,int current_children_function_value) {
	int function_value = 0;
	pair<vector<vector<int>>,int> temp;

	int count = 0;

	 for (multimap<int,pair<vector<vector<int>>,int>>::iterator it=open_A_star.begin(); it!=open_A_star.end(); ++it){
		 function_value = it->first;
		 temp = it->second;
			for(int j = 0; j<NUM_ROWS; j++){
				if(temp.first[j] == children[j]){
					++ count;
				}
			}
			if(count == 3){
				if(current_children_function_value<function_value){
					open_A_star.erase(it);
					return false;
				}else{
				return true;
				}
			}else
			{
				count = 0;
			}
	 }
	 return false;
}

//function for checking the children is a goal or not
bool Agent::check_goal(vector<vector<int>> children) {
	int count = 0;

	for(int j = 0; j<NUM_ROWS; j++){
		if(goal[j] == children[j]){
			++ count;
		}
	}
	if(count == 3){
		return true;
	}

	return false;
}

//function for checking the position is possible for children generation
bool Agent::safe(int i,int j)
{
	if(i>=0 && i<=2 && j>=0 && j<=2)
	return true;
	else
	return false;
}

//function for priting the output
void Agent::print_puzzle(Puzzle& puzzle) {
	for (int rows = 0; rows < NUM_ROWS; rows++) {
		for (int cols = 0; cols < NUM_COLS; cols++) {
			std::cout << "|" << puzzle[rows][cols];
		}
		std::cout << "|" << std::endl;
	}
	cout << "Number of visited nodes : " << visited_nodes << endl;
	cout << "Path cost: " << path_cost << endl;
	path_cost = 0;
}


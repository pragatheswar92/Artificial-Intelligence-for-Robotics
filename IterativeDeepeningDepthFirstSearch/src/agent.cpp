/*
 * Pragatheswar Nagarajan
 * Rubanraj Ravichandran
 * Aritificial Intelligence for Robotics
 * WS 2016
 * Assignment 5
 *
 * agent.cpp
 * */

#include "agent.hpp"

#include <iostream>
#include <string>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <algorithm>

#define map_rows 25
#define map_cols 141

using namespace std;

Agent::Agent(vector<vector<string> > selected_map, const pair<int, int> initial_pos, int number_of_goals):
empty_map(selected_map),
map(selected_map),
initial_pos(initial_pos),
number_of_goals(number_of_goals),
max_number_of_stored_nodes(0),
number_of_visited_nodes(0),
total_of_stored_nodes(0),
deepest_level(0)
{
    
    max_limit = map_rows * map_cols;
    print_map(empty_map);
}

int current_goal = 0;
pair<int, int> initial_pos_dls;
Agent::~Agent()
{
}

void Agent::run()
{
    cout << "Running IDFS " << endl;
    cout << "Number of goals " << number_of_goals << endl;
    sleep(1);
    iterative_deepening_search();
}

void Agent::print_map(vector<vector<string> >& a_map)
{
	system("clear");
	
    int row = initial_pos.first;
    int col = initial_pos.second;


    for(int row = 0; row < map_rows; row++)
    {
        for(int col = 0; col < map_cols; col++)
        {
            cout << a_map[row][col];
        }    
        cout << endl;
    }
    
    this_thread::sleep_for(chrono::milliseconds(20));
}


 bool Agent::recursive_dls(pair<int, int> current_node, int goal, int current_level, int limit,
		 vector<pair<int, int> > current_path)
 {
	 pair<int, int> children;
	  if(limit >= 0){
		  if (map[current_node.first][current_node.second] == std::to_string(goal)){
			  initial_pos_dls = current_node;
			  backtrack_path(current_path);
			  if(deepest_level < current_level){
				  deepest_level = current_level;
			  }
			  return true;
		  }

		current_path.push_back(current_node);

		total_of_stored_nodes++;

		//children one
		children = make_pair(current_node.first, (current_node.second-1));

			if(map[children.first][children.second] == std::to_string(goal)){
						initial_pos_dls = children;
						current_path.push_back(children);
						backtrack_path(current_path);
						  if(deepest_level < current_level){
							  deepest_level = current_level;
						  }
						return true;
					}
				else if(map[children.first][children.second] == " "){
						map[children.first][children.second] = "-";
						number_of_visited_nodes++;
						if(recursive_dls(children, current_goal, current_level, limit-1, current_path)){
							return true;
						}
				}

			//children two
			children = make_pair(current_node.first, (current_node.second+1));

			if(map[children.first][children.second] == std::to_string(goal)){
					initial_pos_dls = children;
					current_path.push_back(children);
					backtrack_path(current_path);
					  if(deepest_level < current_level){
						  deepest_level = current_level;
					  }
					return true;
					}
				else if(map[children.first][children.second] == " "){
						map[children.first][children.second] = "-";
						number_of_visited_nodes++;
						if(recursive_dls(children, current_goal, current_level, limit-1, current_path)){
							return true;
						}
				}

			//children three
			children = make_pair((current_node.first-1), current_node.second);

			if(map[children.first][children.second] == std::to_string(goal)){
						number_of_visited_nodes++;
						initial_pos_dls = children;
						current_path.push_back(children);
						backtrack_path(current_path);
						  if(deepest_level < current_level){
							  deepest_level = current_level;
						  }
						return true;
					}
				else if(map[children.first][children.second] == " "){
						number_of_visited_nodes++;
						map[children.first][children.second] = "-";
						number_of_visited_nodes++;
						if(recursive_dls(children, current_goal, current_level, limit-1, current_path)){
							return true;
						}
				}

			//children four
			children = make_pair((current_node.first+1), current_node.second);

			if(map[children.first][children.second] == std::to_string(goal)){
						number_of_visited_nodes++;
						initial_pos_dls = children;
						current_path.push_back(children);
						backtrack_path(current_path);
						  if(deepest_level < current_level){
							  deepest_level = current_level;
						  }
						return true;
					}
				else if(map[children.first][children.second] == " "){
						map[children.first][children.second] = "-";
						number_of_visited_nodes++;
						if(recursive_dls(children, current_goal, current_level, limit-1, current_path)){
							return true;
						}
				}
			return false;
	  }else{
		  return false;
	  }
}

 bool Agent::depth_limited_seach(int limit)
 {
	 vector<pair<int, int> >current_path; 
	 map = empty_map;
	 if(limit <= max_limit){
	 if(recursive_dls(initial_pos_dls, current_goal, limit, limit, current_path)){
		 return true;
	 }else{
		 return false;
	 }
	 }else{
		 cout << "Goal " << current_goal << " not found"<<endl;
		 return true;
	 }
 }

 void Agent::iterative_deepening_search()
 {
	 int depth = 0;
	 bool flag = false;
	 initial_pos_dls = initial_pos;
	 for(int i = 1; i<=number_of_goals;i++){
		 current_goal = i;
		 map = empty_map;
		 flag = false;
		 depth = 0;
		 while(!flag){
			 flag = depth_limited_seach(depth);
			 depth = depth + 5;
		 }
	 }

	 print_final_results();
 }

void Agent::print_final_results()
{   
    cout << "Deepest level reached: " << deepest_level  << endl;
    cout << "Total of stored nodes: " << total_of_stored_nodes << endl;
    cout << "Total of visited nodes: " << number_of_visited_nodes << endl;
} 

void Agent::backtrack_path(vector<pair<int, int> > current_path)
{

    vector<vector<string> > local_map = empty_map;
    
    pair<int, int> current_data;

    for(pair<int, int> current_data:current_path){
    	local_map[current_data.first][current_data.second] = ".";
    }
    print_map(local_map);
}

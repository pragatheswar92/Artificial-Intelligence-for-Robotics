/*
 * Rubanraj Ravichandran
 * Pragatheswar Nagarajan
 * Aritifiacial Intelligence for Robots
 * MAS ws 2016
 * Assignment 4
 *
 * agent.cpp
 * */

#include "agent.hpp"

#include <iostream>
#include <stack>
#include <queue>

#include <chrono>
#include <thread>
#include <time.h>


using namespace std;

int max_size_queue = 0;
double elapsed_secs = 0.0;
time_t start1;
time_t end1;

Agent::Agent(vector<vector<string> > map, const pair<int, int> initial_pos,
		int number_of_goals, int search_option) :
		map(map), initial_pos(initial_pos), number_of_goals(number_of_goals), search_option(
				search_option), number_of_stored_nodes(0), number_of_visited_nodes(
				0) {

}

Agent::~Agent() {

}

void Agent::run() {
	if (search_option == 1) {
		cout << "Running BFS " << endl;
		cout << "Number of goals " << number_of_goals << endl;
		bfs();
	} else {
		cout << "Running DFS " << endl;
		cout << "Number of goals " << number_of_goals << endl;
		dfs();
	}

}

void Agent::print_map() {
	system("clear");

	for (int row = 0; row < map_rows; row++) {
		for (int col = 0; col < map_cols; col++) {
			cout << map[row][col];
		}
		cout << endl;
	}

	this_thread::sleep_for(chrono::milliseconds(10));

}

void Agent::bfs() {
	time (&start1);
	queue<pair<int, int> > nodes_queue;
	pair<int, int> current_node;
	pair<int, int> children;

	nodes_queue.push(make_pair(initial_pos.first, initial_pos.second));
	max_size_queue = nodes_queue.size();
	number_of_stored_nodes++;

	//checking initial node is goal node or not
	current_node = nodes_queue.front();

	if(map[current_node.first][current_node.second] == "*"){
		goal_positions.push_back(current_node);
	}


	while (nodes_queue.size() >0 && goal_positions.size()!=number_of_goals) {


		current_node = nodes_queue.front();
		//Checking maximum queue size
		if(max_size_queue < nodes_queue.size()){
			max_size_queue = nodes_queue.size();
		}

		nodes_queue.pop();
		map[current_node.first][current_node.second] = "-";
		number_of_visited_nodes++;

		children = make_pair(current_node.first, (current_node.second-1));
			if(map[children.first][children.second] == "*"){
					goal_positions.push_back(children);
					map[children.first][children.second] = "-";
					number_of_visited_nodes++;
				}
			else if(map[children.first][children.second] == " "){
				if(!find(nodes_queue,children)){
					nodes_queue.push(children);
					number_of_stored_nodes++;
				}
			}


		children = make_pair(current_node.first, (current_node.second+1));
			if(map[children.first][children.second] == "*"){
					goal_positions.push_back(children);
					map[children.first][children.second] = "-";
					number_of_visited_nodes++;
				}
			else if(map[children.first][children.second] == " "){
				if(!find(nodes_queue,children)){
					nodes_queue.push(children);
					number_of_stored_nodes++;
				}
			}


		children = make_pair(current_node.first-1, (current_node.second));
			if(map[children.first][children.second] == "*"){
					goal_positions.push_back(children);
					map[children.first][children.second] = "-";
					number_of_visited_nodes++;
				}
			else if(map[children.first][children.second] == " "){
				if(!find(nodes_queue,children)){
					nodes_queue.push(children);
					number_of_stored_nodes++;
				}
			}


		children = make_pair(current_node.first+1, (current_node.second));
			if(map[children.first][children.second] == "*"){
					goal_positions.push_back(children);
					map[children.first][children.second] = "-";
					number_of_visited_nodes++;
				}
			else if(map[children.first][children.second] == " "){
				if(!find(nodes_queue,children)){
					nodes_queue.push(children);
					number_of_stored_nodes++;
				}
			}

		print_map();

	}
	time (&end1);
	elapsed_secs = difftime (end1,start1);
	print_evaluation_metrics("queue");
}


bool Agent::find(queue<pair<int, int> > nodes_queue,pair<int, int> children){


	bool flag = false;
	while(nodes_queue.size()>0 && flag == false){
		 if(nodes_queue.front().first == children.first && nodes_queue.front().second == children.second){
			 flag =  true;
				   }
		 nodes_queue.pop();
	}

	   return flag;
}
void Agent::dfs() {
	time (&start1);
	stack<pair<int, int> > nodes_stack;
	pair<int, int> current_node;
	pair<int, int> children;

	nodes_stack.push(make_pair(initial_pos.first, initial_pos.second));
	max_size_queue = nodes_stack.size();
	number_of_stored_nodes++;

	//checking initial node is goal node or not
		current_node = nodes_stack.top();

		if(map[current_node.first][current_node.second] == "*"){
			goal_positions.push_back(current_node);
		}

	while (nodes_stack.size() > 0 && goal_positions.size()!=number_of_goals) {

		current_node = nodes_stack.top();

		//Checking maximum queue size
		if(max_size_queue < nodes_stack.size()){
			max_size_queue = nodes_stack.size();
		}
		nodes_stack.pop();

		map[current_node.first][current_node.second] = "-";
		number_of_visited_nodes++;
		children = make_pair(current_node.first, (current_node.second-1));
			if(map[children.first][children.second] == "*"){
					goal_positions.push_back(children);
					map[children.first][children.second] = "-";
					number_of_visited_nodes++;
				}
			else if(map[children.first][children.second] == " "){
				if(!findDFS(nodes_stack,children)){
					nodes_stack.push(children);
					number_of_stored_nodes++;
				}
			}


		children = make_pair(current_node.first, (current_node.second+1));
			if(map[children.first][children.second] == "*"){
					goal_positions.push_back(children);
					map[children.first][children.second] = "-";
					number_of_visited_nodes++;
				}
			else if(map[children.first][children.second] == " "){
				if(!findDFS(nodes_stack,children)){
					nodes_stack.push(children);
					number_of_stored_nodes++;
				}
			}


		children = make_pair(current_node.first-1, (current_node.second));
			if(map[children.first][children.second] == "*"){
					goal_positions.push_back(children);
					map[children.first][children.second] = "-";
					number_of_visited_nodes++;
				}
			else if(map[children.first][children.second] == " "){
				if(!findDFS(nodes_stack,children)){
					nodes_stack.push(children);
					number_of_stored_nodes++;
				}
			}


		children = make_pair(current_node.first+1, (current_node.second));
			if(map[children.first][children.second] == "*"){
					goal_positions.push_back(children);
					map[children.first][children.second] = "-";
					number_of_visited_nodes++;
				}
			else if(map[children.first][children.second] == " "){
				if(!findDFS(nodes_stack,children)){
					nodes_stack.push(children);
					number_of_stored_nodes++;
				}
			}

		print_map();

	}

		time (&end1);
		elapsed_secs = difftime (end1,start1);
	print_evaluation_metrics("stack");

}

bool Agent::findDFS(stack<pair<int, int> > nodes_stack,pair<int, int> children){


	bool flag = false;
	while(nodes_stack.size()>0 && flag == false){
		 if(nodes_stack.top().first == children.first && nodes_stack.top().second == children.second){
			 flag =  true;
				   }
		 nodes_stack.pop();
	}

	   return flag;
}

void Agent::print_evaluation_metrics(string data_structure_name) {
	print_map();
	cout << "All the possible nodes have been explored " << endl;
	cout << "Found " << goal_positions.size() << " of " << number_of_goals
			<< endl;
	cout << "Maximum size of the " << data_structure_name << " : "
			<<  max_size_queue << endl;
	cout << "Number of stored nodes: " << number_of_stored_nodes << endl;
	cout << "Total of visited nodes: " << number_of_visited_nodes << endl;
	cout << "Time Consumed: " << elapsed_secs << " Seconds"<<endl;
	print_goal_positions();
}

void Agent::print_goal_positions() {
	cout << "Goals: " << endl;
	for (int i = 0; i < goal_positions.size(); i++) {
		cout << i + 1 << ": (" << goal_positions[i].first << ", "
				<< goal_positions[i].second << ")" << endl;
	}
}


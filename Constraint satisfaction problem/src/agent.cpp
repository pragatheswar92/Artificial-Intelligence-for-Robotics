/*
 * Rubanraj Ravichandran
 * Pragatheswar Nagarajan
 * Aritificial Intelligence for Robotics
 * WS 2016
 * Assignment 9
 *
 * agent.cpp
 * */
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <agent.hpp>
#include<math.h>

int counter = 0;

Agent::Agent(vector<Location> scenario1, vector<Location> scenario2, vector<Location> scenario3, vector<Location> scenario4, vector<Location> scenario5):scenario1(scenario1),
scenario2(scenario2),
scenario3(scenario3),
scenario4(scenario4),
scenario5(scenario5) 
{
	
    
}

Agent::~Agent()
{

}

void Agent::run()
{
     int choice;

     cout << "Please select the scenario.. " << endl;
     cout << "(1)scenario1 (2)scenario2 (3)scenario3 (4)scenario4 (5)scenario5" << endl;
     cin >> choice;

     switch(choice){
         case 1:
                execute(scenario1);
                break;
         case 2:
                execute(scenario2);
                break;
         case 3:
                execute(scenario3);
                break;
         case 4:
                execute(scenario4);
                break;
         case 5:
                execute(scenario5);
                break;
     }
     
}

//this function gets the scenario by user selection and pass the scenario to dbs_bactracking() method.
void Agent::execute(vector<Location> scenario){
     cout << endl ;
     cout << "Line number in the list method : " << endl;
     dbs_bactracking(scenario);

     cout << endl ;
     cout << "Euclidean distance method : " << endl;
     vector<Location> euclidean_scenario = sort_by_euclidean(scenario);
     dbs_bactracking(euclidean_scenario);

     cout << endl ;
     cout << "Time to deadline method : " << endl;
     vector<Location> deadline_scenario = sort_by_deadline(scenario);
     dbs_bactracking(deadline_scenario);
}

//This function takes a scenario as input and sort it according to the euclidean distance
vector<Location> Agent::sort_by_euclidean(vector<Location> scenario){

    vector<float> initial_order;
    initial_order.push_back(0.0);
    vector<Location> euclidean_scenario;
    for(int i = 1; i<scenario.size();i++){
       initial_order.push_back(distanceCalculate(scenario[0].get_x(),scenario[0].get_y(),scenario[i].get_x(),scenario[i].get_y()));
    }
    vector<float> copy = initial_order;
    sort (initial_order.begin(), initial_order.end());
    for(int i = 0; i<initial_order.size();i++){
    euclidean_scenario.push_back(scenario[find(copy.begin(), copy.end(), initial_order[i]) - copy.begin()]);
    }

    return euclidean_scenario; 
}


//This function takes a scenario as input and sort it according to the deadline
vector<Location> Agent::sort_by_deadline(vector<Location> scenario){
    
    vector<float> initial_order;
    vector<Location> deadline_scenario;
    for(int i = 0; i<scenario.size();i++){
        initial_order.push_back(scenario[i].get_deadline());
    }
    vector<float> copy = initial_order;
    sort (initial_order.begin(), initial_order.end());

    for(int i = 0; i<initial_order.size();i++){
    deadline_scenario.push_back(scenario[find(copy.begin(), copy.end(), initial_order[i]) - copy.begin()]);
    }

    return deadline_scenario; 
}

//This is an utility function to print the scenario
void Agent::print_scenario(vector<Location>& scenario) {
    
    std::vector<double> x, y;
    for (int i = 0; i < scenario.size(); i++) {
        std::cout << "Location: " << scenario[i].get_x() << "," << scenario[i].get_y() << " Deadline: " << scenario[i].get_deadline() << std::endl;
    }
    
}


//this is the main function gets the scenario as input and start the recursive_bactracking algorithm
vector<Location> Agent::dbs_bactracking(vector<Location> scenario) {

    vector<int> assignment;
    vector<int> variables;
    float total_length = 0.0;

    for(int i=0;i<scenario.size();i++){
        variables.push_back(i);
    }

    assignment.push_back(0);
    vector<int> result = recursive_bactracking(assignment,scenario,variables,0.0);
    
    if(result.size() == scenario.size()){
        total_length += distanceCalculate(scenario[0].get_x(),scenario[0].get_y(),scenario[0].get_x(),scenario[0].get_y());

        cout << result[0] << " -->  position x : " << scenario[result[0]].get_x() 
        << " position y : " << scenario[result[0]].get_y() << 
        " deadline : " << scenario[result[0]].get_deadline() <<
        " total distance: " << total_length << endl;

            for(int i=0;i<result.size()-1;i++){
            total_length += distanceCalculate(scenario[result[i]].get_x(),scenario[result[i]].get_y(),scenario[result[i+1]].get_x(),scenario[result[i+1]].get_y());
            cout << result[i+1] << " -->  position x : " << scenario[result[i+1]].get_x() 
            << " position y : " << scenario[result[i+1]].get_y() << 
            " deadline : " << scenario[result[i+1]].get_deadline() <<
            " total distance: " << total_length << endl;
        }
    }else{
        cout << "There is no possible path for this scenario" <<endl;
    }
    cout<< "Number of evaluation : " << counter <<endl;
    cout << "Total path length : " << calculate_path_length(result,scenario) << endl;
    counter = 0;
    return scenario;
}


//This recursive_bactracking() method finds the possible path to drop the packages within the deadline
vector<int> Agent::recursive_bactracking(vector<int> assignment,vector<Location> scenario,vector<int> variables,float distance) {
    if(assignment.size() != scenario.size()){
        vector<int> unassigned = select_unassigned_variable(variables,assignment);
        for (std::vector<int>::iterator it = unassigned.begin(); it != unassigned.end(); ++it){
            Location location_one = scenario[assignment.back()];
            Location location_two = scenario[*it];float current_distance = (distanceCalculate(location_one.get_x(),location_one.get_y(),location_two.get_x(),location_two.get_y()) + distance);
             counter ++;
            if( current_distance < location_two.get_deadline() ){
                //cout << location_two.get_deadline() << endl;
                assignment.push_back(*it);
                vector<int> result = recursive_bactracking(assignment,scenario,variables,current_distance);
                if(result.size() == scenario.size()){
                    return result;
                }else{
                    assignment.pop_back();
                }
            }
    }
    }
        return assignment;
}

//This is an utility function used to filter unassigned cities from the initial set of cities
vector<int> Agent::select_unassigned_variable(vector<int> variables,vector<int> assignment){
    vector<int> unassigned;
        for (std::vector<int>::iterator it_ass = variables.begin(); it_ass != variables.end(); ++it_ass){
            if(!(find(assignment.begin(), assignment.end(), *it_ass) != assignment.end())){
                unassigned.push_back(*it_ass);
            }
    }

    return unassigned;
   
}

//This is an utility function used to calculate the euclidean distance between two co-ordinates

float Agent::distanceCalculate(float x1, float y1, float x2, float y2)
{
    float x = x1 - x2;
    float y = y1 - y2;
    float dist;

    dist = pow(x,2)+pow(y,2); 
    dist = sqrt(dist);

    return dist;
}

//This is an utility function used to calculate the total path length
float Agent::calculate_path_length(vector<int> result,vector<Location> scenario){
    float total_length = 0.0;
    for(int i=0;i<result.size()-1;i++){
        total_length += distanceCalculate(scenario[result[i]].get_x(),scenario[result[i]].get_y(),scenario[result[i+1]].get_x(),scenario[result[i+1]].get_y());
    }
    return total_length;
}
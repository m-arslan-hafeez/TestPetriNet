#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <thread>

using namespace std;

// Function to generate a random 2-digit number
int generateRandom2DigitNumber(int routeNumber) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dist(10, 99); // 2-digit range
	return (routeNumber * 100) + dist(gen);
}

int main() {
	int totalInRoutes = 4;
	srand(time(0)); // Seed the random number generator with the current time
	int totalOutRoutes = 2;

	cout << "Total 'in' routes: " << totalInRoutes << endl;
	cout << "Total 'out' routes: " << totalOutRoutes << endl;

	// Create vectors to store the number of vehicles for "in" and "out" routes
	vector<int> vehiclesInPerRoute{ 50, 137, 65, 53 }; // Specific number of vehicles for each in-route
	vector<int> vehiclesOutPerRoute(totalOutRoutes); // Total vehicles for each out-route
	vector<vector<int>> assignedOutRouteVehicles(totalOutRoutes); // Vehicles assigned to each out-route
	vector<int> routeTimes(totalInRoutes, 0); // Time for each in-route

	// Generate random 2-digit numbers for vehicles in "in" routes
	vector<int> allVehicles;
	int totalVehiclesIn = 0; // Total vehicles in "in" routes
	vector<pair<int, int>> routePriorities; // Pair of <priority, route>

	for (int inRoute = 0; inRoute < totalInRoutes; inRoute++) {
		int numVehicles = vehiclesInPerRoute[inRoute]; // Specific number of vehicles for each in-route
		vehiclesInPerRoute[inRoute] = numVehicles;
		totalVehiclesIn += numVehicles;
		routePriorities.push_back({ numVehicles, inRoute });

		// Assign a random 2-digit number to each vehicle in this in-route
		vector<int> inRouteVehicles;
		for (int i = 0; i < numVehicles; i++) {
			int vehicleNumber = generateRandom2DigitNumber(inRoute + 1);
			allVehicles.push_back(vehicleNumber);
			inRouteVehicles.push_back(vehicleNumber);
		}

		cout << "'in' Route " << (inRoute + 1) << ": " << numVehicles << " \nVehicles - Vehicle Numbers: " << endl;
		for (const int vehicleNumber : inRouteVehicles) {
			cout << vehicleNumber << " ";
		}
		cout << endl;
	}

	// Shuffle all vehicles randomly
	random_shuffle(allVehicles.begin(), allVehicles.end());

	// Sort the route priorities based on the number of vehicles in descending order
	sort(routePriorities.rbegin(), routePriorities.rend());

	// Create data files for gnuplot to store data
	ofstream inRouteDataFile("in_route_data.txt");
	ofstream outRouteDataFile("out_route_data.txt");
	ofstream timeDataFile("time_data.txt");
	int priorityOrder = 1;

	// Display vehicle sequence for each route
	ofstream sequenceFile("vehicle_sequence.txt");

	for (const auto& routePriority : routePriorities) {
		int routeWithMaxVehicles = routePriority.second;
		int vehiclesToMove = vehiclesInPerRoute[routeWithMaxVehicles];

		for (int i = 0; i < vehiclesToMove; i++) {
			int outRoute = rand() % totalOutRoutes; // Pick a random "out" route
			int vehicleNumber = allVehicles.back();
			assignedOutRouteVehicles[outRoute].push_back(vehicleNumber);
			vehiclesOutPerRoute[outRoute]++;
			allVehicles.pop_back();

			// Store the vehicle sequence in the sequence file
			sequenceFile << "'in' Route " << (routeWithMaxVehicles + 1) << " -> 'out' Route " << (outRoute + 1) << " - \nVehicle Number: " << vehicleNumber << endl;
		}

		// Calculate the time based on the number of vehicles (adjust this calculation as needed)
		int calculatedTime = vehiclesToMove * 2; // Adjust this as needed

		// Update the time for the in-route
		routeTimes[routeWithMaxVehicles] += calculatedTime;

		// Store data for the in-route, out-route, and time in their respective data files
		inRouteDataFile << priorityOrder << " " << routeWithMaxVehicles + 1 << " " << vehiclesInPerRoute[routeWithMaxVehicles] << endl;
		timeDataFile << priorityOrder << " " << routeWithMaxVehicles + 1 << " " << calculatedTime << endl;
		outRouteDataFile << priorityOrder << " " << routeWithMaxVehicles + 1 << " " << vehiclesOutPerRoute[routeWithMaxVehicles] << endl;
		priorityOrder++;
	}

	// Close the sequence file
	sequenceFile.close();

	// Calculate the total number of vehicles passed through "out" routes
	int totalVehiclesOut = 0;
	for (int outRoute = 0; outRoute < totalOutRoutes; outRoute++) {
		totalVehiclesOut += vehiclesOutPerRoute[outRoute];
	}

	// Calculate the total time
	int totalTime = 0;
	for (int time : routeTimes) {
		totalTime += time;
	}

	cout << "Total number of vehicles in 'in' routes : " << totalVehiclesIn << endl;
	cout << "Total number of vehicles passed through 'out' routes : " << totalVehiclesOut << endl;
	cout << "Total time took to clear traffic : " << totalTime << " seconds" << endl;

	// Execute the Gnuplot script for the total data
	system("gnuplot plot_total_data.plt");

	inRouteDataFile.close();
	outRouteDataFile.close();
	timeDataFile.close();

	return 0;
}

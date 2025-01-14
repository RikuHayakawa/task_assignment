#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace std;

void generateInput(int tasks, int robots, int stations, int stationCapacity, int constraintTime, int guaranteedEnergy, ofstream &outFile)
{
    srand(time(0)); // Initialize random seed

    // Generate UUID
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    outFile << uuid << endl;

    outFile << tasks << " " << robots << " " << stations << endl;
    outFile << constraintTime << endl;
    outFile << guaranteedEnergy << endl;

    // Generate initial battery levels
    for (int i = 0; i < robots; i++)
    {
        outFile << (rand() % 11) << (i == robots - 1 ? "\n" : " "); // Random value between 0 and 10
    }

    // Generate maximum battery capacities
    for (int i = 0; i < robots; i++)
    {
        outFile << (10) << (i == robots - 1 ? "\n" : " "); // Fixed value as per the example
    }

    // Generate charging efficiencies for stations
    for (int i = 0; i < stations; i++)
    {
        outFile << "1" << (i == stations - 1 ? "\n" : " "); // Fixed value as per the example
    }

    // Generate station capacities
    for (int i = 0; i < stations; i++)
    {
        outFile << stationCapacity << (i == stations - 1 ? "\n" : " "); // Fixed value as per the example
    }

    // Generate task execution times matrix
    vector<vector<int>> taskTimes(tasks, vector<int>(robots, 0));
    for (int i = 0; i < tasks; i++)
    {
        for (int j = 0; j < robots; j++)
        {
            taskTimes[i][j] = rand() % 3 + 1; // Random value between 1 and 3
            outFile << taskTimes[i][j] << (j == robots - 1 ? "\n" : " ");
        }
    }

    // Generate task energy consumption matrix (efficiency * time)
    std::vector<int> efficiencies(robots, 0);
    for (int i = 0; i < robots; i++)
    {
        efficiencies[i] = rand() % 2 + 1; // Random value between 1 and 2
    }
    for (int i = 0; i < tasks; i++)
    {
        for (int j = 0; j < robots; j++)
        {
            outFile << efficiencies[j] * taskTimes[i][j] << (j == robots - 1 ? "\n" : " "); // Random value between 1 and 6
        }
    }

    // Generate profit matrix
    for (int i = 0; i < tasks; i++)
    {
        for (int j = 0; j < robots; j++)
        {
            outFile << (rand() % 5 + 1) << (j == robots - 1 ? "\n" : " "); // Random value between 1 and 5
        }
    }
}

int main()
{
    int tasks, robots, stations, stationCapacity, constraintTime, guaranteedEnergy;

    // Input the fixed values
    cout << "Enter the number of tasks: ";
    cin >> tasks;
    cout << "Enter the number of robots: ";
    cin >> robots;
    cout << "Enter the station capacity: ";
    cin >> stationCapacity;
    cout << "Enter the constraint time: ";
    cin >> constraintTime;
    cout << "Enter the guaranteed energy: ";
    cin >> guaranteedEnergy;

    // Open output file
    ofstream outFile("generatedtest.txt");
    if (!outFile)
    {
        cerr << "Error: Could not open file generatedtest.txt for writing." << endl;
        return 1;
    }

    // Generate the input data
    generateInput(tasks, robots, stations = 1, stationCapacity, constraintTime, guaranteedEnergy, outFile);

    cout << "Input data has been written to generatedtest.txt" << endl;

    return 0;
}
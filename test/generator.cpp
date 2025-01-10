#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

void generateInput(int tasks, int robots, int stations, int constraintTime, ofstream &outFile)
{
    srand(time(0)); // Initialize random seed

    outFile << tasks << " " << robots << " " << stations << endl;
    outFile << constraintTime << endl;

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

    // Generate energy consumption efficiencies
    for (int i = 0; i < robots; i++)
    {
        outFile << (rand() % 2 + 1) << (i == robots - 1 ? "\n" : " "); // Random value between 1 and 2
    }

    // Generate charging efficiencies for stations
    for (int i = 0; i < stations; i++)
    {
        outFile << "1" << (i == stations - 1 ? "\n" : " "); // Fixed value as per the example
    }

    // Generate task execution times
    for (int i = 0; i < tasks; i++)
    {
        outFile << (rand() % 3 + 1) << (i == tasks - 1 ? "\n" : " "); // Random value between 1 and 3
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
    int tasks, robots, stations, constraintTime;

    // Input the fixed values
    cout << "Enter the number of tasks: ";
    cin >> tasks;
    cout << "Enter the number of robots: ";
    cin >> robots;
    cout << "Enter the number of stations: ";
    cin >> stations;
    cout << "Enter the constraint time: ";
    cin >> constraintTime;

    // Open output file
    ofstream outFile("generatedtest.txt");
    if (!outFile)
    {
        cerr << "Error: Could not open file generatedtest.txt for writing." << endl;
        return 1;
    }

    // Generate the input data
    generateInput(tasks, robots, stations, constraintTime, outFile);

    cout << "Input data has been written to generatedtest.txt" << endl;

    return 0;
}
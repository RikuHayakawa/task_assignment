#include <iostream>
#include <fstream>
#include <vector>
#include <exec_result.h>

namespace exec_result
{
    ExecResult::ExecResult()
    {
    }
    ExecResult::ExecResult(std::string test_id, int total_tasks, int total_robots, int constraint_time, int guaranteed_energy, int station_capacity, double gap_for_constraint_size_time, double gap_for_constraint_time, double schedule_planner_time, double exec_time, std::vector<int> occupation_per_time, int competition, double standard_deviation, double value)
        : m_test_id(test_id), m_total_tasks(total_tasks), m_total_robots(total_robots), m_constraint_time(constraint_time), m_guaranteed_energy(guaranteed_energy), m_station_capacity(station_capacity), m_gap_for_constraint_size_time(gap_for_constraint_size_time), m_gap_for_constraint_time(gap_for_constraint_time), m_schedule_planner_time(schedule_planner_time), m_exec_time(exec_time), m_occupation_per_time(occupation_per_time), m_competition(competition), m_standard_deviation(standard_deviation), m_value(value)
    {
    }
    ExecResult::~ExecResult()
    {
    }
    void ExecResult::Print()
    {
        std::cout << "Test ID: " << m_test_id << std::endl;
        std::cout << "Total tasks: " << m_total_tasks << std::endl;
        std::cout << "Total robots: " << m_total_robots << std::endl;
        std::cout << "Constraint time: " << m_constraint_time << std::endl;
        std::cout << "Guaranteed energy: " << m_guaranteed_energy << std::endl;
        std::cout << "Station capacity: " << m_station_capacity << std::endl;
        std::cout << "Gap for constraint size time: " << m_gap_for_constraint_size_time << std::endl;
        std::cout << "Gap for constraint time: " << m_gap_for_constraint_time << std::endl;
        std::cout << "Schedule planner time: " << m_schedule_planner_time << std::endl;
        std::cout << "Exec time: " << m_exec_time << std::endl;
        std::cout << "Occupation per time: ";
        for (int i = 0; i < m_occupation_per_time.size(); i++)
        {
            std::cout << m_occupation_per_time[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "Competition: " << m_competition << std::endl;
        std::cout << "Standard deviation: " << m_standard_deviation << std::endl;
        std::cout << "Value: " << m_value << std::endl;
    }
}
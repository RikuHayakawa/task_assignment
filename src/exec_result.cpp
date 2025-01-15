#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <exec_result.h>
#include <algorithm>
#include <schedule_planner.h>

namespace exec_result
{
    ExecResult::ExecResult()
    {
    }
    ExecResult::ExecResult(std::string test_id, int total_tasks, int total_robots, int constraint_time, int guaranteed_energy, int station_capacity,
                           double gap_for_constraint_size_time, double gap_for_constraint_time, double schedule_planner_time, double exec_time, std::vector<int> initial_occupation, std::vector<int> schedule_occupation)
        : m_test_id(test_id), m_total_tasks(total_tasks), m_total_robots(total_robots), m_constraint_time(constraint_time), m_guaranteed_energy(guaranteed_energy), m_station_capacity(station_capacity),
          m_gap_for_constraint_size_time(gap_for_constraint_size_time), m_gap_for_constraint_time(gap_for_constraint_time), m_schedule_planner_time(schedule_planner_time), m_exec_time(exec_time),
          m_initial_occupation(initial_occupation), m_schedule_occupation(schedule_occupation)
    {
        calculateStationOccupation(initial_occupation, m_initial_result);
        calculateStationOccupation(schedule_occupation, m_scheduled_result);
    }
    ExecResult::~ExecResult()
    {
    }

    void ExecResult::calculateStationOccupation(vector<int> occupation_per_time, std::tuple<int, double, double, double, int> &result)
    {
        schedule_planner::SchedulePlanner schedule;
        std::tuple<int, double, double> initialResult = schedule.calculateValue(occupation_per_time, m_station_capacity);
        // m_initial_occupationから最大値を取得
        int max = *std::max_element(occupation_per_time.begin(), occupation_per_time.end());
        double average = 0;
        for (int i = 0; i < occupation_per_time.size(); i++)
        {
            average += occupation_per_time[i];
        }
        average /= occupation_per_time.size();
        result = std::make_tuple(std::get<0>(initialResult), std::get<1>(initialResult), std::get<2>(initialResult), average, max);
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
        std::cout << "Initial occupation: ";
        for (int i = 0; i < m_initial_occupation.size(); i++)
        {
            std::cout << m_initial_occupation[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "Schedule occupation: ";
        for (int i = 0; i < m_schedule_occupation.size(); i++)
        {
            std::cout << m_schedule_occupation[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "Initial result: " << "競合数" << std::get<0>(m_initial_result) << " 標準偏差" << std::get<1>(m_initial_result) << " 評価値" << std::get<2>(m_initial_result) << " 平均収容率" << std::get<3>(m_initial_result) << " 最大収容数" << std::get<4>(m_initial_result) << std::endl;
        std::cout << "Scheduled result: " << "競合数" << std::get<0>(m_scheduled_result) << " 標準偏差" << std::get<1>(m_scheduled_result) << " 評価値" << std::get<2>(m_scheduled_result) << " 平均収容率" << std::get<3>(m_scheduled_result) << " 最大収容数" << std::get<4>(m_scheduled_result) << std::endl;
    }
}
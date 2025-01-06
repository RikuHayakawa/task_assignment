#include "schedule_planner.h"
#include "schedule_option.h"
#include <iostream>
#include <vector>
#include <algorithm>

namespace schedule_planner
{

    SchedulePlanner::SchedulePlanner()
        : m_gap_instance(nullptr)
    {
    }

    SchedulePlanner::~SchedulePlanner()
    {
    }

    SchedulePlanner::SchedulePlanner(gap::CGap *gap_instance)
        : m_gap_instance(gap_instance)
    {
        for (int i = 0; i < m_gap_instance->m_bins.size(); i++)
        {
            std::vector<std::pair<int, int>> tasks;

            int charging_id = -1;

            for (const auto &task : m_gap_instance->m_bins[i].m_assignment)
            {
                if (task.first == "charging")
                {
                    charging_id = task.second;
                    break;
                }
                tasks.push_back(std::make_pair(task.second, gap_instance->m_items[task.second - 1].m_workigtime));
            }
            if (charging_id == -1)
                continue;
            const int charging_time = gap_instance->m_chargings[charging_id - 1].m_time;

            createScheduleOptionsForRobot(tasks, m_gap_instance->m_bins[i].m_id, m_gap_instance->constaint_time, charging_time);
        }
        displayScheduleOptions();
    }

    std::map<int, std::vector<std::vector<std::pair<std::string, int>>>> SchedulePlanner::generateTaskGroups(
        const std::vector<std::pair<int, int>> &tasks)
    {
        std::map<int, std::vector<std::vector<std::pair<std::string, int>>>> groupedSubsets;

        size_t n = tasks.size();
        for (size_t subset = 0; subset < (1 << n); ++subset)
        {
            std::vector<std::pair<std::string, int>> currentSubset;
            int sum = 0;

            for (size_t i = 0; i < n; ++i)
            {
                if (subset & (1 << i))
                {
                    currentSubset.push_back(std::make_pair("task", tasks[i].first)); // タスクのIDを追加
                    sum += tasks[i].second;                                          // 実行時間を加算
                }
            }

            // 実行時間の合計でグループ化
            groupedSubsets[sum].push_back(currentSubset);
        }

        return groupedSubsets;
    }

    void SchedulePlanner::createScheduleOptionsForRobot(const std::vector<std::pair<int, int>> &tasks, const int robot_id, const int constant_time, const int charging_time)
    {
        // タスクIDに基づいてスケジュールの選択肢を生成
        std::map<int, std::vector<std::vector<std::pair<std::string, int>>>> taskGroups = generateTaskGroups(tasks);

        bool replayCheck = true;

        for (const auto &group : taskGroups)
        {
            ScheduleOption scheduleOption;
            const int initialEnergy = m_gap_instance->m_bins[robot_id - 1].m_initial_size;
            if (replayCheck)
            {
                const bool overCharging = isOvercharging(initialEnergy - (group.first * m_gap_instance->m_bins[robot_id - 1].m_energy_efficiency), charging_time, robot_id);
                if (overCharging)
                {
                    continue;
                }
                else
                {
                    replayCheck = false;
                }
            }

            std::vector<int> binary_option = std::vector<int>(constant_time, 0);

            for (int i = group.first; i < group.first + charging_time; i++)
            {
                binary_option[i] = 1;
            }
            scheduleOption.binary_option = binary_option;
            scheduleOption.robot_id = robot_id;
            scheduleOption.m_assignment = group.second[0]; // 1つ目の部分集合を設定
            scheduleOptions.push_back(scheduleOption);
        }
    }

    void SchedulePlanner::displayScheduleOptions() const
    {
        for (const auto &scheduleOption : scheduleOptions)
        {
            std::cout << "Robot ID: " << scheduleOption.robot_id << std::endl;
            std::cout << "Binary Option: ";
            for (const auto &option : scheduleOption.binary_option)
            {
                std::cout << option << " ";
            }
            std::cout << std::endl;
            std::cout << "Assignment: ";
            for (const auto &assignment : scheduleOption.m_assignment)
            {
                std::cout << "(" << assignment.first << ", " << assignment.second << ") ";
            }
            std::cout << std::endl;
        }
    }

    // robotの最大容量を超過する場はtrueを返す
    bool SchedulePlanner::isOvercharging(const int reset_energy, const int charging_time, const int robot_id)
    {
        const int charge_energy = m_gap_instance->GetMinChargeEfficiency() * charging_time;
        if (reset_energy + charge_energy > m_gap_instance->m_bins[robot_id - 1].m_max_size)
            return true;
        return false;
    }

}
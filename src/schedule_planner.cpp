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
        int selectedOptionIndex = 0;

        for (const auto &robot : m_gap_instance->m_bins)
        {
            std::vector<std::pair<int, int>> tasks;

            int charging_id = -1;

            for (const auto &task : robot.m_assignment)
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

            const std::pair<bool, std::vector<ScheduleOption>> result = createScheduleOptionsForRobot(tasks, robot.m_id, m_gap_instance->constaint_time, charging_time);
            if (result.first)
            {
                m_schedule_options.push_back(std::vector<ScheduleOption>());
                m_schedule_options[selectedOptionIndex] = result.second;
                selectedOptionIndex++;
            }
        }
        displayScheduleOptions();
        solve(m_gap_instance->m_stations[0].m_capacity, m_gap_instance->constaint_time);
        setSelectedOptionForRobot();
        // displayPreScheduleOptions();
        // displaySelectedOption();
    }

    /**
     * generateTaskGroups
     * @brief タスクのグループ化
     * @param tasks タスクのリスト
     * @return std::map<int, std::vector<std::vector<std::pair<std::string, int>>>> グループ化されたタスクのリスト
     * example: {1: [[(task, 1)], [(task, 2)], [(task, 3)]], 2: [[(task, 1), (task, 2)], [(task, 1), (task, 3)], [(task, 2), (task, 3)]],...}
     */
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

    std::pair<bool, std::vector<ScheduleOption>> SchedulePlanner::createScheduleOptionsForRobot(const std::vector<std::pair<int, int>> &tasks, const int robot_id, const int constant_time, const int charging_time)
    {
        // タスクIDに基づいてスケジュールの選択肢を生成
        std::map<int, std::vector<std::vector<std::pair<std::string, int>>>> taskGroups = generateTaskGroups(tasks);
        std::vector<ScheduleOption> scheduleOptionsForRobot;
        int selectedGroupTasksIndex = -1;

        for (const auto &group : taskGroups)
        {
            ScheduleOption scheduleOption;
            const int initialEnergy = m_gap_instance->m_bins[robot_id - 1].m_initial_size;
            for (int i = 0; i < group.second.size(); i++)
            {
                // 消費エネルギーを計算
                int totalEnergy = 0;
                for (const auto &task : group.second[i])
                {
                    totalEnergy += m_gap_instance->m_sizematrix[task.second - 1][robot_id - 1];
                }
                const bool overCharging = isOvercharging(initialEnergy - totalEnergy, charging_time, robot_id);
                if (overCharging)
                {
                    cout << "Robot " << robot_id << "group " << i << " is overcharging" << endl;
                    continue;
                }
                else
                {
                    selectedGroupTasksIndex = i;
                    break;
                }
            }

            std::vector<int> binary_option = std::vector<int>(constant_time, 0);

            for (int i = group.first; i < group.first + charging_time; i++)
            {
                binary_option[i] = 1;
            }
            scheduleOption.m_binary_option = binary_option;
            scheduleOption.m_robot_id = robot_id;
            scheduleOption.m_assignment = group.second[(selectedGroupTasksIndex != -1) ? selectedGroupTasksIndex : 0];
            scheduleOptionsForRobot.push_back(scheduleOption);
        }

        return {
            selectedGroupTasksIndex != -1, scheduleOptionsForRobot};
    }

    void SchedulePlanner::displayScheduleOptions() const
    {

        for (int i = 0; i < m_schedule_options.size(); i++)
        {
            std::cout << "Robot " << m_schedule_options[i][0].m_robot_id << std::endl;
            for (int j = 0; j < m_schedule_options[i].size(); j++)
            {
                std::cout << "Option " << j << " - ";
                for (auto &task : m_schedule_options[i][j].m_assignment)
                {
                    std::cout << "(" << task.first << ", " << task.second << ") ";
                }
                std::cout << std::endl;
                std::cout << "Binary Option: ";
                for (auto &option : m_schedule_options[i][j].m_binary_option)
                {
                    std::cout << option << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }

    // robotの最大容量を超過する場はtrueを返す
    bool SchedulePlanner::isOvercharging(const int rest_energy, const int charging_time, const int robot_id)
    {
        const int charge_energy = m_gap_instance->GetMinChargeEfficiency() * charging_time;
        if (rest_energy + charge_energy > m_gap_instance->m_bins[robot_id - 1].m_max_size)
            return true;
        return false;
    }

    // 充電ステーションの競合数を計算
    int SchedulePlanner::calculateOverflow(const vector<int> &charge, int stationCapacity)
    {
        int overflow = 0;
        for (int c : charge)
        {
            if (c > stationCapacity)
            {
                overflow += c - stationCapacity;
            }
        }
        return overflow;
    }

    // DPで問題を解く関数
    void SchedulePlanner::solve(int stationCapacity, int timeSteps)
    {
        int toDecideNum = m_schedule_options.size();

        // DPテーブルと遷移記録用
        map<vector<int>, int> dpPrev, dpNext;           // 状態 -> 最小競合数
        map<vector<int>, pair<int, vector<int>>> trace; // 状態 -> (選択肢, 遷移元)

        // 初期状態
        vector<int> initialCharge(timeSteps, 0);
        dpPrev[initialCharge] = 0;

        // DP遷移
        for (int i = 0; i < toDecideNum; ++i)
        {
            dpNext.clear();
            for (auto &[currentCharge, currentOverflow] : dpPrev)
            {
                for (int k = 0; k < m_schedule_options[i].size(); ++k)
                {
                    const auto &option = m_schedule_options[i][k];

                    // 次の充電状態を計算
                    vector<int> nextCharge = currentCharge;
                    for (int t = 0; t < timeSteps; ++t)
                    {
                        nextCharge[t] += option.m_binary_option[t];
                    }

                    // 競合数を計算
                    int overflow = calculateOverflow(nextCharge, stationCapacity);

                    // DPテーブルを更新
                    if (dpNext.find(nextCharge) == dpNext.end() || dpNext[nextCharge] > overflow)
                    {
                        dpNext[nextCharge] = overflow;
                        trace[nextCharge] = {k, currentCharge};
                    }
                }
            }
            dpPrev = dpNext;
        }

        // 最小競合数の探索
        int minOverflow = 100000;
        vector<int> bestCharge;
        for (auto &[charge, overflow] : dpPrev)
        {
            if (overflow < minOverflow)
            {
                minOverflow = overflow;
                bestCharge = charge;
            }
        }

        // 結果の復元
        vector<int> charge = bestCharge;
        m_selected_index_array = vector<int>(toDecideNum);
        for (int i = toDecideNum - 1; i >= 0; --i)
        {
            m_selected_index_array[i] = trace[charge].first;
            charge = trace[charge].second;
        }

        // 結果の出力
        cout << "Minimum Overflow: " << minOverflow << endl;
        for (int i = 0; i < toDecideNum; ++i)
        {
            cout << "Robot " << m_schedule_options[i][m_selected_index_array[i]].m_robot_id << ": Option " << m_selected_index_array[i] << " - ";
            for (auto &task : m_schedule_options[i][m_selected_index_array[i]].m_assignment)
            {
                cout << "(" << task.first << ", " << task.second << ") ";
            }
            cout << endl;
        }
    }

    void SchedulePlanner::setSelectedOptionForRobot()
    {
        // scheduleOptionのm_assignmentをm_gap_instanceのm_binsに反映
        // ただし、scheduleOptionのm_assignmentにはchargingとそれ以降のタスクがふくまれていないため、robotのm_assignmentを参照して反映する
        m_occupation_all = std::vector<int>(m_gap_instance->constaint_time, 0);

        for (int i = 0; i < m_gap_instance->m_bins.size(); i++)
        {
            auto &robot = m_gap_instance->m_bins[i];
            ScheduleOption selectedOption;

            int optionsIndex = -1;
            int selectedOptionIndex = -1;
            // robot.m_scheduled_assignment /
            for (int j = 0; j < m_schedule_options.size(); j++)
            {

                if (m_schedule_options[j][0].m_robot_id == robot.m_id)
                {
                    optionsIndex = j;
                    selectedOptionIndex = m_selected_index_array[j];
                    break;
                }
            }
            if (selectedOptionIndex == -1 || optionsIndex == -1)
            {
                selectedOption.m_binary_option = std::vector<int>(m_gap_instance->constaint_time, 0);
                selectedOption.m_robot_id = robot.m_id;
                selectedOption.m_assignment = robot.m_assignment;
                selectedOption.m_total_time = robot.m_total_time;
                robot.m_scheduled_assignment = selectedOption;
                continue;
            }
            // 充電タスクが存在する場合
            int chargingId = -1;
            for (int j = 0; j < robot.m_assignment.size(); j++)
            {
                if (robot.m_assignment[j].first == "charging")
                {
                    chargingId = robot.m_assignment[j].second;
                    break;
                }
            }

            selectedOption = m_schedule_options[optionsIndex][selectedOptionIndex];
            selectedOption.m_total_time = robot.m_total_time;
            selectedOption.m_assignment.push_back(std::make_pair("charging", m_gap_instance->m_chargings[chargingId - 1].m_id));
            for (int j = 0; j < robot.m_assignment.size(); j++)
            {
                if (robot.m_assignment[j].first == "task") // m_assignmentのidが一致しない場合に追加
                {
                    selectedOption.addAssignmentIfNotExists("task", robot.m_assignment[j].second, m_schedule_options[optionsIndex][selectedOptionIndex].m_assignment, selectedOption.m_assignment);
                }
            }
            assignChargingToStation(selectedOption, chargingId);
            robot.m_scheduled_assignment = selectedOption;
        }
    }

    // 充電ステーションが一つだけであることを前提としている
    void SchedulePlanner::assignChargingToStation(const ScheduleOption &selected_option, const int charging_id)
    {
        std::vector<int> station_occupation = std::vector<int>(m_gap_instance->constaint_time, 0);
        for (int j = 0; j < m_gap_instance->constaint_time; j++)
        {
            m_occupation_all[j] += selected_option.m_binary_option[j];
            station_occupation[j] = m_gap_instance->m_stations[0].m_occupation_per_time[j] + selected_option.m_binary_option[j];
        }
        // m_occupation_all[j]の最大値を算出する
        int size = *std::max_element(station_occupation.begin(), station_occupation.end());
        if (size > m_gap_instance->m_stations[0].m_capacity)
            return;

        m_gap_instance->m_stations[0].m_occupation_per_time = station_occupation;
        m_gap_instance->m_chargings[charging_id - 1].m_assigned_station_id = m_gap_instance->m_stations[0].m_id;
    }

    void SchedulePlanner::displayPreScheduleOptions() const
    {
        cout << "Pre Schedule Options for Robot " << endl;
        for (int i = 0; i < m_gap_instance->m_bins.size(); i++)
        {
            auto &robot = m_gap_instance->m_bins[i];
            cout << "Robot " << robot.m_id << " executes tasks:" << endl;
            robot.displayAssignments();
        }
        cout << endl;
    }

    void SchedulePlanner::displaySelectedOption() const
    {
        cout << "Selected Options for Robot " << endl;
        for (int i = 0; i < m_gap_instance->m_bins.size(); i++)
        {
            auto &robot = m_gap_instance->m_bins[i];
            cout << "Robot " << robot.m_id << " executes tasks:" << endl;
            robot.m_scheduled_assignment.displayAssignments();
        }
        cout << endl;
    }
}
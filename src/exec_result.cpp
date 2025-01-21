#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <sstream>
#include <exec_result.h>
#include <algorithm>
#include <schedule_planner.h>
#include <gap.h>

namespace exec_result
{
    ExecResult::ExecResult()
    {
    }
    ExecResult::ExecResult(std::string test_id, int total_tasks, int total_robots, int constraint_time, int guaranteed_energy, int station_capacity,
                           double gap_for_constraint_size_time, double gap_for_constraint_time, double schedule_planner_time, double exec_time, std::vector<int> initial_occupation, std::vector<int> schedule_occupation, gap::CGap *gap_instance)
        : m_test_id(test_id), m_total_tasks(total_tasks), m_total_robots(total_robots), m_constraint_time(constraint_time), m_guaranteed_energy(guaranteed_energy), m_station_capacity(station_capacity),
          m_gap_for_constraint_size_time(gap_for_constraint_size_time), m_gap_for_constraint_time(gap_for_constraint_time), m_schedule_planner_time(schedule_planner_time), m_exec_time(exec_time),
          m_initial_occupation(initial_occupation), m_schedule_occupation(schedule_occupation)
    {
        calculateStationOccupation(initial_occupation, m_initial_result);
        calculateStationOccupation(schedule_occupation, m_scheduled_result);

        int idleTime = gap_instance->m_bins.size() * gap_instance->constaint_time;
        // 充電がステーションに割り当てられていないものを含めてカウントするために使用する

        gap::CGap gap_copy = *gap_instance; // コピーコンストラクタを使う
        gap::CGap *gap_normal = &gap_copy;  // コピーしたオブジェクトのアドレスを渡す

        // 割り当てれなかった充電とそれによって割り当てれなくなったタスクを削除
        calculateTaskSizeAndValue(*gap_normal);

        for (int i = 0; i < gap_instance->m_bins.size(); i++)
        {
            const int initialAssignmentSize = gap_instance->m_bins[i].m_initial_assignment.m_assignment.size();
            const int scheduledAssignmentSize = gap_instance->m_bins[i].m_scheduled_assignment.m_assignment.size();
            gap_instance->m_bins[i].m_initial_assignment.removeChargeAssignment();
            const int initialTaskSize = gap_instance->m_bins[i].m_initial_assignment.m_assignment.size();
            gap_instance->m_bins[i].m_scheduled_assignment.removeChargeAssignment();
            const int scheduledTaskSize = gap_instance->m_bins[i].m_scheduled_assignment.m_assignment.size();

            if (initialTaskSize != scheduledTaskSize)
            {
                // タスク数が異なる場合エラー
                cerr << "Error: Initial assignment and scheduled assignment are different." << endl;
                exit(1);
            }
            // 割り当てられたタスク数をカウント
            m_assigned_tasks += scheduledTaskSize;
            if (gap_instance->m_bins[i].m_scheduled_assignment.m_total_time == gap_instance->m_bins[i].m_initial_assignment.m_total_time)
            {
                idleTime -= gap_instance->m_bins[i].m_scheduled_assignment.m_total_time;
            }
            else
            {
                // ロボットの総稼働時間が異なる場合エラー
                cerr << "Error: Total time is different." << endl;
            }

            if (gap_instance->m_bins[i].m_scheduled_assignment.m_rest_energy < gap_instance->m_guaranteed_energy)
            {
                // error: 最小充電残量が閾値を超えている場合
                cerr << "Error: Poor charge." << endl;
                exit(1);
            }
        }

        if (idleTime != 0)
        {
            // 充電またはタスクを行わないロボットが存在する場合エラー
            cerr << "Error: Idle time is not 0." << endl;
            exit(1);
        }

        outputResultsToFile("output.csv", test_id);
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

    // Function to check if an ID exists in the file
    bool ExecResult::isTestIdExists(const std::string &filename, const std::string &test_id)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            return false; // File doesn't exist
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string existing_id;
            if (std::getline(iss, existing_id, ','))
            {
                if (existing_id == test_id)
                {
                    return true; // Match found
                }
            }
        }

        file.close();
        return false; // No match found
    }

    void ExecResult::calculateTaskSizeAndValue(gap::CGap &gap_instance)
    {
        // create copy of gap_instance
        gap::CGap gap_copy = gap_instance;
        const gap::CGap *gap_normal = &gap_copy;

        for (int i = 0; i < gap_normal->m_bins.size(); i++)
        {
            gap::CBin &bin = gap_instance.m_bins[i];

            bin.m_initial_assignment.resetAssignment();
            for (int j = 0; j < gap_normal->m_bins[i].m_initial_assignment.m_assignment.size(); j++)
            {
                if (gap_normal->m_bins[i].m_initial_assignment.m_assignment[j].first == "task")
                {
                    // task
                    bool result = bin.m_initial_assignment.addAssignmentWithCheck(gap_normal->m_bins[i].m_initial_assignment.m_assignment[j].first, gap_normal->m_bins[i].m_initial_assignment.m_assignment[j].second,
                                                                                  gap_normal->m_timematrix[gap_normal->m_bins[i].m_initial_assignment.m_assignment[j].second - 1][gap_normal->m_bins[i].m_id - 1],
                                                                                  gap_normal->m_sizematrix[gap_normal->m_bins[i].m_initial_assignment.m_assignment[j].second - 1][gap_normal->m_bins[i].m_id - 1]);
                    if (result)
                    {
                        // アイテムの評価値を計算
                        m_initial_task_value += gap_instance.m_profitmatrix[gap_normal->m_bins[i].m_initial_assignment.m_assignment[j].second - 1][gap_normal->m_bins[i].m_id - 1];
                        // 総数をカウント

                        m_initial_task_size++;
                    }
                }
                else if (gap_normal->m_bins[i].m_initial_assignment.m_assignment[j].first == "charging")
                {
                    // charging
                    cout << "Charging time is over the station capacity Id, " << gap_normal->m_bins[i].m_initial_assignment.m_assignment[j].second << ", " << gap_normal->m_chargings[gap_normal->m_bins[i].m_initial_assignment.m_assignment[j].second - 1].m_time << endl;
                    bin.m_initial_assignment.addAssignmentWithCheck(gap_normal->m_bins[i].m_initial_assignment.m_assignment[j].first, gap_normal->m_bins[i].m_initial_assignment.m_assignment[j].second,
                                                                    gap_normal->m_chargings[gap_normal->m_bins[i].m_initial_assignment.m_assignment[j].second - 1].m_time,
                                                                    gap_instance.GetMinChargeEfficiency() * gap_normal->m_chargings[gap_normal->m_bins[i].m_initial_assignment.m_assignment[j].second - 1].m_time);

                    if (gap_normal->m_chargings[gap_normal->m_bins[i].m_initial_assignment.m_assignment[j].second - 1].m_time > 0)
                    {
                        m_initial_charging_average++;
                    }
                }
            }

            // schedule assignment も同様に計算
            bin.m_scheduled_assignment.resetAssignment();
            for (int j = 0; j < gap_normal->m_bins[i].m_scheduled_assignment.m_assignment.size(); j++)
            {
                if (gap_normal->m_bins[i].m_scheduled_assignment.m_assignment[j].first == "task")
                {
                    // task
                    bool result = bin.m_scheduled_assignment.addAssignmentWithCheck(gap_normal->m_bins[i].m_scheduled_assignment.m_assignment[j].first, gap_normal->m_bins[i].m_scheduled_assignment.m_assignment[j].second,
                                                                                    gap_normal->m_timematrix[gap_normal->m_bins[i].m_scheduled_assignment.m_assignment[j].second - 1][gap_normal->m_bins[i].m_id - 1],
                                                                                    gap_normal->m_sizematrix[gap_normal->m_bins[i].m_scheduled_assignment.m_assignment[j].second - 1][gap_normal->m_bins[i].m_id - 1]);
                    if (result)
                    {
                        // アイテムの評価値を計算
                        m_scheduled_task_value += gap_instance.m_profitmatrix[gap_normal->m_bins[i].m_scheduled_assignment.m_assignment[j].second - 1][gap_normal->m_bins[i].m_id - 1];
                        // 総数をカウント
                        m_scheduled_task_size++;
                    }
                }
                else if (gap_normal->m_bins[i].m_scheduled_assignment.m_assignment[j].first == "charging")
                {
                    // charging
                    bin.m_scheduled_assignment.addAssignmentWithCheck(gap_normal->m_bins[i].m_scheduled_assignment.m_assignment[j].first, gap_normal->m_bins[i].m_scheduled_assignment.m_assignment[j].second,
                                                                      gap_normal->m_chargings[gap_normal->m_bins[i].m_scheduled_assignment.m_assignment[j].second - 1].m_time,
                                                                      gap_instance.GetMinChargeEfficiency() * gap_normal->m_chargings[gap_normal->m_bins[i].m_scheduled_assignment.m_assignment[j].second - 1].m_time);
                    if (gap_normal->m_chargings[gap_normal->m_bins[i].m_scheduled_assignment.m_assignment[j].second - 1].m_time > 0)
                    {
                        m_scheduled_charging_average++;
                    }
                }
            }

            m_initial_rest_energy_average += gap_instance.m_bins[i].m_initial_assignment.m_rest_energy;
            m_scheduled_rest_energy_average += gap_instance.m_bins[i].m_scheduled_assignment.m_rest_energy;
        }

        m_initial_rest_energy_average /= gap_instance.m_bins.size();
        m_scheduled_rest_energy_average /= gap_instance.m_bins.size();
        m_initial_charging_average /= gap_instance.m_bins.size();
        m_scheduled_charging_average /= gap_instance.m_bins.size();
    }

    void ExecResult::outputResultsToFile(
        const std::string &filename, const std::string &test_id)
    {
        if (isTestIdExists(filename, test_id))
        {
            std::cerr << "Test ID " << test_id << " already exists in the file. Aborting write." << std::endl;
            return;
        }
        std::ifstream file_in(filename);
        bool file_exists = file_in.good();
        file_in.close();

        std::ofstream file(filename, std::ios::app); // Append mode
        file << "\xEF\xBB\xBF";                      // Write UTF-8 BOM
        if (!file.is_open())
        {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }

        if (!file_exists)
        {
            // Write headers in Japanese if the file does not exist
            file << "テストID,"
                 << "タスクの総数,"
                 << "ロボットの総数,"
                 << "指定した実行時間,"
                 << "終了時の充電残量,"
                 << "ステーション容量,"
                 << "Fase1の実行時間,"
                 << "Fase2の実行時間,"
                 << "充電計画の実行時間,"
                 << "総実行時間,";
            for (int i = 0; i < m_initial_occupation.size(); i++)
            {
                file << "Fase1: 時間ごとの収容状況,";
            }
            for (int i = 0; i < m_schedule_occupation.size(); i++)
            {
                file << "Fase2: 時間ごとの収容状況,";
            }
            file << "Fase1: 競合数,"
                 << "Fase1: 標準偏差,"
                 << "Fase1: 評価値,"
                 << "Fase1: 平均収容率,"
                 << "Fase1: 最大収容数,"
                 << "Fase2: 競合数,"
                 << "Fase2: 標準偏差,"
                 << "Fase2: 評価値,"
                 << "Fase2: 平均収容率,"
                 << "Fase2: 最大収容数,"
                 << "割り当てタスク数,"
                 << "計画前の平均充電回数,"
                 << "計画後の平均充電回数,"
                 << "計画前の充電残量平均,"
                 << "計画後の充電残量平均,"
                 << "計画前のタスク数,"
                 << "計画後のタスク数,"
                 << "計画前のタスクの価値,"
                 << "計画後のタスクの価値,"
                 << std::endl;
        }

        // Write data to file
        file << test_id << "," << m_total_tasks << ","
             << m_total_robots << "," << m_constraint_time << ","
             << m_guaranteed_energy << "," << m_station_capacity << ","
             << m_gap_for_constraint_size_time << "," << m_gap_for_constraint_time << ","
             << m_schedule_planner_time << "," << m_exec_time << ",";

        for (const auto &occ : m_initial_occupation)
            file << occ << ",";
        for (const auto &occ : m_schedule_occupation)
            file << occ << ",";

        file << std::get<0>(m_initial_result) << ","
             << std::get<1>(m_initial_result) << ","
             << std::get<2>(m_initial_result) << ","
             << std::get<3>(m_initial_result) << ","
             << std::get<4>(m_initial_result) << ","
             << std::get<0>(m_scheduled_result) << ","
             << std::get<1>(m_scheduled_result) << ","
             << std::get<2>(m_scheduled_result) << ","
             << std::get<3>(m_scheduled_result) << ","
             << std::get<4>(m_scheduled_result) << ","
             << m_assigned_tasks << "," << m_initial_charging_average << ","
             << m_scheduled_charging_average << "," << m_initial_rest_energy_average << ","
             << m_scheduled_rest_energy_average << ","
             << m_initial_task_size << "," << m_scheduled_task_size << ","
             << m_initial_task_value << "," << m_scheduled_task_value << ","
             << std::endl;

        file.close();
        std::cout << "Results written to " << filename << std::endl;
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

        std::cout << "Assigned tasks: " << m_assigned_tasks << std::endl;
        std::cout << "Initial charging average: " << m_initial_charging_average << std::endl;
        std::cout << "Scheduled charging average: " << m_scheduled_charging_average << std::endl;
        std::cout << "Initial rest energy average: " << m_initial_rest_energy_average << std::endl;
        std::cout << "Scheduled rest energy average: " << m_scheduled_rest_energy_average << std::endl;
        std::cout << "Initial task size: " << m_initial_task_size << std::endl;
        std::cout << "Scheduled task size: " << m_scheduled_task_size << std::endl;
        std::cout << "Initial task value: " << m_initial_task_value << std::endl;
        std::cout << "Scheduled task value: " << m_scheduled_task_value << std::endl;
    }
}
#include "schedule_planner.h"
#include "schedule_option.h"
#include "gap.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <map>
#include <tuple>
using namespace std;

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
        // robotごとのスケジュールの選択肢
        std::map<int, std::vector<std::vector<int>>> binaryOptions;

        // ロボットごとにスケジュールの選択肢を生成
        for (const auto &robot : m_gap_instance->m_bins)
        {
            // task: <int: タスクのID, int: タスクの消費エネルギー>
            std::vector<int> taskIds;

            int chargingId = -1;

            for (const auto &task : robot.m_initial_assignment.m_assignment)
            {
                if (task.first == "charging")
                {
                    chargingId = task.second;
                }
                taskIds.push_back(task.second);
            }
            std::vector<int> currentBinaryOption(gap_instance->constaint_time, 0); // 現在のオプション
            std::vector<std::vector<int>> binaryOptionsForRobot;                   // 全オプション

            const int chargingTime = gap_instance->m_chargings[chargingId - 1].m_time;

            // 充電パターン生成
            generateBinaryOptions(0, chargingTime, 0, 0, robot.m_initial_size, robot.m_max_size, robot.m_id, currentBinaryOption, binaryOptionsForRobot, taskIds);
            cout << "Binary Options: " << binaryOptionsForRobot.size() << endl;
            binaryOptions[robot.m_id - 1] = binaryOptionsForRobot;
        }

        solve(m_gap_instance->m_stations[0].m_capacity, m_gap_instance->constaint_time, binaryOptions);
        setAssignment();
    }

    double roundToDecimalPlaces(double value, int decimalPlaces)
    {
        double factor = pow(10, decimalPlaces); // 10の指定桁数乗を計算
        return round(value * factor) / factor;  // 四捨五入して元のスケールに戻す
    }

    /**
     * calculateValue
     * @brief 競合数と標準偏差を計算, 評価値を算出
     * @return std::tuple<int, double, double> 競合数, 標準偏差, 評価値
     */
    std::tuple<int, double, double> SchedulePlanner::calculateValue(const std::vector<int> &charge, int stationCapacity)
    {
        // 評価値の計算
        // 競合数 * 重み + 標準偏差 * 重み で評価値を算出
        int value = 0;
        int overflow = 0;
        double variance = 0;
        double standardDeviation = 0;
        int weightOverflow = 10;
        int weightStandardDeviation = 1;
        int sum = 0;
        for (int c : charge)
        {
            sum += c;
        }
        double mean = sum / charge.size();

        for (int c : charge)
        {
            if (c > stationCapacity)
            {
                // 競合数の計算
                overflow += c - stationCapacity;
            }
            // 分散の計算
            variance += (c - mean) * (c - mean);
        }
        standardDeviation = roundToDecimalPlaces(sqrt(variance / charge.size()), 2);

        return {
            overflow,
            standardDeviation,
            weightOverflow * overflow + weightStandardDeviation * standardDeviation,
        };
    }

    // スロットに充電を割り当てるパターンを生成
    void SchedulePlanner::generateBinaryOptions(
        int timeIndex, int remainingCharges, int remainingExecTaskTime, int currentTaskIndex, int restEnergy, int maxEnergy, int robotId,
        std::vector<int> &currentBinaryOption,
        std::vector<std::vector<int>> &binaryOptions, const std::vector<int> &taskIds)
    {

        int totalSlots = currentBinaryOption.size();

        // validation check
        if (restEnergy < 0 || restEnergy > maxEnergy)
        {
            return;
        }

        // 充電がすべて配置された場合
        if (remainingCharges == 0)
        {
            binaryOptions.push_back(currentBinaryOption);
            return;
        }

        // スロットが残っていない場合
        if (timeIndex >= totalSlots)
        {
            return;
        }

        // taskが実行中の場合
        if (remainingExecTaskTime > 0)
        {
            currentBinaryOption[timeIndex] = 0;
            generateBinaryOptions(timeIndex + 1, remainingCharges, remainingExecTaskTime - 1, currentTaskIndex, restEnergy, maxEnergy, robotId, currentBinaryOption, binaryOptions, taskIds);
            return;
        }

        // taskを実行する場合
        if (currentTaskIndex < taskIds.size())
        {
            currentBinaryOption[timeIndex] = 0;
            generateBinaryOptions(timeIndex + 1, remainingCharges, m_gap_instance->m_timematrix[taskIds[currentTaskIndex] - 1][robotId - 1] - 1, currentTaskIndex + 1, restEnergy - m_gap_instance->m_sizematrix[taskIds[currentTaskIndex] - 1][robotId - 1], maxEnergy, robotId, currentBinaryOption, binaryOptions, taskIds);
        }

        // 充電を配置する場合
        currentBinaryOption[timeIndex] = 1;
        generateBinaryOptions(timeIndex + 1, remainingCharges - 1, remainingExecTaskTime, currentTaskIndex, restEnergy + m_gap_instance->GetMinChargeEfficiency(), maxEnergy, robotId, currentBinaryOption, binaryOptions, taskIds);
        currentBinaryOption[timeIndex] = 0; // 元に戻す
    }

    // DPで問題を解く関数
    void SchedulePlanner::solve(int stationCapacity, int timeSteps, std::map<int, std::vector<std::vector<int>>> &binaryOptions)
    {
        int robotNum = binaryOptions.size();

        // DPテーブルと遷移記録用
        map<vector<int>, double> dpPrev, dpNext;        // 状態 -> 最小競合数
        map<vector<int>, pair<int, vector<int>>> trace; // 状態 -> (選択肢, 遷移元)

        // 初期状態
        vector<int> initialCharge(timeSteps, 0);
        dpPrev[initialCharge] = 0;

        // DP遷移
        for (int i = 0; i < robotNum; ++i)
        {
            dpNext.clear();
            for (auto &[currentCharge, currentOverflow] : dpPrev)
            {
                for (int k = 0; k < binaryOptions[i].size(); ++k)
                {
                    // 次の充電状態を計算
                    vector<int> nextCharge = currentCharge;
                    for (int t = 0; t < timeSteps; ++t)
                    {
                        nextCharge[t] += binaryOptions[i][k][t];
                    }

                    // 競合数を計算
                    std::tuple<int, double, double> result = calculateValue(nextCharge, stationCapacity);
                    double value = std::get<2>(result);
                    // DPテーブルを更新
                    if (dpNext.find(nextCharge) == dpNext.end() || dpNext[nextCharge] > value)
                    {
                        dpNext[nextCharge] = value;
                        trace[nextCharge] = {k, currentCharge};
                    }
                }
            }

            // ---ヒープを用いて状態を1000件に制限---
            using State = pair<double, vector<int>>; // {評価値, 状態}
            priority_queue<State> maxHeap;           // 最大ヒープ（評価値が大きい順）

            for (auto &[state, value] : dpNext)
            {
                maxHeap.push({value, state}); // 状態をヒープに挿入
                if (maxHeap.size() > 1)
                {
                    maxHeap.pop();
                }
            }

            // ヒープからdpNextを再構築
            dpNext.clear();
            while (!maxHeap.empty())
            {
                auto [value, state] = maxHeap.top();
                maxHeap.pop();
                dpNext[state] = value;
            }
            // ---ヒープを用いて状態を1000件に制限---

            dpPrev = dpNext;
        }

        // 最小競合数の探索
        double minValue = 1000000000;
        vector<int> bestCharge;
        for (auto &[charge, value] : dpPrev)
        {
            if (value < minValue)
            {
                minValue = value;
                bestCharge = charge;
            }
        }

        // 結果の復元
        vector<int> charge = bestCharge;
        m_selected_index_array = vector<int>(robotNum);
        for (int i = robotNum - 1; i >= 0; --i)
        {
            m_selected_index_array[i] = trace[charge].first;
            charge = trace[charge].second;
        }

        // 結果の反映
        for (int i = 0; i < robotNum; i++)
        {
            m_gap_instance->m_bins[i].m_scheduled_assignment.m_binary_option = binaryOptions[i][m_selected_index_array[i]];
        }
    }

    void SchedulePlanner::setAssignment()
    {
        // binary optionをもとにm_scheduled_assignment.m_assignmentを生成
        m_occupation_all = std::vector<int>(m_gap_instance->constaint_time, 0);

        for (int i = 0; i < m_gap_instance->m_bins.size(); i++)
        {
            auto &robot = m_gap_instance->m_bins[i];

            robot.m_scheduled_assignment.m_assignment = robot.m_initial_assignment.m_assignment;

            // 充電タスクの削除
            robot.m_scheduled_assignment.removeChargeAssignment();
            std::vector<std::pair<std::string, int>> assignment = robot.m_scheduled_assignment.m_assignment;
            robot.m_scheduled_assignment.m_assignment.clear();

            // 前提: タスクの順番は最初の割り当てと同じ
            int chargingTime = 0;
            int taskTime = 0;
            int taskIndex = 0;
            vector<int> chargingIds = vector<int>();

            for (int j = 0; j < robot.m_scheduled_assignment.m_binary_option.size(); j++)
            {
                if (robot.m_scheduled_assignment.m_binary_option[j] == 1)
                {
                    chargingTime++;
                }
                else
                {
                    ++taskTime;
                    if (chargingTime > 0)
                    {
                        const int chargingId = m_gap_instance->m_chargings.size() + 1;
                        gap::CCharging charging = gap::CCharging(chargingId, chargingTime, m_gap_instance->GetMinChargeEfficiency(), m_gap_instance->m_bins[i].m_id, -1);
                        m_gap_instance->AddCharging(charging);
                        robot.m_scheduled_assignment.addAssignment("charging", chargingId, chargingTime, chargingTime * m_gap_instance->GetMinChargeEfficiency());
                        chargingIds.push_back(chargingId);
                        chargingTime = 0;
                    }
                    if (taskIndex < assignment.size())
                    {
                        const int taskId = assignment[taskIndex].second;
                        if (taskTime == m_gap_instance->m_timematrix[taskId - 1][robot.m_id - 1])
                        {
                            robot.m_scheduled_assignment.addAssignment("task", taskId, m_gap_instance->m_timematrix[taskId - 1][robot.m_id - 1],
                                                                       m_gap_instance->m_sizematrix[taskId - 1][robot.m_id - 1]);
                            taskIndex++;
                            taskTime = 0;
                        }
                    }
                }
            }
            if (chargingTime > 0)
            {

                const int chargingId = m_gap_instance->m_chargings.size() + 1;
                gap::CCharging charging = gap::CCharging(chargingId, chargingTime, m_gap_instance->GetMinChargeEfficiency(), m_gap_instance->m_bins[i].m_id, -1);
                m_gap_instance->AddCharging(charging);
                robot.m_scheduled_assignment.addAssignment("charging", chargingId, chargingTime, chargingTime * m_gap_instance->GetMinChargeEfficiency());
                chargingIds.push_back(chargingId);
                chargingTime = 0;
            }
            assignChargingToStation(robot.m_scheduled_assignment, chargingIds);
        }
    }

    // 充電ステーションが一つだけであることを前提としている
    void SchedulePlanner::assignChargingToStation(const ScheduleOption &selected_option, const std::vector<int> charging_ids)
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
        for (int j = 0; j < charging_ids.size(); j++)
        {
            m_gap_instance->m_chargings[charging_ids[j] - 1].m_assigned_station_id = m_gap_instance->m_stations[0].m_id;
        }
    }
}
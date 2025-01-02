#include "schedule_option.h"
#include <iostream>
#include <stdexcept> // std::out_of_range
#include <map>
#include <vector>

namespace schedule_option
{
    std::map<int, std::vector<std::vector<std::pair<std::string, int>>>> generateTaskGroups(
        const std::vector<std::pair<int, int>> &tasks)
    {
        std::map<int, std::vector<std::vector<std::pair<std::string, int>>>> groupedSubsets;

        size_t n = tasks.size();
        // すべての部分集合を列挙 (2^n 通り)
        for (size_t subset = 0; subset < (1 << n); ++subset)
        {
            std::vector<std::pair<std::string, int>> currentSubset;
            int sum = 0;

            // 部分集合に基づいてタスクを選択
            for (size_t i = 0; i < n; ++i)
            {
                if (subset & (1 << i))
                {                                                                    // i 番目のタスクを選択
                    currentSubset.push_back(std::make_pair("task", tasks[i].first)); // タスクのIDを追加
                    sum += tasks[i].second;                                          // 実行時間を加算
                }
            }

            // 実行時間の合計でグループ化
            groupedSubsets[sum].push_back(currentSubset);
        }

        return groupedSubsets;
    }

    ScheduleOption ScheduleOption::createScheduleOptions(const std::vector<int> &taskIds, const int robot_id, const int constantTime)
    {
        ScheduleOption scheduleOption;

        // メンバの初期化
        scheduleOption.robot_id = robot_id;
        scheduleOption.id = taskIds.size(); // idはtaskIdsのサイズに基づけるなど、必要に応じて設定

        // タスクIDと実行時間のペアを生成 - これを引数にしてもいいかもしれない
        std::vector<std::pair<int, int>> tasks;
        for (const auto &taskId : taskIds)
        {
            tasks.push_back(std::make_pair(taskId, 1)); // todo: fix 1 to actual task time
        }

        // タスクIDに基づいてスケジュールの選択肢を生成
        std::map<int, std::vector<std::vector<std::pair<std::string, int>>>> taskGroups = generateTaskGroups(
            std::vector<std::pair<int, int>>{});

        // 例: binary_option として部分集合を設定 (後で調整が必要)
        for (const auto &group : taskGroups)
        {
            scheduleOption.m_assignment = group.second[0]; // 例: 1つ目の部分集合を設定
            // 配列定義
            std::vector<int> binary_option = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            scheduleOption.binary_option.push_back(binary_option);

            // constantTime の数分要素を作成、group.first+1から充電タスクが始まる
            // scheduleOption.binary_option.push_back(group.second);
        }

        // スケジュールの割り当て（例）
        // 例: 割り当てを追加

        return scheduleOption;
    }
};

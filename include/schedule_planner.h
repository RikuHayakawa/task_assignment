#ifndef SCHEDULE_PLANNER_H
#define SCHEDULE_PLANNER_H

#include "gap.h"
#include "schedule_option.h"

namespace schedule_planner
{
    class SchedulePlanner
    {
    public:
        std::vector<ScheduleOption> scheduleOptions;
        gap::CGap *m_gap_instance; // コンポジション

    public:
        SchedulePlanner();
        ~SchedulePlanner();
        SchedulePlanner(gap::CGap *gap_instance);
        void displayScheduleOptions() const;

    private:
        void createScheduleOptionsForRobot(const std::vector<std::pair<int, int>> &tasks, const int robot_id, const int constant_time, const int charging_time);
        /**
         * @brief タスクのグループ化
         *
         * @param tasks タスクのリスト
         * @return std::map<int, std::vector<std::vector<std::pair<std::string, int>>>> グループ化されたタスクのリスト
         * example: {1: [[(task, 1)], [(task, 2)], [(task, 3)]], 2: [[(task, 1), (task, 2)], [(task, 1), (task, 3)], [(task, 2), (task, 3)]],...}
         */
        std::map<int, std::vector<std::vector<std::pair<std::string, int>>>> generateTaskGroups(const std::vector<std::pair<int, int>> &tasks);
    };
};
#endif

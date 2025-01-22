#ifndef SCHEDULE_PLANNER_H
#define SCHEDULE_PLANNER_H

#include "gap.h"
#include "schedule_option.h"

namespace schedule_planner
{
    class SchedulePlanner
    {
    public:
        std::vector<int> m_occupation_all;
        std::vector<std::vector<ScheduleOption>> m_schedule_options;
        std::vector<int> m_selected_index_array;
        gap::CGap *m_gap_instance; // コンポジション

    public:
        SchedulePlanner();
        ~SchedulePlanner();
        SchedulePlanner(gap::CGap *gap_instance);
        std::tuple<int, double, double> calculateValue(const std::vector<int> &charge, int stationCapacity);

    private:
        void solve(int stationCapacity, int timeSteps, std::map<int, std::vector<std::vector<int>>> &binaryOptions);
        void setAssignment();
        void assignChargingToStation(const ScheduleOption &selected_option, const std::vector<int> charging_ids);
        void generateBinaryOptions(int timeIndex, int remainingCharges, int remainExecTaskTime, int currentTaskIndex, int restEnergy, int maxEnergy, int robotId,
                                   std::vector<int> &currentBinaryOption,
                                   std::vector<std::vector<int>> &binaryOptions, const std::vector<int> &taskIds);
    };
};
#endif

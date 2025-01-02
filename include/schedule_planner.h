#ifndef SCHEDULE_PLANNER_H
#define SCHEDULE_PLANNER_H

#include "gap.h"

namespace schedule_planner
{
    class SchedulePlanner
    {
    public:
        gap::CGap *m_gap_instance; // コンポジション

    public:
        SchedulePlanner();
        SchedulePlanner(gap::CGap *gap_instance);
        ~SchedulePlanner();
    };
};
#endif

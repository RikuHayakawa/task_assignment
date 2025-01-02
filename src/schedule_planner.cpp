#include "schedule_planner.h"

namespace schedule_planner
{

    SchedulePlanner::SchedulePlanner()
        : m_gap_instance(nullptr)
    {
    }

    SchedulePlanner::SchedulePlanner(gap::CGap *gap_instance)
        : m_gap_instance(gap_instance)
    {
    }

    SchedulePlanner::~SchedulePlanner()
    {
    }
}
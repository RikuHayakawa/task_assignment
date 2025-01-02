#ifndef SCHEDULE_PLANNER_H
#define SCHEDULE_PLANNER_H

namespace schedule_planner
{
    class SchedulePlanner
    {
    public:
        int m_id;
        int m_time;
        int m_assignedbinid;
        int m_assigned_station_id;

    public:
        SchedulePlanner();
        SchedulePlanner(int id, int time, int m_assignedbinid, int m_assigned_station_id);
        ~SchedulePlanner();
    };
};
#endif

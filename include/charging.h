

#ifndef CCHARGING_H
#define CCHARGING_H

namespace gap
{
    class CCharging
    {
    public:
        int m_id;
        int m_time;
        int assigned_robot_id;
        int assigned_station_id;

    public:
        CCharging();
        CCharging(int id, int m_time, int assigned_robot_id, int assigned_station_id);
        virtual ~CCharging();
    };
};
#endif

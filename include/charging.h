

#ifndef CCHARGING_H
#define CCHARGING_H

namespace gap
{
    class CCharging
    {
    public:
        int m_id;
        int m_time;
        int m_charge_energy;
        int m_assignedbinid;
        int m_assigned_station_id;

    public:
        CCharging();
        CCharging(int id, int time, int charge_energy, int assignedbinid, int assigned_station_id);
        virtual ~CCharging();
        void Print();
        void SetAssignedBinId(int binid, int charge_time, int charge_energy);
        void ClearAssignedBinId();
    };
};
#endif

/**
 * @file station.h
 * @brief Definition of the station class
 * 充電ステーションのクラス定義
 * ・充電ステーション一つにつき、一つのロボットを充電することができる
 */

#ifndef STATION_H
#define STATION_H
#include <vector>

namespace gap
{
    class CStation
    {
    public:
        int m_id;
        int m_charge_efficiency;
        int m_occupation_rate;
        int m_overflow;
        std::vector<int> m_occupation_per_time;

    public:
        CStation();
        CStation(int id, int efficiency, int rate, int overflow, std::vector<int> occupation_per_time);
        virtual ~CStation();
        void Print();
    };
};
#endif

/**
 * @file station.h
 * @brief Definition of the station class
 * 充電ステーションのクラス定義
 * ・充電ステーション一つにつき、一つのロボットを充電することができる
 */

#ifndef STATION_H
#define STATION_H

namespace gap
{
    class CStation
    {
    public:
        int m_id;
        int m_charge_efficiency;

    public:
        CStation();
        CStation(int id, int m_charge_efficiency);
        virtual ~CStation();
    };
};
#endif

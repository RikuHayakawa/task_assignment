/**
 * item.h
 * Author: Zhiyang Su
 * Created on 2014-08-10.
 */

#ifndef ITEM_H
#define ITEM_H

namespace gap
{
    class CItem
    {
    public:
        int m_id;
        int m_weight;
        int m_profit;
        int m_workigtime;
        double m_cost; // cost = profit / weight

        int m_assignedbinid;
        int m_necessary_charging_time;

    public:
        CItem();
        CItem(int id, int weight, int profit, int workigtime, int necessary_charging_time);
        virtual ~CItem();
        void UpdateCost();
    };
};

#endif

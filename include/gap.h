/**
 * gap.h
 * Author: Zhiyang Su
 * Created on 2014-08-10.
 */

#ifndef GAP_H
#define GAP_H

#include <vector>
#include "item.h"
#include "bin.h"
#include "station.h"
#include "charging.h"

using namespace std;

/* Definition of GAP:
 * https://en.wikipedia.org/wiki/Generalized_assignment_problem
 */
namespace gap
{
    class CGap
    {
    public:
        vector<vector<int>> m_sizematrix;
        vector<vector<int>> m_profitmatrix;
        vector<vector<int>> m_station_occupancy_matrix;
        vector<CItem> m_items; // todo: knapsack m_itemsと紛らわしいので, m_all_itemsに変更する
        vector<CItem> m_rest_items;
        vector<CBin> m_bins;
        vector<CStation> m_stations;
        vector<CCharging> m_chargings;
        int m_maxprofit;
        int constaint_time;

    public:
        CGap();
        ~CGap();

        void AddItem(CItem &item);
        void AddRestItem(CItem &item);
        void AddBin(CBin &bin);
        void AddStation(CStation &station);
        void AddCharging(CCharging &charging);

        void ApproximateForConstraintSize();
        void ApproximateForConstraintTime();
        void Print();
        void PrintAssignment(); // The result is stored in each item object
        void SetAssignmentForRobots();
    };
};

#endif

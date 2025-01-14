/**
 * test.cpp
 * Author: Zhiyang Su
 * Created on 2014-08-10.
 */

#include "test.h"
#include "item.h"
#include "bin.h"
#include "knapsack.h"
#include "gap.h"
#include "schedule_planner.h"
#include <iostream>
#include <fstream>

using namespace std;

namespace gap
{
    // void TestKnapsack()
    // {
    //     /* Get input data from testcaseforknapsack.txt
    //      * Format:
    //      * --------------------------------------
    //      * item_num bin_size
    //      * item_size1 item_size2 ...
    //      * item_profit1 item_profit2 ...
    //      * --------------------------------------
    //      */
    //     ifstream file("test/testcaseforknapsack.txt");
    //     int casenum = 1, itemnum, binsize;
    //     if (file.is_open())
    //     {
    //         while (file >> itemnum >> binsize)
    //         {
    //             cout << "Case " << casenum << ":" << endl;
    //             CKnapsack knapsack;
    //             CBin bin(1, binsize, binsize, 1, 1);
    //             knapsack.SetBin(bin);
    //             for (int i = 0; i < itemnum; ++i)
    //             {
    //                 int weight;
    //                 file >> weight;
    //                 // CItem item(i + 1, weight, -1);
    //                 // knapsack.AddItem(item);
    //             }
    //             for (int i = 0; i < itemnum; ++i)
    //             {
    //                 int profit;
    //                 file >> profit;
    //                 knapsack.m_items[i].m_profit = profit;
    //             }
    //             knapsack.Print();
    //             knapsack.DpUnderConstraintSize();
    //             cout << "Max Profit:" << knapsack.m_maxprofit << endl;
    //             knapsack.PrintAssignment();
    //             cout << endl;
    //             ++casenum;
    //         }
    //         file.close();
    //     }
    // }

    void TestGap()
    {
        /* Get input data from testcaseforgap.txt
         * Format:
         * --------------------------------------
         * item_num bin_num
         * bin_size1 bin_size2 ...
         *
         * sizematrix (item_num * bin_num)
         *
         * profitmatrix (item_num * bin_num)
         * --------------------------------------
         */
        ifstream file("test/testcaseforgap.txt");
        string test_id;
        int casenum = 1, itemnum, binnum, stationnum;
        if (!file.is_open())
        {
            throw "File not found!";
        }

        while (file >> test_id >> itemnum >> binnum >> stationnum)
        {
            cout << "Test ID: " << test_id << endl;
            cout << "Case " << casenum << ":" << endl;
            CGap gap;
            vector<int> sizes(binnum);
            vector<int> initial_sizes(binnum);
            file >> gap.constaint_time;
            file >> gap.m_guaranteed_energy;
            for (int i = 0; i < binnum; ++i)
            {
                file >> initial_sizes[i];
                sizes[i] = (initial_sizes[i] - gap.m_guaranteed_energy) > 0 ? initial_sizes[i] - gap.m_guaranteed_energy : 0;
                // 充電が必要な場合
                if ((initial_sizes[i] - gap.m_guaranteed_energy) < 0)
                {
                    gap.m_charge_time_for_guaranteed_energy[i + 1] = -(initial_sizes[i] - gap.m_guaranteed_energy);
                }
            }
            vector<int> max_sizes(binnum);
            for (int i = 0; i < binnum; ++i)
            {
                file >> max_sizes[i];
            }
            for (int i = 0; i < binnum; ++i)
            {
                CBin bin(i + 1, sizes[i], max_sizes[i], initial_sizes[i]);
                gap.AddBin(bin);
            }
            for (int i = 0; i < stationnum; ++i)
            {
                int charge_efficiency, capacity;
                file >> charge_efficiency >> capacity;
                CStation station(i + 1, charge_efficiency, capacity, 0, 0, vector<int>(gap.constaint_time, 0));
                gap.AddStation(station);
            }
            for (int i = 0; i < itemnum; ++i)
            {
                vector<int> line(binnum);
                for (int j = 0; j < binnum; ++j)
                    file >> line[j];
                gap.m_timematrix.push_back(line);
            }
            for (int i = 0; i < itemnum; ++i)
            {
                vector<int> line(binnum);
                for (int j = 0; j < binnum; ++j)
                    file >> line[j];
                gap.m_sizematrix.push_back(line);
            }
            for (int i = 0; i < itemnum; ++i)
            {
                vector<int> line(binnum);
                for (int j = 0; j < binnum; ++j)
                    file >> line[j];
                gap.m_profitmatrix.push_back(line);
            }
            for (int i = 0; i < itemnum; ++i)
            {
                CItem item(i + 1, -1, -1, 0);
                item.m_cost = -1;
                gap.AddItem(item);
            }
            // gap.Print();
            cout << endl;
            gap.ApproximateForConstraintSize();
            // bins dispaly assignments
            for (int i = 0; i < gap.m_bins.size(); i++)
            {
                gap.m_bins[i].displayAssignments();
            }

            gap.ApproximateForConstraintTime();
            // bins dispaly assignments
            for (int i = 0; i < gap.m_bins.size(); i++)
            {
                gap.m_bins[i].displayAssignments();
            }

            schedule_planner::SchedulePlanner schedulePlanner(&gap);
            // m_chargings print
            for (int i = 0; i < gap.m_chargings.size(); i++)
            {
                gap.m_chargings[i].Print();
            }
            cout << endl;
            // m_stations print
            for (int i = 0; i < gap.m_stations.size(); i++)
            {
                gap.m_stations[i].Print();
            }
            cout << endl;
            cout << "Occupation all: ";
            for (int i = 0; i < schedulePlanner.m_occupation_all.size(); i++)
            {
                cout << schedulePlanner.m_occupation_all[i] << " ";
            }
            const double value = schedulePlanner.calculateValue(schedulePlanner.m_occupation_all, gap.m_stations[0].m_capacity);
            cout << endl
                 << "Schedule value: " << value << endl;
            ++casenum;
        }
        file.close();
    }
};

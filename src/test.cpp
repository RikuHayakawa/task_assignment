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
#include "timer.h"
#include "exec_result.h"
#include <iostream>
#include <fstream>

using namespace std;

namespace gap
{
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
        ifstream file("test/generatedtest.txt");
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
            map<int, int> m_charge_energy_for_guarantee;

            for (int i = 0; i < binnum; ++i)
            {
                file >> initial_sizes[i];
                sizes[i] = (initial_sizes[i] - gap.m_guaranteed_energy) > 0 ? initial_sizes[i] - gap.m_guaranteed_energy : 0;
                // 充電が必要な場合
                if ((initial_sizes[i] - gap.m_guaranteed_energy) < 0)
                {
                    m_charge_energy_for_guarantee[i + 1] = -(initial_sizes[i] - gap.m_guaranteed_energy);
                }
            }
            vector<int> max_sizes(binnum);
            for (int i = 0; i < binnum; ++i)
            {
                file >> max_sizes[i];
            }
            for (int i = 0; i < stationnum; ++i)
            {
                int charge_efficiency, capacity;
                file >> charge_efficiency >> capacity;
                CStation station(i + 1, charge_efficiency, capacity, 0, 0, vector<int>(gap.constaint_time, 0));
                gap.AddStation(station);
            }
            for (int i = 0; i < binnum; ++i)
            {
                CBin bin(i + 1, sizes[i], max_sizes[i], initial_sizes[i], gap.constaint_time, m_charge_energy_for_guarantee[i + 1] / gap.GetMinChargeEfficiency());
                gap.AddBin(bin);
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
            // timer for gapForConstraintSize
            timer::Timer gapForConstraintSizeTimer;
            gapForConstraintSizeTimer.start();
            gap.ApproximateForConstraintSize();
            gapForConstraintSizeTimer.stop();

            // timer for gapForConstraintTime
            timer::Timer gapForConstraintTimeTimer;
            gapForConstraintTimeTimer.start();
            gap.ApproximateForConstraintTime();
            gapForConstraintTimeTimer.stop();

            timer::Timer schedulePlanTimer;
            schedulePlanTimer.start();
            schedule_planner::SchedulePlanner schedulePlanner(&gap);
            schedulePlanTimer.stop();
            const vector<int> initial_occuption_all = gap.updateInitialAllBinary();
            // m_bins print
            for (int i = 0; i < gap.m_bins.size(); i++)
            {
                gap.m_bins[i].Print();
            }
            // m_chargings print
            for (int i = 0; i < gap.m_chargings.size(); i++)
            {
                gap.m_chargings[i].Print();
            }
            // m_stations print
            for (int i = 0; i < gap.m_stations.size(); i++)
            {
                gap.m_stations[i].Print();
            }
            cout << endl;
            exec_result::ExecResult execResult(test_id, itemnum, binnum, gap.constaint_time, gap.m_guaranteed_energy,
                                               gap.m_stations[0].m_capacity, gapForConstraintSizeTimer.elapsedSeconds(), gapForConstraintTimeTimer.elapsedSeconds(), schedulePlanTimer.elapsedSeconds(),
                                               gapForConstraintSizeTimer.elapsedSeconds() + gapForConstraintTimeTimer.elapsedSeconds() + schedulePlanTimer.elapsedSeconds(), initial_occuption_all, schedulePlanner.m_occupation_all, &gap);
            execResult.Print();

            ++casenum;
        }
        file.close();
    }
};



#include <iostream>
#include <fstream>
#include <vector>

namespace exec_result
{

    class ExecResult
    {
    private:
        std::string m_test_id;                  // 識別子
        int m_total_tasks;                      // 総タスク
        int m_total_robots;                     // 総ロボット数
        int m_constraint_time;                  // 指定した実行時間
        int m_guaranteed_energy;                // 終了時の充電残量
        int m_station_capacity;                 // 充電ステーションの容量
        double m_gap_for_constraint_size_time;  // 段階1のアルゴリズムの実行時間
        double m_gap_for_constraint_time;       // 段階2のアルゴリズムの実行時間
        double m_schedule_planner_time;         // 段階3のアルゴリズム(充電計画)の実行時間
        double m_exec_time;                     // 全体の実行時間
        std::vector<int> m_initial_occupation;  // 初期の収容状況
        std::vector<int> m_schedule_occupation; // スケジュール後の収容状況

        // モデル内で計算
        std::tuple<int, double, double, double, int> m_initial_result;   // 初期結果(競合数, 標準偏差, 評価値, 平均収容率, 最大収容数)
        std::tuple<int, double, double, double, int> m_scheduled_result; // スケジュール結果(競合数, 標準偏差, 評価値, 平均収容率, 最大収容数)
        int m_assigned_tasks;                                            // 割り当てられた総タスク数
        int m_assigned_chargings;                                        // 充電を行うロボット数
        double m_average_idle_time;                                      // 充電またはタスクを行っていない時間の平均
        bool m_is_poor_charge;                                           // 実行終了時の最小充電残量が閾値を超えているかのフラグ
        double m_finishing_average_charge;                               // 実行終了時の平均充電残量
        double m_average_occupation_rate;                                // 平均収容率
        int m_max_capacity;                                              // 最大収容数

        void calculateStationOccupation(std::vector<int> occupation_per_time, std::tuple<int, double, double, double, int> &result);

    public:
        ExecResult();
        ExecResult(std::string test_id, int total_tasks, int total_robots, int constraint_time, int guaranteed_energy, int station_capacity,
                   double gap_for_constraint_size_time, double gap_for_constraint_time, double schedule_planner_time, double exec_time, std::vector<int> initial_occupation, std::vector<int> schedule_occupation);
        virtual ~ExecResult();
        void Print();
    };
};
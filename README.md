Generalized Assignment Problem Solver
Implementation of approximation algorithms for Generalized Assignment Problem (GAP).

Referenece: Cohen, Reuven, Liran Katzir, and Danny Raz. "An efficient approximation for the generalized assignment problem." Information Processing Letters 100.4 (2006): 162-166.

Notes:
1. For simplicity, variables in the classes are not well encapsulated.

ビルド　make 
クリア　make clean

GAP入力例
1. 4 3 2
タスク数　ロボット数　ステーション数

2. 2 3 4
ロボットの初期充電残量

3. 4 4 4
ロボットの最大充電容量

4. 2 2
ステーションの充電効率

5.  1 1 1
    2 3 3
    2 3 4
    1 2 3
各ロボットがタスクを実行する際の消費電力

6.  3 1 5
    1 1 1
    5 15 25
    25 15 5
各ロボットがタスクを実行する際の利益（タスクの優先度）

7.  1 1 1
    2 3 3
    2 3 4
    1 2 3
各ロボットがタスクを実行する時間



定義

タスク：　ロボットが行う作業のこと。割当の際には消費電力とタスクの優先度の２つの項目を用いて評価を行う。
ロボット：　自立移動型ロボット。個体によって初期充電と最大充電量が異なる

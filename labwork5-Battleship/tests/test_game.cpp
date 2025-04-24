#include <gtest/gtest.h>
#include "Strategy.h"
#include "Field.h"

TEST(StrategyTest, CustomTest) {
    Field field;
    Strategy* strategy = nullptr;
    field.SetWidth(5);
    field.SetHeight(5);
    field.SetCount(4, 2);
    strategy = new Custom(field);
    field.CreateGrid(field.grid);
    field.PlaceShips();
    field.CreateGrid(field.enemy_grid);
    ASSERT_EQ(field.enemy_grid[1][1], 'O');
    std::pair<uint64_t, uint64_t> shot = strategy->GetNextShot();
    ASSERT_EQ(shot.first, 0);
    ASSERT_EQ(shot.second, 0);
    shot = strategy->GetNextShot();
    ASSERT_EQ(shot.first, 0);
    ASSERT_EQ(shot.second, 4);
    strategy->SetSHotNumberAfterHit(0);
    shot = strategy->GetNextShot();
    ASSERT_EQ(shot.first, 0);
    ASSERT_EQ(shot.second, 3);
    shot = strategy->GetNextShot();
    ASSERT_EQ(shot.first, 0);
    ASSERT_EQ(shot.second, 2);
    strategy->SetSHotNumberAfterHit();
    shot = strategy->GetNextShot();
    ASSERT_EQ(shot.first, 1);
    ASSERT_EQ(shot.second, 1);
}

TEST(StrategyTest, OrderedTest) {
    Field field;
    Strategy* strategy = nullptr;
    field.SetWidth(5);
    field.SetHeight(5);
    field.SetCount(2, 2);
    strategy = new Ordered(field);
    field.CreateGrid(field.grid);
    field.PlaceShips();
    field.CreateGrid(field.enemy_grid);
    ASSERT_EQ(field.enemy_grid[2][2], 'O');
    std::pair<uint64_t, uint64_t> shot;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            shot = strategy->GetNextShot();
            ASSERT_EQ(shot.first, i);
            ASSERT_EQ(shot.second, j);
        }
    }
}

TEST(FirldTest, LoadTest) {
    Field field;
    Strategy* strategy = nullptr;
    char file_name[1024];
    strncpy(file_name, "C:/Users/MB/c++github/labwork5-maksimbelov1/load_file.txt", sizeof(file_name) - 1);
    field.Load(file_name);
    ASSERT_EQ(field.GetCount(1), 1);
    ASSERT_EQ(field.GetCount(2), 1);
    ASSERT_EQ(field.GetCount(3), 0);
    ASSERT_EQ(field.GetCount(4), 1);
    ASSERT_EQ(field.GetWidth(), 10);
    ASSERT_EQ(field.GetHeight(), 9);
    strategy = new Ordered(field);
    std::pair<uint64_t, uint64_t> shot = strategy->GetNextShot();
    ASSERT_EQ(shot.first, 0);
    ASSERT_EQ(shot.second, 0);
    ASSERT_EQ(strategy->Enemy_shot(0, 0), "kill"); 
    ASSERT_EQ(strategy->Enemy_shot(1, 8), "hit"); 
    ASSERT_EQ(strategy->Enemy_shot(3, 4), "hit"); 
    ASSERT_EQ(strategy->Enemy_shot(4, 4), "kill"); 
    ASSERT_EQ(strategy->Enemy_shot(4, 5), "miss"); 
    ASSERT_FALSE(strategy->IsFinished() || strategy->IsWin() || strategy->IsLose());
}

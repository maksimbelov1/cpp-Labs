#include "Field.h"
#include "Strategy.h"

class Game {
public:
    Game();
    void GameStart();
    bool GameProcess();

    void GetOF(bool status);
    void GetYN(bool status);

private:
std::string command;
std::string shot_result;
uint64_t command_int;
char file_name[1024];
uint32_t ship_size;
Field field;
Strategy* strategy;
std::pair<uint64_t, uint64_t> shot;
bool is_my_shot;
uint64_t X;
uint64_t Y;
bool is_slave;
bool is_strategy_set;
bool is_load;
};
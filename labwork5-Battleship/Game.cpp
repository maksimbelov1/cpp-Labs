#include "Game.h"
#include "Field.h"

Game::Game(): is_my_shot(false), is_slave(false), is_strategy_set(false), strategy(nullptr), is_load(false) {}

void Game::GetOF(bool status) {
    if (!status) {
        std::cout << "failed" << '\n';
    } else {
        std::cout << "ok" << '\n';
    }
}
void Game::GetYN(bool status) {
    if (!status) {
        std::cout << "no" << '\n';
    } else {
        std::cout << "yes" << '\n';
    }
}

void Game::GameStart() {
    while (true) {
        std::cin >> command;
        if (command == "ping") {
            std::cout << "pong" << '\n';
        } if (command == "exit") {
            std::cout << "ok" << '\n';
            return;
        } else if (command == "create") {
            std::cin >> command;
            if (command == "master") {
                GetOF(field.SetWidth(20) && field.SetHeight(20));
                field.SetCount(1, 0);
                field.SetCount(2, 0);
                field.SetCount(3, 0);
                field.SetCount(4, 2);
            } else if (command == "slave") {
                is_slave = true;
                is_my_shot = true;
                std::cout << "ok" << '\n';
            }
        } else if ((command == "set") && is_slave) {
            std::cin >> command;
            if (command == "height") {
                std::cin >> command_int;
                GetOF(field.SetHeight(command_int));
            } else if (command == "width") {
                std::cin >> command_int;
                GetOF(field.SetWidth(command_int));
            } else if (command == "count") {
                std::cin >> ship_size >> command_int;
                GetOF(field.SetCount(ship_size, command_int));
            }
        }  else if (command == "strategy") {
            std::cin >> command;
            if (command == "ordered") {
                strategy = new Ordered(field);
                is_strategy_set = true;
                std::cout << "ok" << '\n';
            } else if (command == "custom") {
                is_strategy_set = true;
                strategy = new Custom(field);
                std::cout << "ok" << '\n';
            }
        } else if (command == "get") {
            std::cin >> command;
            if (command == "width") {
                std::cout << field.GetWidth() << '\n';
            } else if (command == "height") {
                std::cout << field.GetHeight() << '\n';
            } else if (command == "count") {
                std::cin >> command_int;
                std::cout << field.GetCount(command_int) << '\n';
            }
        }
        else if (command == "dump") {
            std::cin >> file_name;
            GetOF(field.Dump(file_name));
        } else if (command == "load") {
            is_load = true;
            is_slave = true;
            is_my_shot = true;
            std::cin >> file_name;
            GetOF(field.Load(file_name)); 
        } else if (command == "start") {
            if (!is_strategy_set) {
                is_strategy_set = true;
                strategy = new Custom(field);
            }
            if (is_load) {
                if (!Game::GameProcess()) {
                    return;
                }
            } else if (field.CreateGrid(field.grid) && field.PlaceShips()) {
                if (field.CreateGrid(field.enemy_grid)) {
                    if (!Game::GameProcess()) {
                        return;
                    }
                }
            }
        }
    }
}

bool Game::GameProcess() {
    std::cout << "ok" << '\n';
    while (true) {
        std::cin >> command;
        if (command == "shot") {
            if (is_my_shot) {
                shot = strategy->GetNextShot();
                std::cout << shot.second << " " << shot.first << '\n';
                std::cin >> command;
                std::cin >> command;
                std::cin >> command;
                if (command == "miss") {
                    is_my_shot = false;
                    std::cout << "ok" << '\n';
                } else if (command == "hit") {
                    field.enemy_grid[shot.first][shot.second] = 'S';
                    strategy->SetSHotNumberAfterHit(0);
                    field.enemy_current_sum_of_decks--;
                    std::cout << "ok" << '\n';
                } else if (command == "kill") {
                    field.enemy_grid[shot.first][shot.second] = 'S';
                    strategy->SetSHotNumberAfterHit();
                    field.enemy_current_sum_of_decks--;
                    std::cout << "ok" << '\n';
                }
            } else {
                std::cin >> X >> Y;
                shot_result = strategy->Enemy_shot(X, Y); 
                std::cout << shot_result << '\n';
                if (shot_result == "miss") {
                    is_my_shot = true;
                }
            }
        } else if (command == "finished") {
            GetYN(strategy->IsFinished());
        } else if (command == "win") {
            GetYN(strategy->IsWin());
        } else if (command == "lose") {
            GetYN(strategy->IsLose());            
        } else if (command == "stop") {
            std::cout << "ok" << '\n';
            return true;
        } else if (command == "exit") {
            return false;
            std::cout << "ok" << '\n';
        }
    }
}
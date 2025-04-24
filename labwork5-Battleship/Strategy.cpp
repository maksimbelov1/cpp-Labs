#include "Strategy.h"

Strategy::Strategy(Field& field): field_(field) {}
Custom::Custom(Field& field): Strategy(field) {}
Ordered::Ordered(Field& field): Strategy(field) {}

bool Strategy::IsFinished() {
    return is_finished;
}
bool Strategy::IsWin() {
    return is_finished && is_win;
} 
bool Strategy::IsLose() {
    return (is_finished && !is_win);
}

std::string Strategy::Enemy_shot(uint64_t x, uint64_t y) {
    if (field_.current_sum_of_decks > 0) {
        for (std::vector<std::pair<uint64_t, uint64_t>>& ship : field_.ships_coordinates) {
            for (int i = 0; i < ship.size(); ++i) {
                if (ship[i].first == y && ship[i].second == x) {
                    ship.erase(ship.begin() + i);
                    field_.current_sum_of_decks--;
                    if (field_.current_sum_of_decks == 0 && !is_finished) {
                        is_finished = true;
                        is_win = false;
                    }
                    if (ship.empty()) {
                        return "kill";
                    } else {
                        return "hit";
                    }
                } 
            }
        }
    } else {
        if (!is_finished) {
            is_win = false;
        }
        is_finished = true;
    }
    return "miss";
}

 void Strategy::SetSHotNumberAfterHit(int16_t shot_number) {
    if (shot_number_after_hit == -1) {
        shot_number_after_hit = shot_number;
    }
    if (shot_number == -1) {
        shot_number_after_hit = shot_number;
    }
 }

std::pair<uint64_t, uint64_t> Ordered::GetNextShot() {
    if (field_.enemy_current_sum_of_decks == 0 && !is_finished) {
        is_finished = true;
        is_win = true;
    }
    if (is_first_shot) {
        is_first_shot = false;
        last_shot = {0, 0};
        return last_shot;
    } else {
        if (last_shot.second + 1 == field_.GetWidth()) {
            last_shot.first = (last_shot.first + 1) % (field_.GetHeight());
            last_shot.second = 0;
        } else {
            ++last_shot.second;
        }
        return last_shot;
    }
};

std::pair<uint64_t, uint64_t> Custom::GetNextShot() {
    if (field_.enemy_current_sum_of_decks > 0) {
        if (shot_number_after_hit != -1) {
            return GetNextAfterHitShot();
        }
        if (is_first_shot || !field_.enemy_current_sum_of_decks) {
            is_first_shot = false;
            last_shot = {0, 0};
            shift = 0;
            return last_shot;
        }
        is_plased = false;
        while (!is_plased) {
            if (last_shot.second + gap >= field_.GetWidth()) {
                if (last_shot.first + 1 < field_.GetHeight()) {
                    ++last_shot.first; 
                    ++shift;
                    last_shot.second = shift % gap;

                } else {
                    --gap;
                    last_shot = {0, 0};
                }
            } else {
                last_shot.second += gap;
            }
            if (field_.enemy_grid[last_shot.first][last_shot.second] == 'O') {
                if (!IsShipNearby(last_shot)) {
                    field_.enemy_grid[last_shot.first][last_shot.second] = 'H';
                    is_plased = true;
                }
            }
        }
    } else {
        if (!is_finished) {
            is_win = true;
        }
        is_finished = true;
    }
    return last_shot;
}

bool Custom::IsShipNearby(std::pair<uint64_t, uint64_t> coordinates) {
    for (int i = 0; i < 8; ++i) {
        new_x = coordinates.second + check_shift[i][0];
        new_y = coordinates.first + check_shift[i][1];
        if (new_x >= 0 && new_x < field_.GetWidth() && new_y >=0 && new_y < field_.GetHeight()) {
            if (field_.enemy_grid[new_y][new_x] == 'S') {
                return true;
            }
        }
    }
    return false;
}

std::pair<uint64_t, uint64_t> Custom::GetNextAfterHitShot() {
    last_shot_after_hit = last_shot;
    is_plased = false;
    while (!is_plased) {
        ++shot_number_after_hit;
        if (shot_number_after_hit < 4) {
            if (last_shot.second >= shot_number_after_hit) {
                last_shot_after_hit.second -= shot_number_after_hit;
            }
        } else if (shot_number_after_hit > 3 && shot_number_after_hit < 7) {
            if (last_shot.second + shot_number_after_hit - 3 < field_.GetWidth()) {
                last_shot_after_hit.second += (shot_number_after_hit - 3);
            }
        } else if (shot_number_after_hit > 6 && shot_number_after_hit < 10) {
            if (last_shot.first + 6 >= shot_number_after_hit) {
                last_shot_after_hit.first -= (shot_number_after_hit - 6);
            }
        } else if (shot_number_after_hit > 9 && shot_number_after_hit < 13) {
            if (last_shot.first + shot_number_after_hit - 9  < field_.GetHeight()) {
                last_shot_after_hit.first += (shot_number_after_hit - 9);
            }
        }
        if (field_.enemy_grid[last_shot_after_hit.first][last_shot_after_hit.second] == 'O') {
            field_.enemy_grid[last_shot_after_hit.first][last_shot_after_hit.second] = 'H';
            is_plased = true;
        }
    }
    return last_shot_after_hit;
}
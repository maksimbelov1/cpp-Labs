#include "Field.h"


Field::Field(): width_(-1), height_(-1), current_sum_of_decks(0), enemy_current_sum_of_decks(0), grid(nullptr), enemy_grid(nullptr) {}

Field::~Field() {
    if (grid) {
        for (uint64_t i = 0; i < height_; ++i) {
            delete[] grid[i];
        }
        delete[] grid;
        grid = nullptr;
    }
    if (enemy_grid) {
        for (uint64_t i = 0; i < height_; ++i) {
            if (enemy_grid[i] != nullptr) {
                delete[] enemy_grid[i];
            }
        }
        delete[] enemy_grid;
        enemy_grid = nullptr;
    }
}

bool Field::SetWidth(uint64_t width) {
    width_ = width;
    return true;
}

bool Field::SetHeight(uint64_t height) {
    height_ = height;
    return true;
}

bool Field::CreateGrid(char**& grid_) {
    if (grid_ != nullptr) {
        for (uint64_t i = 0; i < height_; ++i) {
            delete[] grid_[i];
        }
        delete[] grid_;
        grid_ = nullptr;
    }
    grid_ = new char*[height_];
    if (grid_ == nullptr) {
        return false;
    }
    for (uint64_t i = 0; i < height_; ++i) {
        grid_[i] = new char[width_];
        if (grid_[i] == nullptr) {
            for (uint64_t j = 0; j < i; ++j) {
                delete[] grid_[j];
            }
            delete[] grid_;
            grid_ = nullptr;
            return false;
        } 
        for (int j = 0; j < width_; ++j) {
            grid_[i][j] = 'O';
        }
    }
    return true;
}

uint64_t Field::GetWidth() {
    return width_;
}
uint64_t Field::GetHeight() {
    return height_;
}

bool Field::SetCount(uint16_t ship_size, uint64_t number_of_such_ships) {
    if ((ship_size > 0) && (ship_size < 5)) {
        number_of_ships[ship_size - 1] = number_of_such_ships;
        current_sum_of_decks += ship_size * number_of_such_ships;
        enemy_current_sum_of_decks = current_sum_of_decks;
        return true;
    } else {
        return false;
        
    }
}

uint64_t Field::GetCount(uint8_t ship_size) {
    return number_of_ships[ship_size - 1];
}

bool Field::Load(char* load_name) {
    if(!ships_coordinates.empty()) {
        ships_coordinates.clear();
    }
    if(!ships.empty()) {
        ships.clear();
    }
    std::ifstream load_file(load_name);
    if (!load_file) {
        return false;
    }
    load_file >> width_ >> height_;
    char orientation;
    uint16_t ship_size = 0;
    uint64_t x = 0;
    uint64_t y = 0;
    while (load_file >> ship_size >> orientation >> x >> y) {
        ships.push_back(Ship(x, y, ship_size, orientation));
        std::vector<std::pair<uint64_t, uint64_t>> current_ship_coordinates; 
        ++number_of_ships[ship_size - 1];
        current_sum_of_decks += ship_size;
        enemy_current_sum_of_decks = current_sum_of_decks;
        if (orientation == 'v') {
            for (int i = 0; i < ship_size; ++i) {
                current_ship_coordinates.push_back({y + i, x});
            }
        }
        if (orientation == 'h') {
            for (int i = 0; i < ship_size; ++i) {
                current_ship_coordinates.push_back({y, x + i});
            }
        }
        ships_coordinates.push_back(current_ship_coordinates);
    }
    load_file.close();
    return true;  
}

bool Field::Dump(char* dump_name) {
    std::ofstream dump_file(dump_name);
    if (!dump_file) {
        return false;
    }
    dump_file << width_ << " " << height_ << '\n';
    for (Ship& ship: ships) {
        dump_file << ship.ship_size << " " << ship.orientation << " " << ship.x << " " << ship.y << '\n';
    }
    dump_file.close();
    return true;
}

    
bool Field::CanPlaceShip(uint64_t x, uint64_t y, uint8_t ship_size, bool is_horizontal) {
    if (is_horizontal) {
        if (x + ship_size > width_) {
            return false;
        }
        for (int i = 0; i < ship_size; ++i) {
            if (grid[y][x + i] != 'O') {
                return false;
            }
        }
    } else {
        if (y + ship_size > height_) {
            return false;
        }
        for (int i = 0; i < ship_size; ++i) {
            if (grid[y + i][x] != 'O') {
                return false;
            }
        }
    }
    return true;
}
void Field::PlaceShip(uint64_t x, uint64_t y, uint8_t ship_size, bool is_horizontal) {
    std::vector<std::pair<uint64_t, uint64_t>> current_ship_coordinates; 
    if (is_horizontal) {
        for (int i = 0; i < ship_size; ++i) {
            current_ship_coordinates.push_back({y, x + i});
            grid[y][x + i] = 'S';
        }
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= ship_size; ++j) {
                new_x = x + j;
                new_y = y + i;
                if (new_x < width_ && new_y < height_) {
                    grid[new_y][new_x] = '~';
                }
            }
        }
    } else {
        for (int i = 0; i < ship_size; ++i) {
            current_ship_coordinates.push_back({y + i, x});
            grid[y + i][x] = 'S';
        }
        for (int i = -1; i <= ship_size; ++i) {
            for (int j = -1; j <= 1; ++j) {
                new_x = x + j;
                new_y = y + i;
                if (new_x < width_ && new_y < height_) {
                    grid[new_y][new_x] = '~';
                }
            }
        }       
    }
    ships_coordinates.push_back(current_ship_coordinates);
}

bool Field::PlaceShips() {
    for (int i = 4; i > 0; --i) {
        for (int j = 0; j < number_of_ships[i - 1]; ++j) {
            number_of_tries = 0;
            is_placed = false;
            while (!is_placed && number_of_tries < 100000) {
                rand_x = std::rand() % width_;
                rand_y = std::rand() % height_;
                rand_is_horizontal = std::rand() % 2;
                if (CanPlaceShip(rand_x, rand_y, i, rand_is_horizontal)) {
                    PlaceShip(rand_x, rand_y, i, rand_is_horizontal);
                    if (rand_is_horizontal) {
                        ships.push_back(Ship(rand_x, rand_y, i, 'h'));
                    } else {
                        ships.push_back(Ship(rand_x, rand_y, i, 'v'));
                    }
                    is_placed = true;
                }
                ++number_of_tries;
            }
            if (!is_placed) {
                return false;
            }
        }
    }
    for (uint64_t i = 0; i < height_; ++i) {
            delete[] grid[i];
        }
    delete[] grid;
    grid = nullptr;
    return true;
}




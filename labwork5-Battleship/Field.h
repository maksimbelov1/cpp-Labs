#pragma once
#include <iostream>
#include <cstdint>
#include <fstream>
#include <vector>

class Field {
public:
    Field();
    ~Field();

    bool SetWidth(const uint64_t width);
    bool SetHeight(const uint64_t height);
    uint64_t GetWidth();
    uint64_t GetHeight();
    
    bool CreateGrid(char**& grid_);

    bool SetCount(uint16_t ship_size, uint64_t number_of_such_ships);
    uint64_t GetCount(const uint8_t ship_size);

    bool Load(char* load_name);
    bool Dump(char* dump_name);

    bool CanPlaceShip(uint64_t x, uint64_t y, uint8_t ship_size, bool is_horizontal);
    void PlaceShip(uint64_t x, uint64_t y, uint8_t ship_size, bool is_horizontal);
    bool PlaceShips();

    char** grid;
    char** enemy_grid;
    uint64_t number_of_ships[4] = {0};
    uint64_t current_sum_of_decks;
    uint64_t enemy_current_sum_of_decks;
    struct Ship {
        uint64_t x;
        uint64_t y;
        uint16_t ship_size;
        char orientation;
        
        Ship(uint64_t x_, uint64_t y_, uint16_t ship_size_, char orientation_) :
        x(x_),
        y(y_),
        ship_size(ship_size_),
        orientation(orientation_) {}
    };
    std::vector<std::vector<std::pair<uint64_t, uint64_t>>> ships_coordinates;

private:
    uint64_t width_;
    uint64_t height_;
    uint64_t new_x;
    uint64_t new_y;
    uint64_t rand_x;
    uint64_t rand_y;
    uint32_t number_of_tries;
    bool rand_is_horizontal;
    bool is_placed;
    std::vector<Ship> ships;
};

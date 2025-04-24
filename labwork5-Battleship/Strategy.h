#include "Field.h"

class Strategy {
public: 
    Strategy(Field& field);
    virtual ~Strategy() {}

    virtual std::pair<uint64_t, uint64_t> GetNextShot() = 0;
    std::string Enemy_shot(uint64_t x, uint64_t y);
    void SetSHotNumberAfterHit(int16_t shot_number = -1);

    bool IsWin();
    bool IsFinished();
    bool IsLose();

protected:
    Field& field_;
    bool is_finished = false;
    bool is_win = false;
    std::pair<uint64_t, uint64_t> last_shot;
    bool is_first_shot = true;
    int16_t shot_number_after_hit = -1;
};

class Ordered: public Strategy {
public:
    Ordered(Field& field);
    ~Ordered() {};
    std::pair<uint64_t, uint64_t> GetNextShot();
};

class Custom: public Strategy {
public:
    Custom(Field& field);
    ~Custom() {};
    
    std::pair<uint64_t, uint64_t> GetNextShot();
    std::pair<uint64_t, uint64_t> GetNextAfterHitShot();
    bool IsShipNearby(std::pair<uint64_t, uint64_t> coordinates);

private:
    std::pair<uint64_t, uint64_t> last_shot_after_hit;
    uint16_t shift = 0;
    uint16_t gap = 4;
    bool is_plased;
    bool is_orientation_known;
    bool is_horizontal;
    int16_t check_shift[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    int64_t new_x;
    int64_t new_y;  
};
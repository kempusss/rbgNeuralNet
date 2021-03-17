#ifndef FLAT_LEARNER
#define FLAT_LEARNER

#include "reasoner.hpp"
#include "rl_model.hpp"
#include <array>
#include <filesystem>

class FlatLearner : public RLModel
{
private:
    std::array<std::array<float, reasoner::NUMBER_OF_PIECES + 1>, reasoner::BOARD_SIZE> scores; //[pos_id][piece_id] = value of given piece (or of empty cell if piece_id == 0) on given board position

    float smoothingRate;
    void update(const reasoner::game_state& state, float stateValue);

public:
    FlatLearner(float smoothingRate);

    float eval(const reasoner::game_state& game_state) const override;
    void update(const std::vector< std::pair<const reasoner::game_state&, float> >& to_learn) override;

    void save(std::filesystem::path file_path) const override;
    void load(std::filesystem::path file_path) override;
};

#endif

#include "flatlearner.hpp"
#include <fstream>

float FlatLearner::eval(const reasoner::game_state& game_state) const
{
    float score = 0;
    for(int cellId = 0; cellId < reasoner::BOARD_SIZE; ++cellId)
    {
        int pieceId = game_state.get_piece(cellId);
        score += scores[cellId][pieceId];
    }
    return score;
}


void FlatLearner::update(const std::vector< std::pair<const reasoner::game_state&, float> >& to_learn)
{
    for(auto& [state, score] : to_learn)
    {
        update(state, score);
    }

}

void FlatLearner::update(const reasoner::game_state& state, float score)
{    
    for(int cellId = 0; cellId < reasoner::BOARD_SIZE; ++cellId)
    {
        int pieceId = state.get_piece(cellId);
        scores[cellId][pieceId] *= 0.9;
        scores[cellId][pieceId] += 0.1 * score;
    }

}


void FlatLearner::save(std::filesystem::path file_path) const
{
    std::ofstream output(file_path);
    for(const auto& cell_scores : scores)
    {
        for(auto piece_score : cell_scores)
        {
            output << piece_score << ' ';
        }
        output << '\n';
    }

}

void FlatLearner::load(std::filesystem::path file_path)
{
    std::ifstream input(file_path);
    for(auto& cell_scores : scores)
    {
        for(auto& piece_score : cell_scores)
        {
            input >> piece_score;
        }
    }

}
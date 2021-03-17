#include "flatlearner.hpp"
#include <fstream>

FlatLearner::FlatLearner(float smoothingRate)
    : smoothingRate(smoothingRate)
{
}

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
        scores[cellId][pieceId] *= smoothingRate;
        scores[cellId][pieceId] += (1.f - smoothingRate) * score;
    }

}


void FlatLearner::save(std::filesystem::path file_path) const
{
    std::ofstream output(file_path);

    output << smoothingRate;
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

    input >> smoothingRate;
    for(auto& cell_scores : scores)
    {
        for(auto& piece_score : cell_scores)
        {
            input >> piece_score;
        }
    }

}
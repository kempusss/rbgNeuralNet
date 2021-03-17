#include "flat_exp_learner.hpp"
#include <fstream>

FlatExpLearner::FlatExpLearner(float smoothingRate)
    : smoothingRate(smoothingRate)
{
}

float FlatExpLearner::eval(const reasoner::game_state& game_state) const
{
    float score = 0;
    for(int cellId = 0; cellId < reasoner::BOARD_SIZE; ++cellId)
    {
        int pieceId = game_state.get_piece(cellId);
        score += scores[cellId][pieceId];
    }
    return score;
}


void FlatExpLearner::update(const std::vector< std::pair<std::reference_wrapper<const reasoner::game_state>, float> >& to_learn)
{
    for(const auto& [state, score] : to_learn)
    {
        update(state, score);
    }

}

void FlatExpLearner::update(const reasoner::game_state& state, float score)
{    
    for(int cellId = 0; cellId < reasoner::BOARD_SIZE; ++cellId)
    {
        int pieceId = state.get_piece(cellId);
        scores[cellId][pieceId] *= smoothingRate;
        scores[cellId][pieceId] += (1.f - smoothingRate) * score;
    }

}


void FlatExpLearner::save(std::filesystem::path file_path) const
{
    std::ofstream output(file_path);

    output << smoothingRate << '\n';
    for(const auto& cell_scores : scores)
    {
        for(auto piece_score : cell_scores)
        {
            output << piece_score << ' ';
        }
        output << '\n';
    }

}

void FlatExpLearner::load(std::filesystem::path file_path)
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
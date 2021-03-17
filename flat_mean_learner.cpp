#include "flat_mean_learner.hpp"
#include <fstream>


float FlatMeanLearner::eval(const reasoner::game_state& game_state) const
{
    float score = 0;
    for(int cellId = 0; cellId < reasoner::BOARD_SIZE; ++cellId)
    {
        int pieceId = game_state.get_piece(cellId);
        score += scores[cellId][pieceId];
    }
    return score;
}


void FlatMeanLearner::update(const std::vector< std::pair<std::reference_wrapper<const reasoner::game_state>, float> >& to_learn)
{
    for(const auto& [state, score] : to_learn)
    {
        update(state, score);
    }

}

void FlatMeanLearner::update(const reasoner::game_state& state, float score)
{
    for(int cellId = 0; cellId < reasoner::BOARD_SIZE; ++cellId)
    {
        int pieceId = state.get_piece(cellId);
        float oldSum = scores[cellId][pieceId] * update_counts[cellId][pieceId];
        ++update_counts[cellId][pieceId];
        scores[cellId][pieceId] = (oldSum + score) / update_counts[cellId][pieceId];
    }

}


void FlatMeanLearner::save(std::filesystem::path filePath) const
{
    std::ofstream output(filePath);

    for(const auto& cell_scores : scores)
    {
        for(auto piece_score : cell_scores)
        {
            output << piece_score << ' ';
        }
        output << '\n';
    }

    output << '\n';
    for(const auto& cell_counts : update_counts)
    {
        for(auto piece_count : cell_counts)
        {
            output << piece_count << ' ';
        }
        output << '\n';
    }

}

void FlatMeanLearner::load(std::filesystem::path filePath)
{
    std::ifstream input(filePath);

    for(auto& cell_scores : scores)
    {
        for(auto& piece_score : cell_scores)
        {
            input >> piece_score;
        }
    }


    for(const auto& cell_counts : update_counts)
    {
        for(auto piece_count : cell_counts)
        {
            input >> piece_count;
        }
    }

}
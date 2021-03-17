#include "reasoner.hpp"
#include <torch/torch.h>
#include "state_parser.hpp"
#include <cmath>
#include <limits>

double constexpr sqrtNewtonRaphson(double x, double curr, double prev) noexcept
{
    return curr == prev
        ? curr
        : sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
}

double constexpr sqrt(double x) noexcept
{
	return sqrtNewtonRaphson(x, x, 0);
}

torch::Tensor StateParser::parseGameState(const reasoner::game_state& game_state, 
                                          torch::Device device)
{
    constexpr int N = (int)(sqrt(reasoner::BOARD_SIZE));
    return torch::Tensor();
}
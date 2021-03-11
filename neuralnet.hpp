#ifndef NEURAL_NET
#define NEURAL_NET

#include "reasoner.hpp"
#include <vector>
#include <filesystem>
#include <sys/types.h>

class NeuralNet 
{
public:
    NeuralNet() {};
    virtual float eval(const reasoner::game_state& game_state) const = 0;
    virtual void update(const std::vector< std::pair<const reasoner::game_state&, float> >& to_learn) = 0;
    virtual void save(std::filesystem::path file_path) const {};
    virtual void load(std::filesystem::path file_path);
};

#endif

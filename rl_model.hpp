#ifndef RL_MODEL
#define RL_MODEL

#include "reasoner.hpp"
#include <vector>
#include <filesystem>
#include <sys/types.h>
#include <functional>

class RLModel 
{
public:
    RLModel() {};
    virtual float eval(const reasoner::game_state& game_state) const = 0;
    virtual void update(const std::vector< std::pair<std::reference_wrapper<const reasoner::game_state>, float> >& to_learn) = 0;
    virtual void save(std::filesystem::path file_path) const = 0;
    virtual void load(std::filesystem::path file_path) = 0;
};

#endif

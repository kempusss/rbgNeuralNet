#ifndef NEURAL_NET
#define NEURAL_NET

#include "rl_model.hpp"
#include <memory>
#include <stdexcept>

#include <torch/torch.h>

template<class Parser>
class NeuralNet : public RLModel
{
private:
    std::unique_ptr<torch::nn::Module> net;
    torch::Device device;

public:
    NeuralNet(std::unique_ptr<torch::nn::Module> net, bool on_GPU = false);
        : net(net), device(torch::kCPU)
    {
        if(on_GPU)
        {
            if(torch::cuda::is_available())
            { 
                device = torch::Device(torch::kGPu);
            }
            else
            {
                throw logic_error("GPU is not available!");
            }
        }
    }

    float eval(const reasoner::game_state& game_state) const override
    {
        auto inputTensor = Parser::parseGameState(game_state, device);
        return net->forward(inputTensor);
    }

    void update(const std::vector< std::pair<const reasoner::game_state&, float> >& to_learn) override
    {
        // TODO
    }

    void save(std::filesystem::path file_path) override
    {
        torch::save(*net, file_path);
    }

    void load(std::filesystem::path file_path) override
    {
        if (net == nullptr) 
        {
            throw logic_error("Net must be initalized before loading");
        }
        
        torch::load(*net, file_path);
    }
};

#endif
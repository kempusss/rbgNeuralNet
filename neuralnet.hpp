#ifndef NEURAL_NET
#define NEURAL_NET

#include "rl_model.hpp"
#include "net_module.hpp"
#include "state_parser.hpp"
#include <memory>
#include <type_traits>
#include <stdexcept>
#include <torch/torch.h>

template<class Parser>
class NeuralNet : public RLModel
{
private:
    std::unique_ptr<NetModule> net;
    torch::Device device;

public:
    NeuralNet(std::unique_ptr<NetModule> net, bool on_GPU = false)
        : net(std::move(net)), device(torch::kCPU)
    {
        static_assert(std::is_base_of_v<StateParser, Parser>, "Parser must inherit from interface StateParser");
        if (!this->net) 
        {
            throw std::logic_error("Net must be initalized before loading");
        }

        if(on_GPU)
        {
            if(torch::cuda::is_available())
            {
                device = torch::Device(torch::kCUDA);
                this->net->to(device);
                std::cout << "Neural net moved to GPU!" << std::endl;
            }
            else
            {
                throw std::logic_error("GPU is not available!");
            }
        }
    }

    float eval(const reasoner::game_state& game_state) const override
    {
        auto inputTensor = Parser::parseGameState(game_state, device);
        return (net->forward(inputTensor)).item().toFloat();
    }

    void update(const std::vector< std::pair<const reasoner::game_state&, float> >& to_learn) override
    {
        // TODO
    }

    void save(std::filesystem::path file_path) const override
    {
        torch::serialize::OutputArchive output_archive;
        net->save(output_archive);
        output_archive.save_to(file_path);
    }

    void load(std::filesystem::path file_path) override
    {
        torch::serialize::InputArchive archive;
        archive.load_from(file_path);
        net->load(archive);
        net->to(device);
    }

    torch::Tensor forward(torch::Tensor x)
    {
        x = x.to(device);
        return net->forward(x);
    }

    void train(torch::Tensor input, torch::Tensor target, float learning_rate = 0.01f, int iters = 100)
    {
        input = input.to(device);
        target = target.to(device);

        torch::optim::SGD optimizer(net->parameters(), learning_rate);

        for(int i = 0; i < iters; ++i)
        {
            optimizer.zero_grad();
            torch::Tensor pred = forward(input);
            torch::Tensor loss = torch::mse_loss(pred, target);
            loss.backward();
            optimizer.step();
        }
    }
};

#endif
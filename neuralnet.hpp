#ifndef NEURAL_NET
#define NEURAL_NET

#include "rl_model.hpp"
#include "net_module.hpp"
#include "default_state_parser.hpp"
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
    std::unique_ptr<torch::optim::SGD> optimizer;
    float learning_rate;

public:
    NeuralNet(std::unique_ptr<NetModule> net, bool on_GPU = false, float learning_rate = 0.01f)
        : net(std::move(net)), 
          device(torch::kCPU), 
          learning_rate(learning_rate), 
          optimizer(std::make_unique<torch::optim::SGD>(this->net->parameters(), learning_rate))
    {
        static_assert(std::is_base_of_v<DefaultStateParser, Parser>, "Parser must inherit from interface StateParser");
        
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
        auto input_tensor = Parser::parse_game_state(game_state, device);
        return (net->forward(input_tensor)).item().toFloat();
    }

    void update(const std::vector< std::pair<std::reference_wrapper<const reasoner::game_state>, float> >& to_learn) override
    {
        std::vector <torch::Tensor> game_states(to_learn.size());
        std::vector <float> targets(to_learn.size());

        for(unsigned int i = 0; i < to_learn.size(); ++i)
        {
            game_states[i] = Parser::parse_game_state(to_learn[i].first, device);
            targets[i] = to_learn[i].second;
        }

        auto options = torch::TensorOptions().dtype(torch::kFloat32).requires_grad(true);

        torch::Tensor input = torch::stack(game_states, 0);

        torch::Tensor output = torch::from_blob(targets.data(), {(int)targets.size(), 1}, options).to(device);

        optimizer->zero_grad();
        torch::Tensor predictions = net->forward(input);
        torch::Tensor loss = torch::mse_loss(predictions, output);
        loss.backward();
        optimizer->step();
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
        optimizer = std::make_unique<torch::optim::SGD>(this->net->parameters(), learning_rate);
    }

    torch::Tensor forward(torch::Tensor x)
    {
        x = x.to(device);
        return net->forward(x);
    }

    void train(torch::Tensor input, torch::Tensor target, int iters = 100)
    {
        input = input.to(device);
        target = target.to(device);

        for(int i = 0; i < iters; ++i)
        {
            optimizer->zero_grad();
            torch::Tensor pred = forward(input);
            torch::Tensor loss = torch::mse_loss(pred, target);
            loss.backward();
            optimizer->step();
        }
    }
};

#endif
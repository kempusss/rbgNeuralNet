#include "shallownet.hpp"
#include "neuralnet.hpp"
#include "default_state_parser.hpp"
#include "flat_exp_learner.hpp"
#include "flat_mean_learner.hpp"
#include "reasoner.hpp"
#include <torch/torch.h>
#include <iostream>
#include <vector>

void XORtest()
{
    torch::Device device(torch::kCPU);

    float XORarr[4][2] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
    float XORout[4][1] = {{0}, {1}, {1}, {0}};
    auto options = torch::TensorOptions().dtype(torch::kFloat32).requires_grad(true);
    
    torch::Tensor XORdataset = torch::from_blob(XORarr, {4, 2}, options);
    torch::Tensor XORtarget = torch::from_blob(XORout, {4, 1}, options);

    std::cout << "Dataset:" << std::endl;
    std::cout << XORdataset.to(at::kCUDA) << std::endl;

    NeuralNet<DefaultStateParser> net(std::make_unique<ShallowNet>(2,10), true);
    std::cout << "Network initialized" << std::endl;
    net.train(XORdataset, XORtarget, 100000);

    std::vector <torch::Tensor> tensory;

    for (int i = 0; i < 4; ++i)
    {
        float test[2] = {0, 1};
        tensory.push_back(torch::from_blob(test, {2}, options));
    }

    std::cout << "Trained" << std::endl;

    auto stacktest = torch::stack(tensory);

    std::cout << net.forward(stacktest) << std::endl;

    net.save("siema.pt");

    NeuralNet<DefaultStateParser> net2(std::make_unique<ShallowNet>(2, 10), false);

    net2.load("siema.pt");

    std::cout << net.forward(XORdataset) << std::endl;
}

void testFlatLeaners()
{
    FlatExpLearner learnerExp(0.5);
    learnerExp.save("learn_exp.txt");
    learnerExp.load("learn_exp.txt");
    FlatMeanLearner learnerMean;
    learnerMean.save("learn_mean.txt");
    learnerMean.load("learn_mean.txt");
}

void testParser()
{
    reasoner::game_state state;
    reasoner::resettable_bitarray_stack wtf;

    for(int i = 0; i < 5; ++i)
        state.apply_any_move(wtf);

    auto otherstate = state;
    for(int i = 0; i < 5; ++i)
        otherstate.apply_any_move(wtf);

    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            std::cout << state.get_piece(3 * i + j) << ' ';
        }
        std::cout << std::endl;
    }

    std::cout << DefaultStateParser::parse_game_state(state, torch::kCPU) << std::endl;
    return;

    std::vector< std::pair<std::reference_wrapper<const reasoner::game_state>, float> > temp;

    for(int i = 0; i < 3; ++i)
    {
        temp.push_back({state, 0.8});
    }


    FlatExpLearner learnerExp(0.5);
    learnerExp.update(temp);
    std::cout << learnerExp.eval(state) << std::endl;

    FlatMeanLearner learnerMean;
    learnerMean.update(temp);
    std::cout << learnerMean.eval(state) << std::endl;
    
    for(int i = 0; i < 3; ++i)
    {
        temp.push_back({otherstate, 0.4});
    }

    learnerExp.update(temp);
    std::cout << learnerExp.eval(state) << std::endl;
    std::cout << learnerExp.eval(otherstate) << std::endl;

    learnerMean.update(temp);
    std::cout << learnerMean.eval(state) << std::endl;
    std::cout << learnerMean.eval(otherstate) << std::endl;

    NeuralNet<DefaultStateParser> net(std::make_unique<ShallowNet>(DefaultStateParser::input_size(), 100), true);

    for(int i = 0; i < 10000; ++i)
        net.update(temp);

    std::cout << "Training completed!" << std::endl;

    std::cout << net.eval(state) << std::endl;
    std::cout << net.eval(otherstate) << std::endl;


}

int main()
{
    testParser();


    return 0;
}

#ifndef NET_MODULE
#define NET_MODULE

#include <torch/torch.h>

class NetModule : public torch::nn::Module
{
public:
    virtual torch::Tensor forward(torch::Tensor x) = 0;
};

#endif
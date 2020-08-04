//
// Created by leenus on 27/07/2020.
//

#ifndef MNIST_NETWORK_HPP
#define MNIST_NETWORK_HPP

#include <cmath>
#include <numeric>
#include <random>
#include <vector>

static inline auto sigmoid(float input) -> float {
    return (1 / (1 + expf(-input)));
}

class Network {

    const std::vector<size_t> structure;
    std::mt19937 gen = std::mt19937(42);

    struct Neuron {
        std::vector<float> weights;
        float bias;

        explicit Neuron(const int &inputs) : bias(0.0f) {
            for (size_t i = 0; i < inputs; ++i) {
                weights.push_back(1.0f);
            }
        }
        explicit Neuron(const int &inputs, std::mt19937 &gen) {

            auto dis = std::normal_distribution<float>(0, 1);

            // Randomly initialize the bias and every weight

            bias = dis(gen);

            for (size_t i = 0; i < inputs; ++i) {
                weights.push_back(dis(gen));
            }
        }

        [[nodiscard]] auto activation(const std::vector<float> &inputs) const
            -> float {

            float sum = 0.0f;
            // Fix cache behaviour?
            for (size_t i = 0; i < inputs.size(); ++i) {
                sum += inputs.at(i) * weights.at(i);
            }
            return sigmoid(sum + bias);
        }
    };

    using Layer = std::vector<Neuron>;
    std::vector<Layer> network;

    static auto get_activations(const std::vector<float> &input,
                                const Layer &layer) -> std::vector<float> {

        auto output = std::vector<float>();
        output.reserve(input.size());

        for (const auto &neuron : layer) {
            const float activation = neuron.activation(input);
            output.push_back(activation);
        }
        return output;
    }

  public:

    auto backprop(std::vector<float>& errors, int layer) {

        for (const auto & neuron : network.at(layer)) {

            for (const auto& w : neuron.weights) {

                //Error pos or negative??
            }
        }
    }


    static auto squared_error(const std::vector<float> &activations,
                              const int label) -> float {

        auto expected = std::vector<float>(activations.size());
        expected.at(label) = 1.0f;

        constexpr auto transform = [](const float &a, const float &b) -> float {
            return (a - b) * (a - b);
        };

        auto reduce = std::plus<>();

        auto squared_error = std::transform_reduce(
            expected.cbegin(), expected.cend(), activations.cbegin(), 0.0f,
            reduce, transform);

        return squared_error;
    }

    // structure is a vector of numbers that describe the size of each layer.
    explicit Network(const std::vector<size_t> &structure)
        : structure(structure) {

        // Init input layer with default bias and weights.
        auto input_layer = Layer();
        input_layer.reserve(structure.at(0));

        for (size_t i = 0; i < structure.at(0); ++i) {
            const auto n = Neuron(1);
            input_layer.push_back(n);
        }
        network.push_back(input_layer);

        // for every other layer we will give each neuron a ref to our RNG
        for (int i = 1; i < structure.size(); ++i) {

            auto layer = Layer();
            const size_t inputs = structure.at(i - 1);

            // for every neuron
            for (size_t j = 0; j < structure.at(i); ++j) {

                auto n = Neuron(inputs, gen);
                layer.push_back(n);
            }
            network.push_back(layer);
        }
    }

    static auto error_vector(const std::vector<float> &activations,
                             const int label) -> std::vector<float> {

        auto expected = std::vector<float>(activations.size());
        expected.at(label) = 1.0f;

        constexpr auto transform = [](const float &a, const float &b) -> float {
            return 2 * (a - b);
        };

        auto errors = std::vector<float>(activations.size());
        std::transform(expected.cbegin(), expected.cend(),
                       activations.cbegin(), errors.begin(), transform);

        return errors;
    }

    auto feedforward(const std::vector<float> &input) -> std::vector<float> {

        std::vector<float> next_input = input;

        for (int i = 1; i < network.size(); ++i) {
            next_input = get_activations(next_input, network.at(i));
        }

        return next_input;
    }

    inline auto rand_weight() -> float {
        auto dis = std::normal_distribution<float>(0, 1);
        return dis(gen);
    }
};

#endif // MNIST_NETWORK_HPP

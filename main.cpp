#include "include/dataset.hpp"
#include "include/network.hpp"
//#include "include/drawimage.hpp"

#include <iostream>
#include <string>

using namespace std;

int main() {

    auto labels = read_labels("../train/train-labels-idx1-ubyte");
    auto images = read_images("../train/train-images-idx3-ubyte");

    const auto image_size = images.at(0).size();
    const auto img_label = labels.at(0);

    auto structure = std::vector<size_t>{1, 1, 1};
//    auto structure = std::vector<size_t>{image_size, 16, 10};
    auto net = Network(structure);

    auto img_float = std::vector<float>();
    auto float_input = std::vector<float>{1.0f};
    const auto &img = images.at(0);

    img_float.reserve(img.size());
    for (unsigned char i: img) {
        img_float.push_back(static_cast<float>(i) / 255.0f);
    }

    auto res = net.feedforward(float_input);
//    auto res = net.feedforward(img_float);

    auto idx = 0;

    for (const auto& number: res) {
        cout << "[" << idx << "]" << number << endl;
        ++idx;
    }
    cout << endl;
    idx = 0;

    auto errors = Network::error_vector(res, 0);

    for (const auto& err: errors) {
        cout << "[" << idx << "]" << err << endl;
        ++idx;
    }

    cout << "Total Cost: " << Network::squared_error(res, 0) << endl;

    //    auto index = images.size() - 10000;
    //
    //    auto img = images.at(index);
    //
    //    cout << images.size() << endl;
    //    cout << img.size() << endl;
    //
    //    auto label = labels.at(index);
    //    auto title = to_string(label);
    //
    //    draw_image(img, 28, title);

    return 0;
}

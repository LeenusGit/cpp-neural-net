//
// Created by leenus on 24/07/2020.
//

#ifndef MNIST_DATASET_HPP
#define MNIST_DATASET_HPP

#include <fstream>
#include <string>
#include <vector>

auto read_next_byte(std::ifstream &file) -> u_char {

    char label;
    file.read(&label, sizeof(label));
    return (static_cast<u_char>(label));
}

auto read_labels(const std::string &path) -> std::vector<int> {

    auto output = std::vector<int>();
    auto file = std::ifstream(path, std::ios::binary);

    char mem_block[sizeof(int32_t)];

    file.read(mem_block, sizeof(mem_block));
    std::reverse(mem_block, mem_block + sizeof(mem_block));
    const int magic_number = *reinterpret_cast<int32_t *>(mem_block);
    if (magic_number != 2049) return output;

    file.read(mem_block, sizeof(mem_block));
    std::reverse(mem_block, mem_block + sizeof(mem_block));
    const int items_count = *reinterpret_cast<int32_t *>(mem_block);
    output.reserve(items_count);

    for (int i = 0; i < items_count; ++i) {
        auto label = read_next_byte(file);
        output.push_back(static_cast<int>(label));
    }

    return output;
}

auto read_next_int(std::ifstream &file) -> int {

    char mem_block[sizeof(int32_t)];
    file.read(mem_block, sizeof(mem_block));
    std::reverse(mem_block, mem_block + sizeof(mem_block));
    auto number = *reinterpret_cast<int32_t *>(mem_block);
    return number;
}

using Image = std::vector<u_char>;

auto read_next_image(std::ifstream &file, const int bytes) {

    auto img = Image();
    img.reserve(bytes);

    for (int i = 0; i < bytes; ++i) {
        auto pixel_val = read_next_byte(file);
        img.push_back(pixel_val);
    }

    return img;
}

auto read_images(const std::string &path) -> std::vector<Image> {

    auto output = std::vector<Image>();

    auto file = std::ifstream(path, std::ios::binary);

    const auto magic_number = read_next_int(file);
    if (magic_number != 2051) return output;

    const auto images_count = read_next_int(file);
    output.reserve(images_count);
    const auto rows = read_next_int(file);
    const auto columns = read_next_int(file);

    for (int i = 0; i < images_count; ++i) {
        auto img = read_next_image(file, columns * rows);
        output.push_back(img);
    }
    return output;
}


#endif//MNIST_DATASET_HPP

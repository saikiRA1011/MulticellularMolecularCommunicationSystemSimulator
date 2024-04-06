#include <iostream>
#include <yaml-cpp/yaml.h>

int main(void)
{
    std::string path = "src/config.yaml";

    // 指定したファイルを読み込む
    YAML::Node config = YAML::LoadFile(path);

    // cell/cell_seedの値を取得
    int32_t CELL_SEED = config["cell"]["cell_seed"].as<int32_t>();
    std::cout << "cell_seed: " << CELL_SEED << std::endl;

    return 0;
}
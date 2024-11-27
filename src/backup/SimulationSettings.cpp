#include "SimulationSettings.hpp"

/**
 * @brief 設定ファイルからの読み込み
 * @brief 必要に応じてユーザが設定項目を書き足しても良い。
 * @note 設定ファイルのパスはsrc/config.yaml。パースに失敗した場合は例外処理をする。
 *
 * @return true
 * @return false
 */
bool SimulationSettings::init_settings()
{
    std::string path = "src/config.yaml";

    try {
        YAML::Node config = YAML::LoadFile(path);

        CELL_SEED = config["cell"]["cell_seed"].as<int32_t>();
        CELL_NUM  = config["cell"]["cell_num"].as<int32_t>();

        SIM_STEP             = config["simulation"]["sim_step"].as<int32_t>();
        OUTPUT_INTERVAL_STEP = config["simulation"]["output_interval"].as<int32_t>();
        FIELD_X_LEN          = config["simulation"]["field_x_len"].as<int32_t>();
        FIELD_Y_LEN          = config["simulation"]["field_y_len"].as<int32_t>();
        FIELD_Z_LEN          = config["simulation"]["field_z_len"].as<int32_t>();
        DELTA_TIME           = config["simulation"]["delta_time"].as<double>();

        MOLECULE_FIELD_X_LEN = config["molecule"]["field_x_len"].as<int32_t>();
        MOLECULE_FIELD_Y_LEN = config["molecule"]["field_y_len"].as<int32_t>();
        MOLECULE_FIELD_Z_LEN = config["molecule"]["field_z_len"].as<int32_t>();
        MOLECULE_DELTA_TIME  = config["molecule"]["delta_time"].as<double>();

        USE_CELL_LIST           = config["cell_list"]["use_cell_list"].as<bool>();
        GRID_SIZE_MAGNIFICATION = config["cell_list"]["grid_size_mag"].as<int32_t>();
        SEARCH_RADIUS           = config["cell_list"]["search_radius"].as<int32_t>();

    } catch (YAML::ParserException& e) {
        std::cerr << e.what() << std::endl;

        return false;
    }

    return true;
}

// staticメンバの初期化
bool SimulationSettings::USE_CELL_LIST              = false;
int32_t SimulationSettings::CELL_SEED               = 0;
int32_t SimulationSettings::SIM_STEP                = 0;
int32_t SimulationSettings::OUTPUT_INTERVAL_STEP    = 0;
int32_t SimulationSettings::CELL_NUM                = 0;
int32_t SimulationSettings::GRID_SIZE_MAGNIFICATION = 0;
int32_t SimulationSettings::SEARCH_RADIUS           = 0;
int32_t SimulationSettings::FIELD_X_LEN             = 0;
int32_t SimulationSettings::FIELD_Y_LEN             = 0;
int32_t SimulationSettings::FIELD_Z_LEN             = 0;
int32_t SimulationSettings::MOLECULE_FIELD_X_LEN    = 0;
int32_t SimulationSettings::MOLECULE_FIELD_Y_LEN    = 0;
int32_t SimulationSettings::MOLECULE_FIELD_Z_LEN    = 0;
double SimulationSettings::DELTA_TIME               = 0.0;
double SimulationSettings::MOLECULE_DELTA_TIME      = 0.0;

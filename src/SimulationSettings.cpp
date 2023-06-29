#include "SimulationSettings.hpp"

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
        DELTA_TIME           = config["simulation"]["delta_time"].as<double>();

        USE_CELL_LIST           = config["cell_list"]["use_cell_list"].as<bool>();
        GRID_SIZE_MAGNIFICATION = config["cell_list"]["grid_size_mag"].as<int32_t>();
        SEARCH_RADIUS           = config["cell_list"]["search_radius"].as<int32_t>();

    } catch (YAML::ParserException& e) {
        std::cerr << e.what() << std::endl;
    }

    return true;
}

bool SimulationSettings::USE_CELL_LIST              = false;
int32_t SimulationSettings::CELL_SEED               = 0;
int32_t SimulationSettings::SIM_STEP                = 0;
int32_t SimulationSettings::OUTPUT_INTERVAL_STEP    = 0;
int32_t SimulationSettings::CELL_NUM                = 0;
int32_t SimulationSettings::GRID_SIZE_MAGNIFICATION = 0;
int32_t SimulationSettings::SEARCH_RADIUS           = 0;
int32_t SimulationSettings::FIELD_X_LEN             = 0;
int32_t SimulationSettings::FIELD_Y_LEN             = 0;
double SimulationSettings::DELTA_TIME               = 0.0;

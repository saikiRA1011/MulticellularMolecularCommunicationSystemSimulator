#include "UserMoleculeSpace.hpp"

UserMoleculeSpace::UserMoleculeSpace(const u_int64_t moleculeNum, const MoleculeDistributionType distributionType, const MoleculeSpaceBorderType borderType,
                                     std::vector<std::shared_ptr<UserCell>>& cells, const u_int32_t ID)
  : MoleculeSpace(moleculeNum, distributionType, borderType, cells, ID, 0.024 * 1000000.0)
{
}

UserMoleculeSpace::~UserMoleculeSpace()
{
}

void UserMoleculeSpace::calcConcentrationDiff() noexcept
{
    // 毎ステップ実行されることを考えると控えめな値にしておいた方がいい。あるいは細胞側の放出量を増やす
    constexpr double hydrolysisCoefficient = 5.4;

    // すべての格子について拡散、生成、分解、移流を行う
    // u_int32_t x, y, z;
    // #pragma omp parallel for private(x, y, z)
    for (u_int32_t x = 1; x <= width; x++) {
        for (u_int32_t y = 1; y <= height; y++) {
            for (u_int32_t z = 1; z <= depth; z++) {
                deltaMoleculeSpace[x][y][z] = diffuse(x, y, z);
                deltaMoleculeSpace[x][y][z] += -hydrolysisCoefficient * moleculeSpace[x][y][z];
            }
        }
    }

    // FIXME: cellsの数は常に変化するので、vectorへの参照を持っておく方がいい
    // 細胞からの放出を加える
    const int32_t width  = SimulationSettings::FIELD_X_LEN;
    const int32_t height = SimulationSettings::FIELD_Y_LEN;
    const int32_t depth  = SimulationSettings::FIELD_Z_LEN;
#pragma omp parallel for
    for (u_int32_t i = 0; i < cells.size(); i += 3000) {
        for (u_int32_t j = 0; j < 3000 && i + j < cells.size(); j++) {
            std::shared_ptr<UserCell>& cell = cells[i + j];

            // FIXME : 計算に使うdrと分子空間・細胞空間のdrが異なるので、一旦drを外している。
            // 修正したら+1の前にdrで割る必要がある
            int32_t x = (int32_t)((cell->getPosition().x + width / 2)) + 1;
            int32_t y = (int32_t)((cell->getPosition().y + height / 2)) + 1;
            int32_t z = (int32_t)((cell->getPosition().z + depth / 2)) + 1;

#pragma omp atomic
            deltaMoleculeSpace[x][y][z] += cell->emitMolecule(ID);
        }
    }

#pragma omp parallel for
    for (u_int32_t i = 0; i < cells.size(); i += 3000) {
        for (u_int32_t j = 0; j < 3000 && i + j < cells.size(); j++) {
            int32_t x = (int32_t)((cells[i + j]->getPosition().x + width / 2)) + 1;
            int32_t y = (int32_t)((cells[i + j]->getPosition().y + height / 2)) + 1;
            int32_t z = (int32_t)((cells[i + j]->getPosition().z + depth / 2)) + 1;

            cells[i + j]->setDiffState(moleculeSpace[x][y][z]);
        }
    }
}

void UserMoleculeSpace::nextStep() noexcept
{
    // int x, y, z;
    double dt = SimulationSettings::DELTA_TIME;
    // #pragma omp parallel for schedule(dynamic) private(x, y, z, dt)
    for (int x = 1; x <= width; x++) {
        for (int y = 1; y <= height; y++) {
            for (int z = 1; z <= depth; z++) {
                moleculeSpace[x][y][z] += deltaMoleculeSpace[x][y][z] * dt;
                // deltaMoleculeSpace[x][y][z] = 0.0;
            }
        }
    }

    setupBoundary(moleculeSpace, borderType);
}
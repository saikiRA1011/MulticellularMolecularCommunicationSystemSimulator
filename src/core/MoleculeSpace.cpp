#include "MoleculeSpace.hpp"

// Distribution::Distribution(/* args */)
// {
// }

// Distribution::~Distribution()
// {
// }

// void Distribution::setDistributionType(MoleculeDistributionType distributionType)
// {
//     this->distributionType = distributionType;
// }

// double Distribution::generate(auto& randGen)
// {
//     switch (distributionType) {
//         case MoleculeDistributionType::UNIFORM:
//             return normalDistribution(randGen);
//             break;
//         case MoleculeDistributionType::GAUSSIAN:
//             return uniformDistribution(randGen);
//             break;
//         default:
//             std::cerr << "DistributionType is Wrong: " << NAMEOF_ENUM(distributionType) << std::endl;
//             exit(1);
//     }
// }

// 3次元拡散方程式の参考文献 https://cvtech.cc/diffusion3d/
double MoleculeSpace::diffuse(int32_t x, int32_t y, int32_t z)
{
    return D *
           (moleculeSpace[x + 1][y][z] + moleculeSpace[x - 1][y][z] + moleculeSpace[x][y + 1][z]   //
            + moleculeSpace[x][y - 1][z] + moleculeSpace[x][y][z + 1] + moleculeSpace[x][y][z - 1] //
            - 6.0 * moleculeSpace[x][y][z]) /
           (dr * dr);
}

double MoleculeSpace::production(int32_t x, int32_t y, int32_t z)
{
    return 0.0;
}

double MoleculeSpace::decay(int32_t x, int32_t y, int32_t z)
{
    return 0.0;
}

double MoleculeSpace::advection(int32_t x, int32_t y, int32_t z)
{
    return 0.0;
}

void MoleculeSpace::setupBoundary(Field3D<double>& ms, MoleculeSpaceBorderType borderType)
{
    switch (borderType) {
        case MoleculeSpaceBorderType::NEUMANN:
            // #pragma omp parallel for
            for (int32_t x = 1; x <= width; x++) {
                for (int32_t y = 1; y <= height; y++) {
                    ms[x][y][0]         = ms[x][y][1];
                    ms[x][y][depth + 1] = ms[x][y][depth];
                }
            }
            // #pragma omp parallel for
            for (int32_t x = 1; x <= width; x++) {
                for (int32_t z = 1; z <= depth; z++) {
                    ms[x][0][z]          = ms[x][1][z];
                    ms[x][height + 1][z] = ms[x][height][z];
                }
            }
            // #pragma omp parallel for
            for (int32_t y = 1; y <= height; y++) {
                for (int32_t z = 1; z <= depth; z++) {
                    ms[0][y][z]         = ms[1][y][z];
                    ms[width + 1][y][z] = ms[width][y][z];
                }
            }
            break;

        case MoleculeSpaceBorderType::DIRICHLET:
            // #pragma omp parallel for
            for (int32_t x = 1; x <= width; x++) {
                for (int32_t y = 1; y <= height; y++) {
                    ms[x][y][0]         = 0;
                    ms[x][y][depth + 1] = 0;
                }
            }
            // #pragma omp parallel for
            for (int32_t x = 1; x <= width; x++) {
                for (int32_t z = 1; z <= depth; z++) {
                    ms[x][0][z]          = 0;
                    ms[x][height + 1][z] = 0;
                }
            }
            // #pragma omp parallel for
            for (int32_t y = 1; y <= height; y++) {
                for (int32_t z = 1; z <= depth; z++) {
                    ms[0][y][z]         = 0;
                    ms[width + 1][y][z] = 0;
                }
            }
            break;

        case MoleculeSpaceBorderType::PBC:
            // #pragma omp parallel for
            for (int32_t x = 1; x <= width; x++) {
                for (int32_t y = 1; y <= height; y++) {
                    ms[x][y][0]         = ms[x][y][depth];
                    ms[x][y][depth + 1] = ms[x][y][1];
                }
            }
            // #pragma omp parallel for
            for (int32_t x = 1; x <= width; x++) {
                for (int32_t z = 1; z <= depth; z++) {
                    ms[x][0][z]          = ms[x][height][z];
                    ms[x][height + 1][z] = ms[x][1][z];
                }
            }
            // #pragma omp parallel for
            for (int32_t y = 1; y <= height; y++) {
                for (int32_t z = 1; z <= depth; z++) {
                    ms[0][y][z]         = ms[width][y][z];
                    ms[width + 1][y][z] = ms[1][y][z];
                }
            }
        default:
            std::cerr << "BorderType is Wrong: " << NAMEOF_ENUM(borderType) << std::endl;
            exit(1);
    }
}

MoleculeSpace::MoleculeSpace(const u_int64_t moleculeNum, const MoleculeDistributionType distributionType, const MoleculeSpaceBorderType borderType, std::vector<std::shared_ptr<UserCell>>& cells,
                             const u_int32_t ID, const double _D)
  : width(SimulationSettings::MOLECULE_FIELD_X_LEN)
  , height(SimulationSettings::MOLECULE_FIELD_Y_LEN)
  , depth(SimulationSettings::MOLECULE_FIELD_Z_LEN)
  , dr((double)SimulationSettings::FIELD_X_LEN / (double)width)
  // , dr(10.0)
  , moleculeNum(moleculeNum)
  , borderType(borderType)
  // , moleculeSpace(make_vector<double>({ (size_t)width, (size_t)height, (size_t)depth }))
  , D(_D)
  , cells(cells)
  , ID(ID)
{
    std::mt19937 randGen(0); // TODO: シード値を変更できるようにする
    moleculeSpace      = make_vector<double>({ (size_t)(width + 2), (size_t)(height + 2), (size_t)(depth + 2) });
    deltaMoleculeSpace = make_vector<double>({ (size_t)(width + 2), (size_t)(height + 2), (size_t)(depth + 2) });

    switch (distributionType) {
        case MoleculeDistributionType::UNIFORM: {
            std::cout << "uniform" << std::endl;
            std::uniform_int_distribution<int32_t> randX(0, width - 1);
            std::uniform_int_distribution<int32_t> randY(0, height - 1);
            std::uniform_int_distribution<int32_t> randZ(0, depth - 1);
            std::uniform_real_distribution<double> randMolecule(0.0, 1.0);

            // for (int64_t i = 0; i < moleculeNum; i++) {
            //     int32_t x = randX(randGen);
            //     int32_t y = randY(randGen);
            //     int32_t z = randZ(randGen);

            //     moleculeSpace[x + 1][y + 1][z + 1] += 40.0; // 境界部分からずらす
            // }

            for (int32_t x = 1; x <= width; x++) {
                for (int32_t y = 1; y <= height; y++) {
                    for (int32_t z = 1; z <= depth; z++) {
                        moleculeSpace[x][y][z] = randMolecule(randGen);
                    }
                }
            }

            break;
        }
        case MoleculeDistributionType::GAUSSIAN: {
            std::cout << "gaussian" << std::endl;
            std::normal_distribution<double> randX(width / 2.0, 5.0);
            std::normal_distribution<double> randY(height / 2.0, 5.0);
            std::normal_distribution<double> randZ(depth / 2.0, 5.0);

            for (int64_t i = 0; i < moleculeNum; i++) {
                int32_t x, y, z;

                do {
                    x = randX(randGen);
                } while (!(0 <= x && x < width));

                do {
                    y = randY(randGen);
                } while (!(0 <= y && y < height));

                do {
                    z = randZ(randGen);
                } while (!(0 <= z && z < depth));

                moleculeSpace[x + 1][y + 1][z + 1] += 1.0; // 境界部分からずらす
            }
            break;
        }
        case MoleculeDistributionType::POINT: {
            moleculeSpace[width / 2 + 1][height / 2 + 1][depth / 2 + 1] = moleculeNum;
            break;
        }
        default:
            std::cerr << "DistributionType is Wrong: " << NAMEOF_ENUM(distributionType) << std::endl;
            break;
    }
}

MoleculeSpace::~MoleculeSpace()
{
}

// void MoleculeSpace::calcConcentrationDiff() noexcept
// {
//     for (int32_t x = 1; x <= width; x++) {
//         for (int32_t y = 1; y <= height; y++) {
//             for (int32_t z = 1; z <= depth; z++) {
//                 deltaMoleculeSpace[x][y][z] = diffuse(x, y, z) + production(x, y, z) - decay(x, y, z) - advection(x, y, z);
//             }
//         }
//     }

//     // for (u_int32_t i = 0; i < cells.size(); i++) {
//     //     Cell* cell = cells[i];
//     //     int32_t x  = (int32_t)(cell->getPosition().x / dr) + 1;
//     //     int32_t y  = (int32_t)(cell->getPosition().y / dr) + 1;
//     //     int32_t z  = (int32_t)(cell->getPosition().z / dr) + 1;

//     //     deltaMoleculeSpace[x][y][z] += cell->emitMolecule(ID);
//     // }
// }

// void MoleculeSpace::nextStep() noexcept
// {
//     for (int32_t x = 1; x <= width; x++) {
//         for (int32_t y = 1; y <= height; y++) {
//             for (int32_t z = 1; z <= depth; z++) {
//                 moleculeSpace[x][y][z] += deltaMoleculeSpace[x][y][z];
//                 deltaMoleculeSpace[x][y][z] = 0.0;
//             }
//         }
//     }

//     setupBoundary(moleculeSpace, MoleculeSpaceBorderType::NEUMANN);
// }

void MoleculeSpace::calcConcentrationDiff() noexcept
{
    for (int32_t x = 1; x <= width; x++) {
        for (int32_t y = 1; y <= height; y++) {
            for (int32_t z = 1; z <= depth; z++) {
                deltaMoleculeSpace[x][y][z] = diffuse(x, y, z) + production(x, y, z) - decay(x, y, z) - advection(x, y, z);
            }
        }
    }

    const int32_t width  = SimulationSettings::FIELD_X_LEN;
    const int32_t height = SimulationSettings::FIELD_Y_LEN;
    const int32_t depth  = SimulationSettings::FIELD_Z_LEN;
    for (u_int32_t i = 0; i < cells.size(); i++) {
        std::shared_ptr<UserCell> cell = cells[i];

        int32_t x = (int32_t)((cell->getPosition().x + width / 2) / dr) + 1;
        int32_t y = (int32_t)((cell->getPosition().y + height / 2) / dr) + 1;
        int32_t z = (int32_t)((cell->getPosition().z + depth / 2) / dr) + 1;

        deltaMoleculeSpace[x][y][z] += cell->emitMolecule(ID);
    }
}

void MoleculeSpace::nextStep() noexcept
{
    // #pragma omp parallel for
    for (int32_t x = 1; x <= width; x++) {
        for (int32_t y = 1; y <= height; y++) {
            for (int32_t z = 1; z <= depth; z++) {
                moleculeSpace[x][y][z] += deltaMoleculeSpace[x][y][z];
                // deltaMoleculeSpace[x][y][z] = 0.0;
            }
        }
    }

    setupBoundary(moleculeSpace, borderType);
}

double MoleculeSpace::getMoleculeNum(Vec3 pos) const noexcept
{
    const int32_t width  = SimulationSettings::FIELD_X_LEN;
    const int32_t height = SimulationSettings::FIELD_Y_LEN;
    const int32_t depth  = SimulationSettings::FIELD_Z_LEN;
    const double x       = (pos.x + width / 2) / dr + 1;
    const double y       = (pos.y + height / 2) / dr + 1;
    const double z       = (pos.z + depth / 2) / dr + 1;

    return moleculeSpace[(int)x][(int)y][(int)z];
}

void MoleculeSpace::print() const noexcept
{
    if (depth == 1) {
        for (int x = 0; x <= width + 1; x++) {
            for (int y = 0; y <= height + 1; y++) {
                std::cout << moleculeSpace[x][y][1] << " ";
            }
            std::cout << std::endl;
        }

        return;
    }

    for (int x = 1; x <= width; x++) {
        for (int y = 1; y <= height; y++) {
            for (int z = 1; z <= depth; z++) {
                std::cout << moleculeSpace[x][y][z] << " ";
            }
            std::cout << "|";
        }
        std::cout << std::endl;
    }
}
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
    return D * (moleculeSpace[x + 1][y][z] + moleculeSpace[x - 1][y][z]    //
                + moleculeSpace[x][y + 1][z] + moleculeSpace[x][y - 1][z]  //
                + moleculeSpace[x][y][z + 1] + moleculeSpace[x][y][z - 1]) //
           + (1.0 - 6.0 * D) * moleculeSpace[x][y][z];
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
            for (int32_t x = 1; x <= width; x++) {
                for (int32_t y = 1; y <= height; y++) {
                    ms[x][y][0]         = ms[x][y][1];
                    ms[x][y][depth + 1] = ms[x][y][depth];
                }
            }
            for (int32_t x = 1; x <= width; x++) {
                for (int32_t z = 1; z <= depth; z++) {
                    ms[x][0][z]          = ms[x][1][z];
                    ms[x][height + 1][z] = ms[x][height][z];
                }
            }
            for (int32_t y = 1; y <= height; y++) {
                for (int32_t z = 1; z <= depth; z++) {
                    ms[0][y][z]         = ms[1][y][z];
                    ms[width + 1][y][z] = ms[width][y][z];
                }
            }
            break;

        case MoleculeSpaceBorderType::DIRICHLET:
            for (int32_t x = 1; x <= width; x++) {
                for (int32_t y = 1; y <= height; y++) {
                    ms[x][y][0]         = 0;
                    ms[x][y][depth + 1] = 0;
                }
            }
            for (int32_t x = 1; x <= width; x++) {
                for (int32_t z = 1; z <= depth; z++) {
                    ms[x][0][z]          = 0;
                    ms[x][height + 1][z] = 0;
                }
            }
            for (int32_t y = 1; y <= height; y++) {
                for (int32_t z = 1; z <= depth; z++) {
                    ms[0][y][z]         = 0;
                    ms[width + 1][y][z] = 0;
                }
            }
            break;

        case MoleculeSpaceBorderType::PBC:
            for (int32_t x = 1; x <= width; x++) {
                for (int32_t y = 1; y <= height; y++) {
                    ms[x][y][0]         = ms[x][y][depth];
                    ms[x][y][depth + 1] = ms[x][y][1];
                }
            }
            for (int32_t x = 1; x <= width; x++) {
                for (int32_t z = 1; z <= depth; z++) {
                    ms[x][0][z]          = ms[x][height][z];
                    ms[x][height + 1][z] = ms[x][1][z];
                }
            }
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

MoleculeSpace::MoleculeSpace(int64_t moleculeNum, MoleculeDistributionType distributionType)
  : width(SimulationSettings::MOLECULE_FIELD_X_LEN)
  , height(SimulationSettings::MOLECULE_FIELD_Y_LEN)
  , depth(SimulationSettings::MOLECULE_FIELD_Z_LEN)
  , dr((double)SimulationSettings::FIELD_X_LEN / (double)width)
  , moleculeNum(moleculeNum)
  // , moleculeSpace(make_vector<double>({ (size_t)width, (size_t)height, (size_t)depth }))
  , D(0.1)
{
    std::mt19937 randGen(0); // TODO: シード値を変更できるようにする
    moleculeSpace = make_vector<double>({ (size_t)(width + 2), (size_t)(height + 2), (size_t)(depth + 2) });

    switch (distributionType) {
        case MoleculeDistributionType::UNIFORM: {
            std::cout << "uniform" << std::endl;
            std::uniform_int_distribution<int32_t> randX(0, width - 1);
            std::uniform_int_distribution<int32_t> randY(0, height - 1);
            std::uniform_int_distribution<int32_t> randZ(0, depth - 1);

            for (int64_t i = 0; i < moleculeNum; i++) {
                int32_t x = randX(randGen);
                int32_t y = randY(randGen);
                int32_t z = randZ(randGen);

                moleculeSpace[x + 1][y + 1][z + 1] += 1.0; // 境界部分からずらす
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

// XXX: 多分このあたりが原因でセグフォ出る
void MoleculeSpace::nextStep() noexcept
{
    auto nextMoleculeSpace = make_vector<double>({ (size_t)width + 2, (size_t)height + 2, (size_t)depth + 2 });

    for (int32_t x = 1; x <= width; x++) {
        for (int32_t y = 1; y <= height; y++) {
            for (int32_t z = 1; z <= depth; z++) {
                nextMoleculeSpace[x][y][z] = diffuse(x, y, z) + production(x, y, z) - decay(x, y, z) - advection(x, y, z);
            }
        }
    }

    setupBoundary(nextMoleculeSpace, MoleculeSpaceBorderType::NEUMANN);

    moleculeSpace = nextMoleculeSpace;
}

void MoleculeSpace::print() const noexcept
{
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
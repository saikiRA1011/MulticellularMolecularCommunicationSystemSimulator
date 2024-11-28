#include "Simulation.hpp"

// TODO: cellsをスマートポインタの配列にする。

/**
 * @brief 基本となるコンストラクタ。
 * @details
 * 各乱数生成器と遠隔力フィールドを初期化する。また、標準出力のストリームバッファを保存しておく。
 * @note
 * 例えば、(-8/2, 8/2)でランダムな値を生成すると長さ8の配列に収まるようになる。
 * {[-4, -3), [-3, -2), [-2, -1), [-1, 0), [0, 1), [1, 2), [2, 3), [3, 4)}
 */
Simulation::Simulation()
  : cellList()
  , randomCellPosX(-SimulationSettings::FIELD_X_LEN / 2, SimulationSettings::FIELD_X_LEN / 2)
  , randomCellPosY(-SimulationSettings::FIELD_Y_LEN / 2, SimulationSettings::FIELD_Y_LEN / 2)
// , aroundCellSetList(SimulationSettings::FIELD_Y_LEN, std::unordered_set<int32_t>())
{
    consoleStream = std::cout.rdbuf();
}

/**
 * @brief いまのところ何もしない。
 *
 */
Simulation::~Simulation()
{
}

/**
 * @brief 設定ファイルに出力画像のサイズなどを書き込む。
 *
 */
void Simulation::exportConfig() const
{
    std::ofstream outputfile("config.txt");
    outputfile << SimulationSettings::FIELD_X_LEN << std::endl;
    outputfile << SimulationSettings::FIELD_Y_LEN << std::endl;
    outputfile << SimulationSettings::SIM_STEP << std::endl;

    const double TIME_PER_FILE = SimulationSettings::DELTA_TIME * (double)SimulationSettings::OUTPUT_INTERVAL_STEP;

    outputfile << TIME_PER_FILE << std::endl;

    outputfile.close();
}

/**
 * @brief 各セルをランダムな座標で初期化する。
 *
 */
void Simulation::initCells() noexcept
{
    for (int32_t i = 0; i < SimulationSettings::CELL_NUM; i++) {
        double xPos = randomCellPosX(rand_gen);
        double yPos = randomCellPosY(rand_gen);
        UserCell c(CellType::WORKER, xPos, yPos, 10.0);
        cells.push_back(std::make_shared<UserCell>(c));
    }
}

/**
 * @brief ファイルにヘッダ情報を出力する。
 *
 */
void Simulation::printHeader() const noexcept
{
    std::cout << "ID\ttypeID\tX\tY\tZ\tVx\tVy\tVz\tR\tN_contact\tContact_IDs" << std::endl;
}

/**
 * @brief ファイルに現在のすべてのCell情報を出力する。
 *
 * @param time
 */
void Simulation::printCells(int32_t time) const
{
    std::ostringstream sout;
    sout << std::setfill('0') << std::setw(5) << time;

    std::string outputPath = "./result/cells_" + sout.str();
    std::ofstream ofs(outputPath);
    std::cout.rdbuf(ofs.rdbuf()); // 標準出力の出力先を指定ファイルに変更

    printHeader();
    for (int32_t i = 0; i < (int32_t)cells.size(); i++) {
        if (cells[i]->getCellType() == CellType::NONE)
            continue;
        cells[i]->printCell();
    }

    std::cout.rdbuf(consoleStream);
}

void Simulation::setCellList() noexcept
{
    debugCounter++;

    for (int32_t i = 0; i < (int32_t)cells.size(); i++) {
        cellList.addCell(cells[i]);
    }
}

/**
 * @brief  与えられたCellに対して他のCellから働く力を計算する。
 *
 * @param c
 * @return Vec3
 * @details Cellから働く力は遠隔力と近隣力の2つで構成される。さらに、近接力は体積排除効果と接着力の2つに分類される。
 */
Vec3 Simulation::calcCellCellForce(std::shared_ptr<UserCell> c) const noexcept
{
    Vec3 force = Vec3::zero();

    auto aroundCellList = cellList.aroundCellList(c);

    for (auto i : aroundCellList) {
        if (cells[i]->getCellType() == CellType::NONE || cells[i]->getCellType() == CellType::DEAD)
            continue;

        force += calcRemoteForce(c, cells[i]);
    }
    force = force.normalize();

    for (auto i : aroundCellList) {
        if (cells[i]->getCellType() == CellType::NONE || cells[i]->getCellType() == CellType::DEAD)
            continue;
        force += calcVolumeExclusion(c, cells[i]);
    }

    return force;
}

/**
 * @brief 与えられたCellに対して働く遠隔力を計算する。O(n^2)
 *
 * @param c
 * @return Vec3
 * @details @f{eqnarray*}{
 * F = \sum_i \frac{c(C - C_i)}{|C-C_i|}  *
 * e^{(-|C-C_i|/\lambda)}
 * @f}
 */
Vec3 Simulation::calcRemoteForce(std::shared_ptr<UserCell> c1, std::shared_ptr<UserCell> c2) const noexcept
{
    Vec3 force                   = Vec3::zero();
    constexpr double COEFFICIENT = 1.0;
    const Vec3 diff              = c1->getPosition() - c2->getPosition();
    const double dist            = diff.length();
    constexpr double LAMBDA      = 30.0;
    const double weight          = c2->getWeight() * c1->getWeight();

    // d = |C1 - C2|
    // F += c (C1 - C2) / d * e^(-d/λ)
    force += -diff.normalize().timesScalar(weight).timesScalar(COEFFICIENT).timesScalar(std::exp(-dist / LAMBDA));

    return force;
}

/**
 * @brief 与えられたCellに働く体積排除効果による力を計算する。O(n^2)
 *
 * @param c
 * @return Vec3
 * @details @f{eqnarray*}{
 * F = \sum_i
 * @f}
 */
Vec3 Simulation::calcVolumeExclusion(std::shared_ptr<UserCell> c1, std::shared_ptr<UserCell> c2) const noexcept
{
    Vec3 force        = Vec3::zero();
    const Vec3 diff   = c1->getPosition() - c2->getPosition();
    const double dist = diff.length();
    // const double weight               = c2->getWeight() * c1->getWeight();
    const double sumRadius = c1->getRadius() + c2->getRadius();
    // const double overlapDist          = c1->getRadius() + c2->getRadius() - dist;
    constexpr double ELIMINATION_BIAS = 10.0;
    constexpr double ADHESION_BIAS    = 0.4;

    if (dist < sumRadius) {
        // force += diff.normalize().timesScalar(std::pow(1.8, overlapDist)).timesScalar(BIAS);
        force += diff.normalize().timesScalar(pow(1.0 - dist / sumRadius, 2)).timesScalar(ELIMINATION_BIAS);
        force -= diff.normalize().timesScalar(pow(1.0 - dist / sumRadius, 2)).timesScalar(ADHESION_BIAS);
    }

    return force;
}

void Simulation::stepPreprocess() noexcept
{
    for (auto cell : cells) {
        cell->initForce();
    }
}

void Simulation::stepEndProcess() noexcept
{
}

/**
 * @brief 指定したCellにかかるすべての力を計算する。O(n^2)
 *
 * @param c
 * @return Vec3
 */
Vec3 Simulation::calcForce(std::shared_ptr<UserCell> c) const noexcept
{
    Vec3 force = Vec3::zero();

    force += calcCellCellForce(c);
    // force += calcRemoteForce(c);
    // force += calcVolumeExclusion(c);

    return force;
}

/**
 * @brief すべてのCellに力を加えた後、それぞれのCellの位置を更新する。
 *
 * @return int32_t
 * @details Cellの数が多いので、スレッドを用いて並列処理を行う。
 */
int32_t Simulation::nextStep() noexcept
{
    if (SimulationSettings::USE_CELL_LIST) {
        cellList.resetGrid();
        setCellList();
    }

    Vec3 force = Vec3::zero();

// XXX: スレッド数を増やしてもメモリアクセスがボトルネックになってしまう。
#pragma omp parallel for num_threads(8) schedule(dynamic) private(force)
    for (int32_t i = 0; i < (int32_t)cells.size(); i++) {
        if (cells[i]->getCellType() == CellType::DEAD || cells[i]->getCellType() == CellType::NONE)
            continue;

        force = calcCellCellForce(cells[i]);
        cells[i]->addForce(force);
    }

    for (auto cell : cells) {
        cell->nextStep();
    }

    return 0;
}

/**
 * @brief シミュレーションを実行する。
 *
 * @return int32_t
 * @details Cellの位置更新と情報出力を繰り返すだけ。
 */
int32_t Simulation::run()
{
    std::cout << "Open MP max threads: " << omp_get_max_threads() << std::endl;

    printCells(0);
    auto sumTime = 0;

    std::cout << "initialized." << std::endl;

    for (int32_t step = 1; step < SimulationSettings::SIM_STEP; step++) {
        auto start = std::chrono::system_clock::now();

        stepPreprocess();
        nextStep();
        stepEndProcess();

        const bool willOut = (step % SimulationSettings::OUTPUT_INTERVAL_STEP) == 0;
        if (willOut) {
            printCells(step / SimulationSettings::OUTPUT_INTERVAL_STEP);
        }
        const bool wasOut = willOut;

        auto end  = std::chrono::system_clock::now();
        auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::cout << "step: " << step << "  " << msec << "msec" << (wasOut ? " Outputed" : "") << std::endl;
        sumTime += msec;
    }

    const double averageTime = (double)sumTime / (double)SimulationSettings::SIM_STEP;
    std::cout << "Initial cell count : " << SimulationSettings::CELL_NUM << "    average processing time : " << averageTime << std::endl;

    return 0;
}

/**
 * @brief
 * なくてもいい。Pythonに情報を渡す都合上必要かもしれなかった。(使っていない)
 *
 * @return int32_t
 */
int32_t Simulation::getFieldLen()
{
    return SimulationSettings::FIELD_X_LEN;
}
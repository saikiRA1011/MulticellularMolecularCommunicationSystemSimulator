#include "Simulation.hpp"

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
  , randomCellPosX(-FIELD_X_LEN / 2, FIELD_X_LEN / 2)
  , randomCellPosY(-FIELD_Y_LEN / 2, FIELD_Y_LEN / 2)
// , aroundCellSetList(FIELD_Y_LEN, std::unordered_set<int32_t>())
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
    outputfile << FIELD_X_LEN << std::endl;
    outputfile << FIELD_Y_LEN << std::endl;
    outputfile << SIM_STEP << std::endl;

    outputfile.close();
}

/**
 * @brief 各セルをランダムな座標で初期化する。
 *
 */
void Simulation::initCells() noexcept
{
    for (int32_t i = 0; i < CELL_NUM; i++) {
        double xPos = randomCellPosX(rand_gen);
        double yPos = randomCellPosY(rand_gen);
        Cell c(i, 0, xPos, yPos, 10.0);
        cells.push_back(c);
    }
}

/**
 * @brief ファイルにヘッダ情報を出力する。
 *
 */
void Simulation::printHeader() const noexcept
{
    std::cout << "ID\tX\tY\tZ\tVx\tVy\tVz\tR\tN_contact\tContact_IDs" << std::endl;
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
        cells[i].printCell();
    }

    std::cout.rdbuf(consoleStream);
}

void Simulation::setCellList() noexcept
{
    for (int32_t i = 0; i < (int32_t)cells.size(); i++) {
        cellList.addCell(&cells[i]);
    }
}

/**
 * @brief  与えられたCellに対して他のCellから働く力を計算する。
 *
 * @param c
 * @return Vec3
 * @details Cellから働く力は遠隔力と近隣力の2つで構成される。さらに、近接力は体積排除効果と接着力の2つに分類される。
 */
Vec3 Simulation::calcCellCellForce(Cell& c) const noexcept
{
    Vec3 force = Vec3::zero();

    std::vector<Cell*> aroundCells = cellList.aroundCellList(c);

    for (int32_t i = 0; i < (int32_t)aroundCells.size(); i++) {
        Cell* cell        = aroundCells[i];
        const Vec3 diff   = c.getPosition() - cell->getPosition();
        const double dist = diff.length();

        constexpr double LAMBDA      = 30.0;
        constexpr double COEFFICIENT = 0.5;
        const double weight          = cell->getWeight() * c.getWeight();

        // d = |C1 - C2|
        // F += c (C1 - C2) / d * e^(-d/λ)
        force += -diff.normalize().timesScalar(weight).timesScalar(COEFFICIENT).timesScalar(std::exp(-dist / LAMBDA));
    }

    force = force.normalize();

    for (int32_t i = 0; i < (int32_t)aroundCells.size(); i++) {
        Cell* cell        = aroundCells[i];
        const Vec3 diff   = c.getPosition() - cell->getPosition();
        const double dist = diff.length();

        constexpr double ELIMINATION_BIAS = 10.0;
        constexpr double ADHESION_BIAS    = 0.4;
        const double sumRadius            = c.radius + cell->radius;
        const double overlapDist          = c.radius + cell->radius - dist;

        if (dist < sumRadius) {
            // force += diff.normalize().timesScalar(std::pow(1.8, overlapDist)).timesScalar(BIAS);
            force += diff.normalize().timesScalar(pow(1.0 - dist / sumRadius, 2)).timesScalar(ELIMINATION_BIAS);
            force -= diff.normalize().timesScalar(pow(1.0 - dist / sumRadius, 2)).timesScalar(ADHESION_BIAS);
        }
    }

    force = force.timesScalar(DELTA_TIME);

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
Vec3 Simulation::calcRemoteForce(Cell& c) const noexcept
{
    Vec3 force = Vec3::zero();

    std::vector<Cell*> aroundCells = cellList.aroundCellList(c);

    for (int32_t i = 0; i < (int32_t)aroundCells.size(); i++) {
        Cell* cell = aroundCells[i];

        Vec3 diff   = c.getPosition() - cell->getPosition();
        double dist = diff.length();

        constexpr double LAMBDA      = 30.0;
        constexpr double COEFFICIENT = 0.5;

        // d = |C1 - C2|
        // F += c (C1 - C2) / d * e^(-d/λ)
        force += -diff.normalize().timesScalar(COEFFICIENT).timesScalar(std::exp(-dist / LAMBDA));
    }

    force = force.timesScalar(DELTA_TIME);

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
Vec3 Simulation::calcVolumeExclusion(Cell& c) const noexcept
{
    Vec3 force = Vec3::zero();

    std::vector<Cell*> aroundCells = cellList.aroundCellList(c);

    for (int32_t i = 0; i < (int32_t)aroundCells.size(); i++) {
        Cell* cell = aroundCells[i];

        constexpr double BIAS = 10.0;

        const Vec3 diff          = c.getPosition() - cell->getPosition();
        const double dist        = diff.length();
        const double sumRadius   = c.radius + cell->radius;
        const double overlapDist = c.radius + cell->radius - dist;

        if (dist < c.radius + cell->radius) {
            // force += diff.normalize().timesScalar(std::pow(1.8, overlapDist)).timesScalar(BIAS);
            force += diff.normalize().timesScalar(pow(1.0 - overlapDist / sumRadius, 2)).timesScalar(BIAS);
        }
    }

    force = force.timesScalar(DELTA_TIME);

    return force;
}

/**
 * @brief 指定したCellにかかるすべての力を計算する。O(n^2)
 *
 * @param c
 * @return Vec3
 */
Vec3 Simulation::calcForce(Cell& c) const noexcept
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
    cellList.resetGrid();
    setCellList();

    // threadを使うよりもopenMPを利用したほうが速い
    //#pragma omp parallel
    for (int i = 0; i < CELL_NUM; i++) {
        Vec3 force;
        force = calcForce(cells[i]);
        cells[i].addForce(force);
    }

    // std::vector<std::thread> threads;
    // for (int i = 0; i < CELL_NUM; i++) {
    //     Vec3 force;

    //     // force = calcRemoteForce(cells[i]);
    //     // cells[i].addForce(force);

    //     threads.emplace_back([&, i] {
    //         force = calcForce(cells[i]);
    //         cells[i].addForce(force);
    //     });
    // }

    // // すべてのスレッドが終了するのを待つ
    // for (auto& t : threads) {
    //     t.join();
    // }

    for (int32_t cellID = 0; cellID < CELL_NUM; cellID++) {
        cells[cellID].nextStep();
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
    printCells(0);

    for (int32_t step = 1; step < SIM_STEP; step++) {
        auto start = std::chrono::system_clock::now();
        nextStep();
        printCells(step);
        auto end = std::chrono::system_clock::now();

        auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::cout << "step: " << step << "  " << msec << "msec" << std::endl;
    }

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
    return FIELD_X_LEN;
}
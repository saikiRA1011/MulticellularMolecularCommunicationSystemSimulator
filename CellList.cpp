#include "CellList.hpp"
#include "Vec3.hpp"

CellList::CellList()
{
    init();
}

CellList::~CellList()
{
}

void CellList::init()
{
    constexpr int32_t CELL_GRID_LEN_X = FIELD_X_LEN / GRID_SIZE_MAGNIFICATION;
    constexpr int32_t CELL_GRID_LEN_Y = FIELD_Y_LEN / GRID_SIZE_MAGNIFICATION;

    cellsInGrid.resize(CELL_GRID_LEN_Y);
    for (int32_t y = 0; y < CELL_GRID_LEN_Y; y++) {
        cellsInGrid[y].resize(CELL_GRID_LEN_X);
        for (int32_t x = 0; x < CELL_GRID_LEN_X; x++) {
            cellsInGrid[y][x] = std::vector<Cell*>();
        }
    }
}

/**
 * @brief 指定したCellの周囲にあるCellのIDリストを返す。
 *
 * @param c
 * @return std::vector<int>
 * @note CHECK_WIDTHはcalcRemoteForceのLAMBDAより大きくするのが理想。
 */
std::vector<Cell*> CellList::aroundCellList(const Cell& c) const
{
    std::vector<Cell*> aroundCells;
    constexpr int32_t CHECK_GRID_WIDTH = (SEARCH_RADIUS + GRID_SIZE_MAGNIFICATION - 1) / GRID_SIZE_MAGNIFICATION; // 切り上げの割り算

    Vec3 pos = c.getPosition();

    const int32_t scaledY = (pos.y + FIELD_Y_LEN / 2) / GRID_SIZE_MAGNIFICATION;
    const int32_t scaledX = (pos.x + FIELD_X_LEN / 2) / GRID_SIZE_MAGNIFICATION;

    constexpr int32_t GRID_X_WIDTH = FIELD_X_LEN / GRID_SIZE_MAGNIFICATION;
    constexpr int32_t GRID_Y_WIDTH = FIELD_Y_LEN / GRID_SIZE_MAGNIFICATION;

    for (int32_t y = scaledY - CHECK_GRID_WIDTH; y <= scaledY + CHECK_GRID_WIDTH; y++) {
        for (int32_t x = scaledX - CHECK_GRID_WIDTH; x <= scaledX + CHECK_GRID_WIDTH; x++) {
            // 範囲外の場合はスキップ
            if (x < 0 || GRID_X_WIDTH <= x || y < 0 || GRID_Y_WIDTH <= y) {
                continue;
            }

            for (int i = 0; i < (int32_t)cellsInGrid[y][x].size(); i++) {
                Cell* cell = cellsInGrid[y][x][i];

                const bool isSame    = (cell->id == c.id);
                const bool isInRange = c.getPosition().dist(cell->getPosition()) <= SEARCH_RADIUS;

                // 調べるセルが自分自身、あるいは距離がSEARCH_RADIUSより離れている場合はスキップ
                if (isSame || !isInRange) {
                    continue;
                }

                aroundCells.push_back(cell);
            }
        }
    }

    return aroundCells;
}

void CellList::resetGrid() noexcept
{
    constexpr int32_t GRID_X_WIDTH = FIELD_X_LEN / GRID_SIZE_MAGNIFICATION;
    constexpr int32_t GRID_Y_WIDTH = FIELD_Y_LEN / GRID_SIZE_MAGNIFICATION;

    // グリッドに保存されているCellのリストを初期化する。O(n^2) nは1辺の長さ
    for (int32_t y = 0; y < GRID_Y_WIDTH; y++) {
        for (int32_t x = 0; x < GRID_X_WIDTH; x++) {
            cellsInGrid[y][x].clear();
        }
    }
}

void CellList::addCell(Cell* cell)
{
    Vec3 pos = cell->getPosition();

    const int32_t scaledY = (pos.y + FIELD_Y_LEN / 2) / GRID_SIZE_MAGNIFICATION;
    const int32_t scaledX = (pos.x + FIELD_X_LEN / 2) / GRID_SIZE_MAGNIFICATION;

    cellsInGrid[scaledY][scaledX].push_back(cell);
}
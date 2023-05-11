/**
 * @file UserCell.hpp
 * @author Takanori Saiki
 * @brief シミュレーション固有のCellの設計をここに示す
 * @version 0.1
 * @date 2023-04-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "core/Cell.hpp"

class UserCell : public Cell
{
  public:
    bool checkWillDivide() const noexcept override;
    bool checkWillDie() const noexcept override;
    void metabolize() noexcept override;
    int32_t die() noexcept override;
};

// TODO: 以下のメモをチェックして細胞間相互作用をまとめるクラスを作成する
/*
            CellType
            ↓      ↓
Cell → UserCell → CellCellForce
           ↓           ↓
       Simulator → UserSimulator

CellCellForceは細胞間相互作用を定義するクラス。
UserSimulatorでそれぞれの種類の細胞がどのCellCellForcesを用いるかを条件分岐で定義する。›$

UserCellで扱う分裂や死滅のモデルはどこで定義する？
細胞ごとに挙動を変えたい。
*/
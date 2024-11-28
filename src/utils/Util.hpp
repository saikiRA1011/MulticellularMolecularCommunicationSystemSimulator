/**
 * @file Util.hpp
 * @author Takanori Saiki
 * @brief プロジェクトの様々な箇所で使いそうなものをまとめておく。
 * @version 0.1
 * @date 2022-06-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include <random>
#include <sstream>
#include <string>
#include <vector>

template<typename T>
using FieldLine = std::vector<T>; //!< 2次元フィールドの行
template<typename T>
using Field = std::vector<FieldLine<T>>; //!< 2次元フィールド

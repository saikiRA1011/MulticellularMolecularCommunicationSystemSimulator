/**
 * @file SegTest.cpp
 * @author Takanori Saiki
 * @brief SegmentTreeのテスト
 * @version 0.1
 * @date 2022-06-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "SegmentTree.hpp"
#include <iostream>
#include <vector>

using namespace std;

int main(void)
{
    int h, w;
    cin >> h >> w;
    vector<vector<int>> a(h, vector<int>(w));

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            cin >> a[i][j];
        }
    }

    SegmentTree<int, int> seg(h, w, 0);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            seg.set(i, j, a[i][j]);
        }
    }

    cout << endl;

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            cout << seg.get(i, j) << " ";
        }
        cout << endl;
    }
    cout << endl;

    seg.build();

    for (int i = -2; i < h + 2; i++) {
        for (int j = -2; j < w + 2; j++) {
            cout << seg.query(0, 0, i + 1, j + 1) << " ";
        }
        cout << endl;
    }

    return 0;
}
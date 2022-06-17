#pragma once

#include <cmath>
#include <vector>

template<typename T, typename F>
class SegmentTree
{
    public:
    int H, W;
    std::vector<T> seg;
    const F f(const T& a, const T& b);
    const T I;

    SegmentTree(int h, int w, const T& i);
    void init(int h, int w);
    void set(int h, int w, const T& x);
    void build();

    T get(int h, int w) const;
    T operator()(int h, int w) const;

    void update(int h, int w, const T& x);

    T _inner_query(int h, int w1, int w2);
    T query(int h1, int w1, int h2, int w2);

    private:
    int id(int h, int w) const;
};

template<typename T, typename F>
SegmentTree<T, F>::SegmentTree(int h, int w, const T& i)
  : I(i)
{
    init(h, w);
}

template<typename T, typename F>
const F SegmentTree<T, F>::f(const T& a, const T& b)
{
    return a + b;
}

template<typename T, typename F>
void SegmentTree<T, F>::init(int h, int w)
{
    H = 1;
    W = 1;

    while (H < h)
        H <<= 1;
    while (W < w)
        W <<= 1;

    seg.assign(4 * H * W, I);
}

template<typename T, typename F>
void SegmentTree<T, F>::set(int h, int w, const T& x)
{
    seg[id(h + H, w + W)] = x;
}

template<typename T, typename F>
void SegmentTree<T, F>::build()
{
    // w in [W, 2W)
    for (int w = W; w < 2 * W; w++) {
        for (int h = H - 1; h; h--) {
            seg[id(h, w)] = f(seg[id(2 * h + 0, w)], seg[id(2 * h + 1, w)]);
        }
    }
    // h in [0, 2H)
    for (int h = 0; h < 2 * H; h++) {
        for (int w = W - 1; w; w--) {
            seg[id(h, w)] = f(seg[id(h, 2 * w + 0)], seg[id(h, 2 * w + 1)]);
        }
    }
}

template<typename T, typename F>
T SegmentTree<T, F>::get(int h, int w) const
{
    return seg[id(h + H, w + W)];
}

template<typename T, typename F>
T SegmentTree<T, F>::operator()(int h, int w) const
{
    return seg[id(h + H, w + W)];
}

template<typename T, typename F>
void SegmentTree<T, F>::update(int h, int w, const T& x)
{
    h += H;
    w += W;
    seg[id(h, w)] = x;

    for (int i = h >> 1; i; i >>= 1) {
        seg[id(i, w)] = f(seg[id(2 * i + 0, w)], seg[id(2 * i + 1, w)]);
    }
    for (; h; h >>= 1) {
        for (int j = w >> 1; j; j >>= 1) {
            seg[id(h, j)] = f(seg[id(h, 2 * j + 0)], seg[id(h, 2 * j + 1)]);
        }
    }
}

template<typename T, typename F>
T SegmentTree<T, F>::_inner_query(int h, int w1, int w2)
{
    T res = I;
    for (; w1 < w2; w1 >>= 1, w2 >>= 1) {
        if (w1 & 1) {
            res = f(res, seg[id(h, w1)]);
            w1++;
        }
        if (w2 & 1) {
            --w2;
            res = f(res, seg[id(h, w2)]);
        }
    }
    return res;
}

template<typename T, typename F>
T SegmentTree<T, F>::query(int h1, int w1, int h2, int w2)
{
    h1 = std::max(h1, 0);
    w1 = std::max(w1, 0);
    if (h1 >= h2 || w1 >= w2)
        return I;

    T res = I;
    h1 += H, h2 += H, w1 += W, w2 += W;
    for (; h1 < h2; h1 >>= 1, h2 >>= 1) {
        if (h1 & 1) {
            res = f(res, _inner_query(h1, w1, w2));
            h1++;
        }
        if (h2 & 1) {
            --h2;
            res = f(res, _inner_query(h2, w1, w2));
        }
    }
    return res;
}

template<typename T, typename F>
int SegmentTree<T, F>::id(int h, int w) const
{
    return h * 2 * W + w;
}
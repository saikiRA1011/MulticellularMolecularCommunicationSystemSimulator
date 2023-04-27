enum class CellType
{
    NONE,     // 死滅・消滅した細胞など。CellのID更新には影響しない
    TMP,      // 一時的に作成する細胞。CellのID更新には影響しない
    DEAD,     // 死滅した細胞。細胞自体は残しておきたい場合に使う
    WORKER,   // 運動する細胞など
    TARGET,   // 何かしらの目標となる細胞
    SENDER,   // 何かしらの情報を送る細胞
    RECEIVER, // 何かしらの情報を受け取る細胞
    ENEMY,    // がん細胞など、特に敵対的な細胞
    // other...
};
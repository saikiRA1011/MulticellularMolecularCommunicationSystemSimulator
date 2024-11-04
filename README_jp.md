# Multicellular Molecular Communication System Simulator
![gif](https://github.com/saikiRA1011/CellNetworkShapeSimulation/blob/main/readme_img/sim.gif)

# Overview
分子通信シミュレーションのための汎用シミュレータであり、ユーザがシミュレーションのパラメータや力学作用を定義することでさまざまなモデルに対応したシミュレーションを実行することができます。

tutorial  
jp -> https://github.com/saikiRA1011/CellNetworkShapeSimulation/blob/main/tutorial/out/tutorial.pdf

# Requirement
- gcc(version 11以降)
- Python3系(必要ライブラリはrequirements.txtから取得可能)
- pip
- make
- mac OS or Linux (環境次第ではWindowsも可能)

Pythonの必要ライブラリは以下のコマンドで一括インストールできます。  
`pip install -r requirements.txt`

# Usage
### Step 1
シミュレータを`make`する。  
```sh
$ make
```

### Step 2
シミュレータを実行する。  
```sh
$ make run
```

### Step 3
実行結果を動画に変換する。  
```sh
$ make convert
```

### Step 4
変換した動画を確認する。  
```sh
$ make open
```
  
  
ただし、Step 1 ~ Step 4は`make all`でまとめて実行することもできます。
また、各種コマンドは`make help`から確認することができます。

## シミュレーションモデルの作成方法
`src/UserSimulation.cpp(.hpp)`と`src/UserCell.cpp(.hpp)`にモデルを記述します。  <!-- 現在作成可能なモデルは細胞間に働く力学モデル(calcCellCellForce)のみです。   -->
シミュレーションのパラメータは`src/config.yaml`に記述します。ユーザが記述するプログラムは主にこの3つです。  
<!-- UserSimulationの親クラスはSimulationであり、ユーザが使用できる変数(つまり、publicかprotectedの変数)はcells(シミュレーション中のすべてのCellを保存したリスト)とcellList(CellListクラスのインスタンス)です。  
あるCell cの付近のすべてのCell(のポインタ)を取得したい場合は、`cellList.aroundCellList(c)`によって取得可能です。ただし、すべてのCellを力学モデルの計算対象にしたい場合は、cellsの方を利用したほうが良いです。   -->
サンプルの力学モデルは`UserSimulation::calcCellCellForce()`を参考にすると良いです。

# Features
- CellListのアルゴリズムを利用することによりシミュレーションを高速に実行することが可能となっています。  
- Makefileに定義されたコマンドにより、ユーザはディレクトリの構造を深く考えることなく、コンパイルから結果の確認までを簡単に実行することができます。  
- シミュレーションの結果をテキスト(`./result/*`)に出力しているため、ユーザが独自にビジュアライザを作成し、再利用することもできます。

# Tips
ドキュメントの生成にDoxygenを利用しているので、Doxygenの環境を用意することで各クラスやメソッドの説明を簡単に見ることができます。

# References
- S. Imanaka and T. Nakano, ``Algorithm Design for Multicellular Molecular Communication Simulations,'' 7th International Conference on Knowledge Innovation and Invention 2024 (ICKII 2024), Aichi, Aug. 2024.
- S. Imanaka, T. Saiki and T. Nakano, ``Accelerating Multicellular Molecular Communication Simulations Using the Barnes-Hut Algorithm,'' 2024 IEEE 4th International Conference on Electronic Communications,Internet of Things and Big Data (ICIEB 2024),  Taiwan, April 2024.
- T. Saiki and T. Nakano, ``A Simulation Platform for Dynamic Multicellular Molecular Communication Systems ,'' 2024 IEEE 4th International Conference on Electronic Communications,Internet of Things and Big Data (ICIEB 2024),  Taiwan, April 2024.
- S. Takanori and T. Nakano, ``Design and Implementation of a General-purpose Multicellular Molecular Communication Simulator,'' 14th EAI International Conference on Bio-inspired Information and Communications Technologies (BICT 2023), Okinawa, April 2023.
- T. Saiki and T. Nakano, ``Design and Implementation of a Multicellular Molecular Communication Simulator," full paper, in Proc. 2022 Joint 12th International Conference on Soft Computing and Intelligent Systems and 23rd International Symposium on Advanced Intelligent Systems (SCIS&ISIS 2022), Mie, Nov.--Dec. 2022.

# Author
所属 : https://sites.google.com/c.info.eng.osaka-cu.ac.jp/ect/%E3%83%9B%E3%83%BC%E3%83%A0

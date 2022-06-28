# CellNetworkShapeSimulation
![gif](https://github.com/saikiRA1011/CellNetworkShapeSimulation/blob/main/readme_img/sim.gif)

# Overview
分子通信シミュレーションのための汎用シミュレータであり、ユーザがシミュレーションのパラメータや力学作用を定義することでさまざまなモデルに対応したシミュレーションを実行することができます。

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
シミュレータをmakeする。  
`make`

### Step 2
シミュレータを実行する。  
`make run`

### Step 3
実行結果を動画に変換する。  
`make convert`

### Step 4
変換した動画を確認する。  
`make open`
  
  
ただし、Step 1 ~ Step 4は`make all`でまとめて実行することもできます。
また、各種コマンドは`make help`から確認することができます。

## シミュレーションモデルの作成方法
UserSimulation.cpp(.hpp)にモデルを記述します。現在作成可能なモデルは細胞間に働く力学モデル(calcCellCellForce)のみです。  
シミュレーションのパラメータはSimulationSettings.hppに記述します。ユーザが記述するプログラムは主にこの2つです。  
UserSimulationの親クラスはSimulationであり、ユーザが使用できる変数(つまり、publicかprotectedの変数)はcells(シミュレーション中のすべてのCellを保存したリスト)とcellList(CellListクラスのインスタンス)です。  
あるCell cの付近のすべてのCell(のポインタ)を取得したい場合は、`cellList.aroundCellList(c)`によって取得可能です。ただし、すべてのCellを力学モデルの計算対象にしたい場合は、cellsの方を利用したほうが良いです。  
サンプルの力学モデルは`Simulation::calcCellCellForce(Cell &c)`を参考にすると良いです。

# Features
- CellListのアルゴリズムを利用することによりシミュレーションを高速に実行することが可能となっています。  
- Makefileに定義されたコマンドにより、ユーザはディレクトリの構造を深く考えることなく、コンパイルから結果の確認までを簡単に実行することができます。  
- シミュレーションの結果をテキスト(./result/*)に出力しているため、ユーザが独自にビジュアライザを作成し、再利用することもできます。

# Tips
ドキュメントの生成にDoxygenを利用しているので、Doxygenの環境を用意することで各クラスやメソッドの説明を簡単に見ることができます。

# Author
所属 : https://sites.google.com/c.info.eng.osaka-cu.ac.jp/ect/%E3%83%9B%E3%83%BC%E3%83%A0
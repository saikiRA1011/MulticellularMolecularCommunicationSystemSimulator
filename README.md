# CellNetworkShapeSimulation
![gif](https://github.com/saikiRA1011/CellNetworkShapeSimulation/blob/segtree/readme_img/sim.gif)

# Overview
分子通信シミュレーションのための汎用シミュレータです。ユーザがシミュレーションのパラメータや力学作用を定義することでさまざまなモデルに対応したシミュレーションを実行することができます。

# Requirement
- gcc
- Python3系(必要ライブラリはrequirements.txtから取得可能)
- pip
- make
- mac OS or Linux (環境次第ではWindowsも可能)

Pythonの必要ライブラリは以下のコマンドで一括インストールできます。
`pip install -r requirements.txt`

# Usage
### Step 1
シミュレータをmakeします。  
`make`

### Step 2
シミュレータを実行します。  
`make run`

### Step 3
実行結果を動画に変換します。  
`make convert`

### Step 4
変換した動画を確認します。  
`make open`
  
  
ただし、Step 1 ~ Step 4は`make all`でまとめて実行することもできます。
また、各種コマンドは`make help`から確認することができます。

# Features
- CellListのアルゴリズムを利用することによりシミュレーションを高速に実行することが可能です。  
- Makefileに定義されたコマンドにより、ユーザはディレクトリの構造を深く考えることなく、コンパイルから結果の確認までを簡単に実行することができます。  
- シミュレーションの結果をテキスト(./result/*)に出力しているため、ユーザが独自にビジュアライザを作成し、再利用することもできます。

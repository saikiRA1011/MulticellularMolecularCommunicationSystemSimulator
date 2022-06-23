# CellNetworkShapeSimulation
![gif](https://github.com/saikiRA1011/CellNetworkShapeSimulation/blob/segtree/readme_img/sim.gif)

## Overview
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
## Step 2
シミュレータをmakeします。
`make`

## Step 3
シミュレータを実行します。
`make run`

## Step 4
実行結果を動画に変換します。
`make convert`

## Step 5
変換した動画を確認します。
`make open`

ただし、Step 2 ~ Step 5は`make all`でまとめて実行することもできます。
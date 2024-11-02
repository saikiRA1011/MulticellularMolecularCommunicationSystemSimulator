# Multicellular Molecular Communication System Simulator
![gif](https://github.com/saikiRA1011/CellNetworkShapeSimulation/blob/main/readme_img/sim.gif)

# Overview
It is a general-purpose simulator for multicellular molecular communication simulations and allows users to define simulation parameters and dynamics to run simulations for various models.

## Other Languages README
[日本語](https://github.com/saikiRA1011/CellNetworkShapeSimulation/blob/main/README_jp.md)

## Tutorial  
[日本語](https://github.com/saikiRA1011/CellNetworkShapeSimulation/blob/main/tutorial/out/tutorial.pdf)

# Requirement
- gcc(version 11 or later)
- Python3 series(Required libraries can be obtained from requirements.txt)
- pip
- make
- mac OS or Linux (Windows is possible depending on the environment)

The required Python libraries can be installed in a batch with the following command.  
```sh
$ pip install -r requirements.txt
```

# Usage
### Step 1
`make` the simulator.  
```sh
$ make
```

### Step 2
Run the simulator.  
```sh
$ make run
```

### Step 3
Convert the execution results to video.  
```sh
$ make convert
```

### Step 4
Check the converted video.  
```sh
$ make open
```
  
  
However, Step 1 ~ Step 4 can also be run together with `make all`.
You can also check the various commands from `make help`.

## How to create a simulation model
Describe the model in `src/UserSimulation.cpp(.hpp)` and `src/UserCell.cpp(.hpp)`.  <!-- 現在作成可能なモデルは細胞間に働く力学モデル(`calcCellCellForce()`)、細胞分裂の条件チェック(``)のみです。 -->
The simulation parameters are written in `src/config.yaml`. These are the three main programs written by the user.  
<!-- `UserSimulation`の親クラスは`Simulation`であり、ユーザが使用できる変数(つまり、publicかprotectedの変数)は`cells`(シミュレーション中のすべての`Cell`を保存したリスト)と`cellList`(`CellList`クラスのインスタンス)です。   -->
<!-- あるCell cの付近のすべてのCell(のポインタ)を取得したい場合は、`cellList.aroundCellList(c)`によって取得可能です。ただし、すべてのCellを力学モデルの計算対象にしたい場合は、cellsの方を利用したほうが良いです。   -->
You can refer to ` UserSimulation::calcCellCellForce()` for a sample dynamics model.

# Features
- The CellList algorithm makes it possible to run simulations at high speed.  
- The commands defined in the Makefile allow the user to easily compile and check the results without having to think too much about the directory structure.  
- The simulation results can be saved as text (`./result/*`), allowing users to create and reuse their own visualizers.

# Tips
Since Doxygen is used to generate the documentation, you can easily view the description of each class and method by preparing the Doxygen environment.

# Reference
- S. Imanaka and T. Nakano, ``Algorithm Design for Multicellular Molecular Communication Simulations,'' 7th International Conference on Knowledge Innovation and Invention 2024 (ICKII 2024), Aichi, Aug. 2024.
- S. Imanaka, T. Saiki and T. Nakano, ``Accelerating Multicellular Molecular Communication Simulations Using the Barnes-Hut Algorithm,'' 2024 IEEE 4th International Conference on Electronic Communications,Internet of Things and Big Data (ICIEB 2024),  Taiwan, April 2024.
- T. Saiki and T. Nakano, ``A Simulation Platform for Dynamic Multicellular Molecular Communication Systems ,'' 2024 IEEE 4th International Conference on Electronic Communications,Internet of Things and Big Data (ICIEB 2024),  Taiwan, April 2024.
- S. Takanori and T. Nakano, ``Design and Implementation of a General-purpose Multicellular Molecular Communication Simulator,'' 14th EAI International Conference on Bio-inspired Information and Communications Technologies (BICT 2023), Okinawa, April 2023.
- T. Saiki and T. Nakano, ``Design and Implementation of a Multicellular Molecular Communication Simulator," full paper, in Proc. 2022 Joint 12th International Conference on Soft Computing and Intelligent Systems and 23rd International Symposium on Advanced Intelligent Systems (SCIS&ISIS 2022), Mie, Nov.--Dec. 2022.

# Author
Affiliation : https://sites.google.com/c.info.eng.osaka-cu.ac.jp/ect/%E3%83%9B%E3%83%BC%E3%83%A0

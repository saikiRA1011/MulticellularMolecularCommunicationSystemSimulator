#!/bin/sh

rm ./result/*
rm ./image/*

g++-11 SimMain.cpp -o SimMain -std=c++20 -Wall -Wextra -fopenmp && echo "SimMain compiled" && ./SimMain

python3 create_image.py
python3 img2video.py
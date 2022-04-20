#!/bin/sh

rm ./result/*
rm ./image/*

g++ SimMain.cpp -o SimMain -std=c++20 -Wall -Wextra && echo "SimMain compiled" && ./SimMain

python3 create_image.py
python3 img2video.py
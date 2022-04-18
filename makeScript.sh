#!/bin/sh

rm ./result/*
rm ./image/*

g++ SimMain.cpp -o SimMain -std=c++20 -Wall -Wextra

if [ $? ]; then
    echo "Compilation Success"
else
    echo "Compilation Failed"
    exit 1
fi

./SimMain

python3 create_image.py
python3 img2video.py
from glob import glob
import matplotlib.animation as animation
import matplotlib.cm as cm
import matplotlib.pyplot as plt
from matplotlib import colormaps as cmaps
import cv2
import numpy as np
import math

file_list = glob('molecule_result/0/molecule_*')
file_list = sorted(file_list)
cmap = cmaps.get_cmap('Reds')

for file in file_list:
    with open(file, 'r') as f:
        # スペース区切りでfloatのデータを読み込む
        field2D = f.readlines()
        deg2 = []
        for line in field2D:
            field_lines = list(map(float, line.split(" ")[:-1]))
            deg2.append(field_lines)

        XY = np.array(deg2).squeeze().T
        XY = np.array(XY, dtype=np.uint32)
        color_img = np.array(cmap(XY*20, bytes=True), dtype=np.uint8)

        # 10倍に拡大
        # color_img = color_img.repeat(10, axis=0).repeat(10, axis=1)

        color_img = cv2.cvtColor(color_img, cv2.COLOR_RGBA2BGR)
        img = color_img

        cell_num = int(file[-10:].split('_')[1])

        path = f'./figs/cells_{str(cell_num).zfill(5)}.png'
        is_ok = cv2.imwrite(path, img)

        print(f'{path} : {is_ok}')

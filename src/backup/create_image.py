import cv2
import numpy as np
import glob
import math
from visualize_setting import Setting

files = sorted(glob.glob('./result/*'))

with open('config.txt', 'r') as f:
    img_w_len = int(f.readline())
    img_h_len = int(f.readline())
    step_num = int(f.readline())
    time_per_file = float(f.readline())

IMAGE_LEN = 1024

time_delta_sec = time_per_file * 60

for file in files:
    img = np.full((IMAGE_LEN, IMAGE_LEN, 3), 255, dtype=np.uint8)
    cv2.circle(img, (IMAGE_LEN//2, IMAGE_LEN//2), IMAGE_LEN//2, (0, 0, 0))
    cv2.line(img, (0, IMAGE_LEN//2), (IMAGE_LEN, IMAGE_LEN//2), (0, 0, 0))
    cv2.line(img, (IMAGE_LEN//2, 0), (IMAGE_LEN//2, IMAGE_LEN), (0, 0, 0))

    with open(file, 'r') as f:
        cells = f.read().split('\n')

    # cell[id, typeID, x, y, z, vx, vy, vz, r, contact num, contact id...]
    scale = IMAGE_LEN/img_w_len
    for cell in cells[1:-1]:
        cell = cell.split('\t')
        x = float(cell[2])*scale+IMAGE_LEN/2
        y = float(cell[3])*scale+IMAGE_LEN/2
        z = float(cell[4])*scale+IMAGE_LEN/2
        r = float(cell[8])*scale

        if x < 0 or x > IMAGE_LEN or y < 0 or y > IMAGE_LEN:
            print('out of range')

        cv2.circle(img, (math.floor(x), math.floor(y)),
                   math.floor(r), (0, 200, 0), thickness=1)

    cv2.imwrite(f'./image/cells_{file[-5:]}.png', img)

    print(file)


exit()

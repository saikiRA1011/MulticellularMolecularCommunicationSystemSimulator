import cv2
import numpy as np
import glob
import math

files = sorted(glob.glob('./result/*'))

with open('config.txt', 'r') as f:
    img_w_len = int(f.readline())
    img_h_len = int(f.readline())

IMAGE_LEN = img_w_len

CELL_RADIUS = 2

for file in files:
    img = np.full((IMAGE_LEN, IMAGE_LEN, 3), 255, dtype=np.uint8)
    cv2.circle(img, (IMAGE_LEN//2, IMAGE_LEN//2), IMAGE_LEN//2, (0, 0, 0))
    cv2.line(img, (0, IMAGE_LEN//2), (IMAGE_LEN, IMAGE_LEN//2), (0, 0, 0))
    cv2.line(img, (IMAGE_LEN//2, 0), (IMAGE_LEN//2, IMAGE_LEN), (0, 0, 0))

    with open(file, 'r') as f:
        cells = f.read().split('\n')

    for cell in cells[1:-1]:
        cell = cell.split('\t')
        x = float(cell[1])+IMAGE_LEN/2
        y = float(cell[2])+IMAGE_LEN/2

        if x < 0 or x > IMAGE_LEN or y < 0 or y > IMAGE_LEN:
            print('out of range')

        cv2.circle(img, (math.floor(x), math.floor(y)),
                   CELL_RADIUS, (0, 200, 0), thickness=1)

    cv2.imwrite(f'./image/cells_{file[-5:]}.png', img)

    print(file)


exit()

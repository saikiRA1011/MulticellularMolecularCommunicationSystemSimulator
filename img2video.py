from distutils.log import error
import sys
import cv2
import os
import glob

# encoder
fourcc = cv2.VideoWriter_fourcc('m', 'p', '4', 'v')

img_len = 1024

# output
video = cv2.VideoWriter('./video/out.mp4', fourcc, 20.0, (img_len, img_len))

file_paths = sorted(glob.glob('./image/cells_*'))

for file in file_paths:
    img = cv2.imread(file)

    if img is None:
        print(f"{file} is can't read.")
        break

    video.write(img)
    print(file)

video.release()

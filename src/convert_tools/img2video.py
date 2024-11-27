import cv2
import glob

# encoder
fourcc = cv2.VideoWriter_fourcc('m', 'p', '4', 'v')

# output

file_paths = sorted(glob.glob('./image/cells_*'))
base_img = cv2.imread(file_paths[0])
img_w_len = base_img.shape[1]
img_h_len = base_img.shape[0]

video = cv2.VideoWriter('./video/out.mp4', fourcc,
                        20.0, (img_w_len, img_h_len))

for file in file_paths:
    img = cv2.imread(file)

    if img is None:
        print(f"{file} is can't read.")
        break

    video.write(img)
    print(file)

video.release()

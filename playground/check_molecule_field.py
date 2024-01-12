import matplotlib.pyplot as plt
import numpy as np
import matplotlib.cm as cm
import matplotlib.animation as animation
from glob import glob

ims = []

files = glob("molecule_result/0/molecule_*")
files = sorted(files)

fig, ax = plt.subplots(figsize=(1, 1), dpi=128)
is_first = True
time = 0

# TODO: カラーバーのスケールを固定する
for path in files:
    if time % 1 == 0:
        deg3 = []
        plt.cla()
        fig.clear()

        with open(path, "r") as f:
            field3D = f.readlines()

            for field_surface in field3D:
                deg2 = []
                field_lines = field_surface.split("|")[:-1]

                for line in field_lines:
                    elements = list(map(float, line.split(" ")[:-1]))

                    deg2.append(elements)

                deg3.append(deg2)

        XY = np.array(deg3).squeeze().T

        title = ax.text(0.5, 1.05, path, bbox={
                        'facecolor': 'w', 'alpha': 0.5, 'pad': 5})
        img = plt.imshow(XY, interpolation='hanning', cmap='plasma')
        # fig.colorbar(img)
        plt.clim(0, 5)
        plt.axis("off")
        ax.spines['left'].set_color('none')
        ax.spines['right'].set_color('none')
        ax.spines['top'].set_color('none')

        fig.subplots_adjust(left=0, right=1, bottom=0, top=1)
        plt.savefig("molecule_image/" + path.split("/")[-1] + ".png")

        print(f'out : {path}')

    time += 1

import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
from matplotlib.ticker import ScalarFormatter
import argparse
import sys
import json


def parse(filename):
    print(filename)

    with open(filename) as f:
        data = json.load(f)
        method = data["method"]
        x = np.array(data["particles"]["x"])
        y = np.array(data["particles"]["y"])
        a_x = np.array(data["particles"]["a_x"])
        a_y = np.array(data["particles"]["a_y"])
        mass = np.array(data["particles"]["mass"])
    particles = np.stack([x, y, a_x, a_y, mass], axis=-1)
    return (
        particles,
        method,
        # (grid_x, grid_y, width, height)
    )


def main(filenames):
    plasma = matplotlib.colormaps["plasma"]
    plasma = ListedColormap(plasma.colors[: -len(plasma.colors) // 8])

    for filename in filenames:
        # filename always ends in ".json"
        outfile = f"{filename[:-5]}.png"
        particles, method = parse(filename)
        x, y, ax, ay, mass = particles.T
        size = 50 * (mass - np.min(mass)) / np.max(mass) + 5
        color = np.linalg.norm(np.stack([ax, ay]), axis=0)
        # truncate the colormap so contrast against white is better
        plt.scatter(x, y, s=size, c=color, cmap=plasma)
        quiver = plt.quiver(x, y, ax / color, ay / color, color, cmap=plasma, alpha=0.5)
        plt.title(f"particles and accelerations for {method} method")
        # move axis scaling into the labels
        # see https://stackoverflow.com/a/45766598
        ax = plt.gca()
        ax.figure.canvas.draw()
        formatter = ScalarFormatter(useMathText=True)
        ax.xaxis.set_major_formatter(formatter)
        ax.yaxis.set_major_formatter(formatter)
        x_scale = int(ax.xaxis.get_offset_text().get_text()[2:])
        y_scale = int(ax.yaxis.get_offset_text().get_text()[2:])
        ax.xaxis.offsetText.set_visible(False)
        ax.yaxis.offsetText.set_visible(False)
        plt.xlabel(f"x coordinate ($m \\times 10^{{{x_scale}}}$)")
        plt.ylabel(f"y coordinate ($m \\times 10^{{{y_scale}}}$)")
        plt.colorbar(quiver, label="acceleration ($m/s^2$)")
        plt.tight_layout()
        plt.savefig(outfile)
        plt.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="visualizer",
        description="creates matplotlib visualization of the provided files.",
    )

    parser.add_argument("filenames", nargs="*")
    args = parser.parse_args(sys.argv[1:])
    main(args.filenames)

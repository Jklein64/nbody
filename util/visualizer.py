import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
from matplotlib.ticker import ScalarFormatter
import argparse
import sys
import json

plasma = matplotlib.colormaps["plasma"]
PLASMA = ListedColormap(plasma.colors[: -len(plasma.colors) // 8])


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
        grid = data["grid"]
    particles = np.stack([x, y, a_x, a_y, mass], axis=-1)
    return (particles, method, grid)


def make_arrow_plot(filename):
    # filename always ends in ".json"
    outfile = f"{filename[:-5]} arrows.png"
    particles, method, _ = parse(filename)
    x, y, ax, ay, mass = particles.T
    size = 50 * (mass - np.min(mass)) / np.max(mass) + 5
    color = np.linalg.norm(np.stack([ax, ay]), axis=0)
    # truncate the colormap so contrast against white is better
    plt.scatter(x, y, s=size, c=color, cmap=PLASMA)
    quiver = plt.quiver(x, y, ax / color, ay / color, color, cmap=PLASMA, alpha=0.5)
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


def make_grid_plot(filename):
    # filename always ends in ".json"
    outfile = f"{filename[:-5]} grid.png"
    particles, method, grid = parse(filename)
    x, y, _, _, mass = particles.T
    density = np.zeros((grid["nrows"], grid["ncols"]))
    for i in range(len(mass)):
        x_i = int((x[i] - grid["x"]) / grid["scale"] + 0.5)
        y_i = int((y[i] - grid["y"]) / grid["scale"] + 0.5)
        density[y_i, x_i] += mass[i]
    plt.title(f"particle densities for {method} method")
    plt.xlabel("grid column index")
    plt.ylabel("grid row index")
    colormesh = plt.pcolormesh(np.where(density < 1e-8, np.nan, density))
    plt.scatter(
        (x - grid["x"]) / grid["scale"] + 0.5,
        (y - grid["y"]) / grid["scale"] + 0.5,
        marker=".",
        color="red",
    )
    plt.colorbar(colormesh, label="density ($kg/m^3$)")
    plt.gca().set_aspect("equal")
    plt.savefig(outfile)
    plt.close()


def main(filenames):
    for filename in filenames:
        make_arrow_plot(filename)
        make_grid_plot(filename)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="visualizer",
        description="creates matplotlib visualization of the provided files.",
    )

    parser.add_argument("filenames", nargs="*")
    args = parser.parse_args(sys.argv[1:])
    main(args.filenames)

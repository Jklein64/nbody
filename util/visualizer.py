import numpy as np
import matplotlib.pyplot as plt
import argparse
import sys


def parse(filename):
    with open(filename) as f:
        method = f.readline().strip()
        n = int(f.readline())
        grid_x, grid_y, width, height = f.readline().split()
        particles = np.zeros((n, 5))
        for i in range(n):
            x, y, ax, ay, m = f.readline().split()
            particles[i] = [float(v) for v in (x, y, ax, ay, m)]
    return (
        particles,
        method,
        # (grid_x, grid_y, width, height)
    )


def main(filenames):
    for filename in filenames:
        # filename always ends in ".out"
        outfile = f"{filename[:-4]}.png"
        particles, method = parse(filename)
        x, y, ax, ay, mass = particles.T
        size = 50 * (mass - np.min(mass)) / np.max(mass) + 5
        color = np.linalg.norm(np.stack([ax, ay]), axis=0)
        plt.scatter(x, y, s=size, c=color)
        print(ax, ay)
        quiver = plt.quiver(x, y, ax / color, ay / color, color, alpha=0.5)
        plt.title(f"particles and accelerations for {method} method")
        plt.xlabel("x coordinate (m)")
        plt.ylabel("y coordinate (m)")
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

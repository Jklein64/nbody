# nbody

_Jason Klein and Arnav Muthiayen_

Massively parallel N-body simulation, accelerated with the Barnes-Hut algorithm and paralellized with MPI. This is our final project for CS 5220: Applied Parallel Computing.

This code takes grid size, particle count, frame count, and an interval as input and produces an animated gif showing how the particles would move over time. Grid cell indices refer to the centers of the cell, and particles have floating-point locations that get rounded to the containing cell during each update step.

## Tasks

- [ ] Naive (order $n^2$) implementation
- [ ] Visualization as particle density heatmap
- [ ] Barnes-Hut using Z-Morton-based quadtree
- [ ] MPI shenanigans

## Resources

- https://periodicos.univali.br/index.php/acotb/article/view/19492
- https://dl.acm.org/doi/10.1145/169627.169640
- https://github.com/alexgbrandt/Parallel-NBody

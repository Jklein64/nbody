visualization framework



save a single floating point value for each coordinate for a certain number of frames separated by an interval. interval is fixed and known and hardcoded by us.

save to `.npy` or `.npz` file using [this C++ library](https://github.com/rogersce/cnpy)

load in a python script, apply a colormap/scaling, and then turn to gif with [this library](https://pypi.org/project/array2gif/)

Might also want to just skip python and use [this cpp library](https://github.com/charlietangora/gif-h) to write the gif files. will require a manual colorization though



the actual setup is going to be a big ass data structure similar to the architecture for p3. 

```c++
for (int i = 0; i < num_iterations; i++) {
    if (output && i % save_iter == 0) {
        if (rank == 0) {
            fwrite(h, sizeof(double), (nx + 1) * (ny + 1), fptr);
        }
    }

    step();
}
```

we're not using GPUs with MPI, so this is fine (no need for a transfer function)



basically there's a `solver_init`, `solver_step`, and `solver_free` function (or maybe I just make it a cpp class lol). the init function makes all the particles, the step function applies gravity, and the free deallocates them. this should be enough to get a basic demo running.

the next task would be to implement the barnes hut optimization inside of the step. make the tree every step. optimize that. that is considered "good serial"

then parallelize

some parameters we're going to want are grid dimensions, particle count, and sampler for initialization. visualization parameters include the number of frames to simulate and the number of frames between saves (< 0 => don't save)








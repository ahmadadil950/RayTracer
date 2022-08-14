# Ray-Tracer
A basic Ray tracer made from Peter Shirley's Ray tracing in one weekend series


## Installation

Install the project by using git to create a local repository

```bash
1. First clone the repo onto your local machine:
  git clone https://github.com/ahmadadil950/Ray-Tracer.git
2. Next enter into the project folder
  cd ray
3. Compile the project using the command below
  g++ g++ ray.cpp -o ray -fopenacc -O3 -fopt-info-optimized-omp -march=native
4. Finally in order to render the image, output the file stream to a ppm image format
  ./ray > "filename".ppm
```
    

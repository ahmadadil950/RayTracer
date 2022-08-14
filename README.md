# Ray-Tracer

A basic Ray tracer made from Peter Shirley's Ray tracing in one weekend series

In order to compile the project, C++17 or greater must be installed,
along with the GNU compiler



## Run Locally

Clone the project

```bash
  git clone https://github.com/ahmadadil950/RayTracer.git
```

Go to the project directory

```bash
  cd ray
```

Run program using GNU compiler

```bash
  g++ ray.cpp -o ray -fopenacc -O3 -fopt-info-optimized-omp -march=native
```

Finally in order to render the image, output the file stream to a ppm image format

```bash
  ./ray > "filename".ppm
```

Then to view image, use online ppm file viewer

```bash
https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html
```



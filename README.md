# Ray-Tracer

A basic Ray tracer made from Peter Shirley's Ray tracing in one weekend series

In order to compile the project, C++17 or greater must be installed,
along with the GNU compiler

## Cloning and Running Locally

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

## Downloading binary to run project

```bash
On the release tab download the ray binary
```

```bash
Once downloaded, move binary to a folder location of your choice and open the command line of your choice within that location 
```

```bash
Within the command line run ./ray > "filname.ppm" to convert the file to a ppm image
```

```bash
The image should be rendered in the location where the binary is located and to view it you can use https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html
```

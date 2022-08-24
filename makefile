OBJS	= ray.o
SOURCE	= ray.cpp
HEADER	= ray.h color.h camera.h hittable_list.h hittable.h material.h rt.h sphere.h vec3.h
OUT	= ray
CC	 = g++
FLAGS	 = -g -c -fopenacc -O3 -fopt-info-optimized-omp -march=native
LFLAGS	 = -fopenacc -fopenacc -O3 -fopt-info-optimized-omp -march=native
# -g option enables debugging mode 
# -c flag generates object code for separate files


all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)


# create/compile the individual files >>separately<<
ray.o: ray.cpp
	$(CC) $(FLAGS) ray.cpp

# run the program
run: $(OUT)
		./$(OUT)


# clean house
clean:
	rm -f $(OBJS) $(OUT)

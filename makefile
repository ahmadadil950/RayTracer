#OBJS specifies which files to compile as part of the project
OBJS =  ray.cpp

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
HEADER = ray.h color.h camera.h hittable_list.h hittable.h material.h rt.h sphere.h vec3.h

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -fopenacc -fopt-info-optimized-omp 

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = ray

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -o $(OBJ_NAME)
#include <iostream>
#include "ray.h"
#include "color.h"
#include "vec3.h"
#include "rt.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"


/*
Equation of sphere in vector form is: 
(P-C)*(P-C) = r^2 = (x−C_x)^2+(y−C_y)^2+(z−C_z)^2
C = (C_x,C_y_C_z)
P = (x,y,z)
“any point P that satisfies this equation is on the sphere”
When looking for t:
(P(t)−C)⋅(P(t)−C)=r^2
(A+tb−C)⋅(A+tb−C)=r^2

Fully expanded: t^2b⋅b+2tb⋅(A−C)+(A−C)⋅(A−C)−r^2=0
Vectors and r are known in the equation.
t -> unknown
solutions using quadratic formula:
positive (meaning two real solutions), 
negative (meaning no real solutions), 
or zero (meaning one real solution)
*/

/*Surface normal is a vector that is perpendicular to the surface at the point of intersection.
For sphere: the outward normal is in the direction of the hit point minus the center

A common trick used for visualizing normals (because it’s easy and somewhat intuitive to 
assume n is a unit length vector — so each component is 
between −1 and 1) is to map each component to the interval from 0 to 1, 
and then map x/y/z to r/g/b.*/
double hit_Sphere(const point3 &center, double radius, const ray& r)
{
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc,r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;
    if(discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        return(-half_b - Qrt(discriminant))/a;
    }
}

#pragma acc routine seq
color ray_color(const ray &r, const hittable& world, int depth)
{
    hit_record rec;
    if(depth <= 0)
    {
        return color(0,0,0);
    }
    if(world.hit(r, 0.001 , infinity ,rec ))
    {
        ray scattered;
        color attenuation;
        if(rec.mat_ptr->scatter(r,rec,attenuation,scattered))
        {
            return attenuation * ray_color(scattered,world,depth-1);
        }
        return color(0,0,0);
    }

    vec3 unit_Direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_Direction.y() + 1.0);
    return (1.0 - t) * color(1.0,1.0,1.0) + t*color(0.5,0.7,1.0);
}

hittable_list random_scene()
{
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5,0.5,0.5));

    world.add(make_shared<sphere>(point3(0,-1000.0,0), 1000.0, ground_material));
    #pragma acc parallel loop gang worker
    for(int x = -11; x < 11; x++)
    {
        #pragma acc loop vector
        for(int y = -11; y < 11; y++)
        {
            auto choose_mat = random_double();
            point3 center(x + 0.9 * random_double(), 0.2 , y + 0.9 * random_double());

            if((center - point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<material> sphere_material;


                if(choose_mat < 0.65)
                {
                    //diffusion
                    auto albedo = color::random(0.0,0.999) * color::random(0.0,0.999);
                    sphere_material = make_shared<lambertian>(albedo);

                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if(choose_mat < 0.8)
                {
                    // metal
                    auto albedo = color::random(0.0, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo,fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    //glass
                    sphere_material = make_shared<diaelectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    //middle large sphere
    auto material_1 = make_shared<diaelectric>(1.5);
    world.add(make_shared<sphere>(point3(0,1,0), 1.0, material_1));

    // farthest from camera large sphere
    auto material_2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material_2));
    
    // in front of camera large sphere
    auto material_3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material_3));



    return world;
}



int main()
{
    //Image parameters
    float width;
    float height;
    int user_image_width;
    int pixelSample;
    int lookfrom_x, lookfrom_y, lookfrom_z;

    /*std:: cerr: error output stream*/
    std::cerr << "Enter the aspect ratio width:" << std::endl;
    std::cin >> width;
    std::cerr << "Enter the aspect ratio height:" << std::endl;
    std::cin >> height; 
    const auto aspect_Ratio = width/height;
    std :: cerr << "Enter image width: " << std::endl;
    std:: cin >> user_image_width;
    const int image_width = user_image_width;
    const int image_height = static_cast<int>(image_width/aspect_Ratio);
    std::cerr << "Enter the number of samples for each pixel (EX: 500 is less noisy, but takes much longer)" << std::endl;
    std::cin >> pixelSample;
    std::cerr << "\n" << std::endl;
    const int samples_per_Pixel = pixelSample;
    const int max_depth = 50;

    //Camera
    std::cerr <<"Enter the x axis for the camera (controls left and right movement)" << std::endl;
    std::cin >> lookfrom_x;
    std::cerr <<"Enter the y axis for the camera (controls vertical movement)" << std::endl;
    std::cin >> lookfrom_y;
    std::cerr <<"Enter the z axis for the camera (controls zoom in movement)" << std::endl;
    std::cin >> lookfrom_z;
    point3 lookfrom(lookfrom_x,lookfrom_y,lookfrom_z);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20 , aspect_Ratio , aperture , dist_to_focus);

    // World parameters
    auto world = random_scene();
    //hittable_list world;
    //auto R = cos(pi/4);

    /*auto material_Ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_Center = make_shared<lambertian>(color(0.1,0.2,0.5));
    auto material_Left   = make_shared<diaelectric>(1.5);
    auto material_Right  = make_shared<metal>(color(0.8, 0.6, 0.2),0.0);

    world.add(make_shared<sphere>(point3(0.0,-100.5,-1.0), 100.0, material_Ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_Center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_Left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.45, material_Left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_Right));*/

    /*auto material_left = make_shared<lambertian>(color(0,0,1));
    auto material_right = make_shared<lambertian>(color(1,0,0));

    world.add(make_shared<sphere>(point3(-R,0,-1),R,material_left));
    world.add(make_shared<sphere>(point3(R,0,-1),R,material_right));*/





    //Camera
    /*auto viewport_height = 2.0;
    auto viewport_width = aspect_Ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0,0,0);
    auto horizontal = vec3(viewport_width, 0,0);
    auto vertical = vec3(0,viewport_height,0);
    auto lowerLeft_corner = origin - horizontal/2-vertical/2-vec3(0,0,focal_length);*/




    //progress indicator
    std::cout<<"P3\n" << image_width << ' ' << image_height << "\n255\n";

    //Render
    for(int i = image_height - 1; i>= 0; --i)
    {
        std::cerr << "\rScanLines remaining:" << i << ' ' << std::flush;
        
        //Rows
        for(int j = 0; j < image_width; ++j)
        {
            color pixel_Color(0,0,0);
            for(int s = 0; s<samples_per_Pixel; ++s)
            {
                auto u = (j + random_double()) / (image_width - 1);
                auto v = (i + random_double()) / (image_height - 1);

                ray r = cam.get_ray(u,v);
                pixel_Color += ray_color(r, world,max_depth);
            }
            write_Color(std::cout,pixel_Color,samples_per_Pixel);

        }
    }
    std::cerr << "\nDone.\n";
}
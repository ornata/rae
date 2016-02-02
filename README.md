#rae
baby's fist ray tracer. Assignment for a computer graphics course I'm taking.

## FEATURES IMPLEMENTED

- Several scenes are preset and can be chosen by setting an integer value then compiling.
- Currently, the scene outputs a fractal of cubes where every cube at an odd depth of recursion
 is a mirror. There are several point light sources in the scene which all cast shadows.
- Multiple spheres/instancing: Implemented an instance struct that allows you to
make copies of anything that derives from shape. (several scenes output multiple spheres,
one outputs multiple triangle meshes.
- Matrix transforms: Scaling, movement, and rotation along general axes are implemented by
transforming rays.
- Triangle mesh rendering: Reads in .mesh files OpenGL style (that is, with a triangle and vertex
buffer)
- Supersampling: Basic random sampling for anti-aliasing.
- Multiple point lights which all cast shadows: All stores in a std::vector of lights.
Lighting is based off of the rendering equation, which will allow for global illumination
to easily be added in the future.
- Reflective surfaces: Several scenes output mirror balls. Any shape type can be made a mirror
by setting its mirror flag to true.

## THINGS TO BE DONE

- Implement a kd-tree on triangles to speed up intersections
- Implement some sort of BVH (probably a uniform grid) on the entire scene to avoid firing off too many rays

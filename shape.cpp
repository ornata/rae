/*
* shape.cpp
* Contains implementations of functions for shapes from shape.hpp
* That includes constructors, hit functions output, etc.
*/

#include "shape.hpp"
#define COLLISION_EPS 0.0001f
/* ----- triangle ----- */ 

triangle::triangle(const vec3 &p0_, const vec3 &p1_, const vec3& p2_, const rgb &colour_) :
p0(p0_), p1(p1_), p2(p2_), colour(colour_)
{
    vec3 p0p1 = p1 - p0;
    vec3 p0p2 = p2 - p0;
}

/* 
* Check if r = origin + t * direction intersects with this triangle.
* Return true if it does, false otherwise
*
* Uses barycentric coordinates to define a plane. That is, we define a plane as
* p(alpha, beta, gamma) = alpha*a + beta*b + gamma*c
* where alpha + beta + gamma = 1
*
* PARAMETERS:
*   tmin: Minimum parameterization of r (the closest you can be to the camera)
*   tmax: Maximum parameterization of r (the farthest you can be from the camera)
*   record: Stores information to do with the intersection of r and this triangle, if it occurs
*/
bool triangle::hit(const ray &r, float tmin, float tmax, float time, hitRecord &record) const
{
    float tval; // parameter for the intersection with the triangle

    // edge from p1 -> p0
    float A = p0.x - p1.x;
    float B = p0.y - p1.y;
    float C = p0.z - p1.z; 

    // edge from p2 -> p0
    float D = p0.x - p2.x; 
    float E = p0.y - p2.y;
    float F = p0.z - p2.z;

    // components of ray
    float G = r.direction.x;
    float H = r.direction.y;
    float I = r.direction.z;

    // edge from ray to p0 (lower left point on triangle)
    float J = p0.x - r.origin.x;
    float K = p0.y - r.origin.y;
    float L = p0.z - r.origin.z;

    // precompute differences for computing beta, gamma, and t by Cramer's rule
    float EIHF = E*I - H*F;
    float GFDI = G*F - D*I;
    float DHEG = D*H - E*G;
    float denom = A*EIHF + B*GFDI + C*DHEG;

    float beta = (J*EIHF + K*GFDI + L*DHEG) / denom;

    // failed constraint 0 < beta < 1
    if (beta <= 0.0f || beta >= 1.0f) {
        return false;
    }

    float AKJB = A*K - J*B;
    float JCAL = J*C - A*L;
    float BLKC = B*L - K*C;

    float gamma = (I*AKJB + H*JCAL + G*BLKC) / denom;

    // failed constraint 0 < beta + gamma < 1
    // get this by plugging in alpha = 1 - beta - gamma into 
    // p(alpha,beta,gamma) = A*alpha + B*beta + C*gamma
    if (gamma <= 0.0f || beta + gamma >= 1.0f) {
        return false;
    }

    tval = -(F*AKJB + E*JCAL + D*BLKC) / denom; // dist of intersection

    if (tval >= tmin && tval <= tmax)
    {
        record.t = tval;
        record.normal = makeUnitVector(cross((p1-p0), (p2-p0)));
        record.colour = colour;
        return true;
    }
     
     return false; 
}

/* Check if r intersects with the given triangle. Return true if it does, false otherwise
*/
bool triangle::shadowHit(const ray &r, float tmin, float tmax, float time) const
{
    float tval;
    float A = p0.x - p1.x;
    float B = p0.y - p1.y;
    float C = p0.z - p1.z; 

    float D = p0.x - p2.x;
    float E = p0.y - p2.y;
    float F = p0.z - p2.z;

    float G = r.direction.x;
    float H = r.direction.y;
    float I = r.direction.z;

    float J = p0.x - r.origin.x;
    float K = p0.y - r.origin.y;
    float L = p0.z - r.origin.z;

    float EIHF = E * I - H * F;
    float GFDI = G * F - D * I;
    float DHEG = D * H - E * G;

    float denom = (A * EIHF + B * GFDI + C * DHEG);
    float beta = (J * EIHF + K * GFDI + L * DHEG) / denom;

    if (beta <= 0.0f || beta >= 1.0f) {
        return false;
    }

    float AKJB = A * K - J * B;
    float JCAL = J * C - A * L;
    float BLKC = B * L - K * C;

    float gamma = (I * AKJB + H * JCAL + G * BLKC) / denom;

    if (gamma <= 0.0f || gamma >= 1.0f) {
        return false;
    }

    tval = -(F * AKJB + E * JCAL + D * BLKC) / denom;

    return (tval >= tmin && tval <= tmax);
}

std::ostream &operator <<(std::ostream &out, const triangle &toString)
{
    out << "([" << toString.p0 << "], [" << toString.p1 << "], [" << toString.p2 << "], colour: [" << toString.colour << "])";
    return out;
}

/* ----- triangleMesh ---- */

/*
* Read in a mesh file and initialize a triangleMesh using it.
* 1st 4B of file: # vertices
* 2nd 4B of file: # triangles
* Next # vertices * sizeof(meshVertex) B: vertex data
* Next # triangles * sizeof(meshTriangle) B: triangle data
*/
triangleMesh::triangleMesh(std::string fname)
{
    FILE* meshFile = fopen(fname.c_str(), "rb");
    if (!meshFile) {
        std::cerr << "Could not open '" << fname << "'.\n";
        exit(-1);
    }

    // Read in the number of vertices and then the number of triangles
    if (fread(&nv, 4, 1, meshFile) < 1) {
        std::cerr << "ERROR: Could not load mesh file. \n";
        exit(-1);
    }

    if (fread(&nt, 4, 1, meshFile) < 1) {
        std::cerr << "ERROR: Could not load mesh file. \n";
        exit(-1);
    }

    // Allocate memory for the vertex and triangle arrays
    vertexArray = (meshVertex*) malloc(nv * sizeof(meshVertex));
    if (!vertexArray) {
        std::cerr << "ERROR: Could not allocate memory for vertexArray. (number of vertices = " << nv << ")\n";
        exit(-1);
    }

    triangleArray = (meshTriangle*) malloc(nt * sizeof(meshTriangle));
    if (!triangleArray) {
        std::cerr << "ERROR: Could not allocate memory for triangleArray. (number of triangles = " << nt << ")\n";
        exit(-1);
    }

    // Read in the vertexArray and the triangleArrays
    if (fread(vertexArray, sizeof(float), nv * 8, meshFile) < (nv * 8)) {
        std::cerr << "ERROR: Could not load mesh file. \n";
        exit(-1);
    }

    if (fread(triangleArray, sizeof(int), nt * 3, meshFile) < (nt * 3)) {
        std::cerr << "ERROR: Could not load mesh file. \n";
        exit(-1);
    }

    fclose(meshFile);
}

triangleMesh::~triangleMesh()
{
    free(vertexArray);
    free(triangleArray);
}

/* ----- sphere ----- */ 

sphere::sphere(const vec3 &centre_, float radius_, const rgb &colour_) :
centre(centre_), radius(radius_), colour(colour_)
{}

/* Check if r intersects with the given sphere. Return true if it does, false otherwise */
bool sphere::hit(const ray &r, float tmin, float tmax, float time, hitRecord &record) const
{
    vec3 tmp = r.origin - centre; 

    double a = dot(r.direction, r.direction);
    double b = dot(r.direction, tmp) * 2;
    double c = dot(tmp, tmp) - radius*radius;
    double discriminant = b*b - 4*a*c; // how many real solns?

    // does the ray intersect the sphere?
    if (discriminant > 0) {
        discriminant = sqrt(discriminant);
        double t = (-b - discriminant) / (2*a);

        // check for valid interval
        if (t < tmin) {
            t = (-b + discriminant) / (2*a);
        }

        // too far or too close
        if (t < tmin || t > tmax) {
            return false;
        }

        // have a valid hit
        record.t = t;
        record.normal = makeUnitVector(r.origin + t*r.direction - centre);
        record.colour = colour;
        return true;
    }

    else {
        return false;
    }
}

/* Check if r intersects with the given sphere. Return true if it does, false otherwise*/
bool sphere::shadowHit(const ray &r, float tmin, float tmax, float time) const
{
    vec3 tmp = r.origin - centre; // get vector from origin of ray to centre of sphere

    double a = dot(r.direction, r.direction);
    double b = 2 * dot(r.direction, tmp);
    double c = dot(tmp, tmp) - radius * radius;
    double discriminant = b * b - 4 * a * c; // how many real solns?

    // does the ray intersect the sphere?
    if (discriminant > 0) {
        discriminant = sqrt(discriminant);
        double t = (-b - discriminant)/(a+a);

        if (t < tmin) {
            t = (-b + discriminant)/(a+a);
        }

        if (t < tmin || t > tmax) {
            return false;
        }

        return true;
    }

    // discriminant didn't have real solns or was = 0 (so the ray "grazed by" the sphere)
    return false;
}

std::ostream &operator <<(std::ostream &out, const sphere &toString)
{
    out << "( c = " << toString.centre << ", r = " << toString.radius << ", colour = " << toString.colour << ")";
    return out;
}

/* ----- plane ----- */

plane::plane(const vec3 &pt_, const vec3 &n_, const vec3 &colour_) :
pt(pt_), n(makeUnitVector(n_)), colour(colour_)
{}

/* Check if r intersects with the plane */
bool plane::hit(const ray &r, float tmin, float tmax, float time, hitRecord &record) const
{

    float denom = dot(n, r.direction);

    if (fabs(denom - COLLISION_EPS) < COLLISION_EPS) {
        return false;
    }

    float t = dot(n, vec3(pt - r.origin)) / denom;

    if (t < tmin || t > tmax) { // bounds check
        return false;
    }

    record.t = t;
    record.normal = n;
    record.colour = colour;

    return true;                                                                                                                                                                                                                      

}

bool plane::shadowHit(const ray &r, float tmin, float tmax, float time) const
{
    float denom = dot(n, r.direction);
    if (denom - COLLISION_EPS < COLLISION_EPS) {
        return false;
    }

    float t = dot(n, vec3(pt - r.origin)) / denom;

    if (t < tmin || t > tmax) { // bounds check
        return false;
    }
    
    return true;  
}
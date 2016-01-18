#include "matrix.hpp"

tmat tmat::operator +=(const tmat &other)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m[i][j] += other.m[i][j];
        }
    }

    return *this;
}

tmat tmat::operator -=(const tmat &other)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m[i][j] -= other.m[i][j];
        }
    }

    return *this;
}

tmat tmat::operator *=(const tmat &other)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {

            float s = 0;

            for (int k = 0; k < 4; k++) {
                // m00 * n00 + m01 * n01 .. m03 * n03 for 0..3
                s+= m[i][k] * other.m[i][k];
            }

            // mij = dot(row i in m, col j in n)
            m[i][j] = s;
        }
    }

    return *this;
}

tmat tmat::operator *=(float s)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m[i][j] *= s;
        }
    }

    return *this;
}

tmat operator +(tmat l, const tmat &r)
{
    return l += r;
}

tmat operator -(tmat l, const tmat &r)
{
    return l -= r;
}

tmat operator *(tmat l, const tmat &r)
{
    return l *= r;
}

vec3 operator *(const tmat &l, vec3 r)
{
    r.x = l.m[0][0] * r.x + l.m[0][1] * r.y + l.m[0][2] * r.z;
    r.y = l.m[1][0] * r.x + l.m[1][1] * r.y + l.m[1][2] * r.z;
    r.z = l.m[2][0] * r.x + l.m[2][1] * r.y + l.m[2][2] * r.z;

    return r;
}

tmat operator *(tmat &l, float r)
{
    return l *= r;
}

float tmat::det() const
{
    float d = 0.0f;

    d += det3(m[1][1], m[1][2], m[1][3],
                    m[2][1], m[2][2], m[2][3],
                    m[3][1], m[3][2], m[3][3]);

    d -= det3(m[1][0], m[1][2], m[1][3],
                    m[2][0], m[2][2], m[2][3],
                    m[3][0], m[3][2], m[3][3]);

    d += det3(m[1][0], m[1][1], m[1][3],
                    m[2][0], m[2][1], m[2][3],
                    m[3][0], m[3][1], m[3][3]);

    d -= det3(m[1][0], m[1][1], m[1][2],
                    m[2][0], m[2][1], m[2][2],
                    m[3][0], m[3][1], m[3][2]);

    return d;
}

/* Returns the inverse of mat, assuming it is nonsingular*/
tmat inverse(const tmat &mat)
{

    tmat inv;
    float d = mat.det();

    // row 1
    inv.m[0][0] = det3(mat.m[1][1], mat.m[1][2], mat.m[1][3],
                       mat.m[2][1], mat.m[2][2], mat.m[2][3],
                       mat.m[3][1], mat.m[3][2], mat.m[3][3]) / d;

    inv.m[0][1] = -det3(mat.m[0][1], mat.m[0][2], mat.m[0][3],
                        mat.m[2][1], mat.m[2][2], mat.m[2][3],
                        mat.m[3][1], mat.m[3][2], mat.m[3][3]) / d;

    inv.m[0][2] = det3(mat.m[0][1], mat.m[0][2], mat.m[0][3],
                       mat.m[1][1], mat.m[1][2], mat.m[1][3],
                       mat.m[3][1], mat.m[3][2], mat.m[3][3]) / d;

    inv.m[0][3] = -det3(mat.m[0][1], mat.m[0][2], mat.m[0][3],
                        mat.m[1][1], mat.m[1][2], mat.m[1][3],
                        mat.m[2][1], mat.m[2][2], mat.m[2][3]) / d;

    // row 2
    inv.m[1][0] = -det3(mat.m[1][0], mat.m[1][2], mat.m[1][3],
                        mat.m[2][0], mat.m[2][2], mat.m[2][3],
                        mat.m[3][0], mat.m[3][2], mat.m[3][3]) / d;

    inv.m[1][1] = det3(mat.m[0][0], mat.m[0][2], mat.m[0][3],
                       mat.m[2][0], mat.m[2][2], mat.m[2][3],
                       mat.m[3][0], mat.m[3][2], mat.m[3][3]) / d;

    inv.m[1][2] = -det3(mat.m[0][0], mat.m[0][2], mat.m[0][3],
                        mat.m[1][0], mat.m[1][2], mat.m[1][3],
                        mat.m[3][0], mat.m[3][2], mat.m[3][3]) / d;

    inv.m[1][3] = det3(mat.m[0][0], mat.m[0][2], mat.m[0][3],
                       mat.m[1][0], mat.m[1][2], mat.m[1][3],
                       mat.m[2][0], mat.m[2][2], mat.m[2][3]) / d;

    // row 3
    inv.m[2][0] = det3(mat.m[1][0], mat.m[1][1], mat.m[1][3],
                       mat.m[2][0], mat.m[2][1], mat.m[2][3],
                       mat.m[3][0], mat.m[3][1], mat.m[3][3]) / d;

    inv.m[2][1] = -det3(mat.m[0][0], mat.m[0][1], mat.m[0][3],
                        mat.m[2][0], mat.m[2][1], mat.m[2][3],
                        mat.m[3][0], mat.m[3][1], mat.m[3][3]) / d;

    inv.m[2][2] = det3(mat.m[0][0], mat.m[0][1], mat.m[0][3],
                       mat.m[1][0], mat.m[1][1], mat.m[1][3],
                       mat.m[3][0], mat.m[3][1], mat.m[3][3]) / d;

    inv.m[2][3] = -det3(mat.m[0][0], mat.m[0][1], mat.m[0][3],
                        mat.m[1][0], mat.m[1][1], mat.m[1][3],
                        mat.m[2][0], mat.m[2][1], mat.m[2][3]) / d;

    // row 4
    inv.m[3][0] = -det3(mat.m[1][0], mat.m[1][1], mat.m[1][2],
                        mat.m[2][0], mat.m[2][1], mat.m[2][2],
                        mat.m[3][0], mat.m[3][1], mat.m[3][2]) / d;

    inv.m[3][1] = det3(mat.m[0][0], mat.m[0][1], mat.m[0][2],
                       mat.m[2][0], mat.m[2][1], mat.m[2][2],
                       mat.m[3][0], mat.m[3][1], mat.m[3][2]) / d;

    inv.m[3][2] = - det3(mat.m[0][0], mat.m[0][1], mat.m[0][2],
                        mat.m[1][0], mat.m[1][1], mat.m[1][2],
                        mat.m[3][0], mat.m[3][1], mat.m[3][2]) / d;

    inv.m[3][3] = det3(mat.m[0][0], mat.m[0][1], mat.m[0][2],
                       mat.m[1][0], mat.m[1][1], mat.m[1][2],
                       mat.m[2][0], mat.m[2][1], mat.m[2][2]) / d;

    return inv;
}

/* Returns the transpose of mat */
tmat transpose(const tmat &mat)
{
    tmat tm;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            tm.m[i][j] = mat.m[j][i];
        }
    }

    return tm;
}

/* Returns the 4 x 4 identity matrix */
tmat identityMatrix()
{
    tmat I;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            I.m[i][j] = 0;
        }
    }

    I.m[0][0] = 1;
    I.m[1][1] = 1;
    I.m[2][2] = 1;
    I.m[3][3] = 1;

    return I;
}

/* Returns the 4x4 zero matrix */
tmat zeroMatrix()
{
    tmat Z;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            Z.m[i][j] = 0;
        }
    }

    return Z;
}

/* Create a tmat that translates things by x in the x direction, y in the
* y direction, z in the z direction */
tmat translate(float x, float y, float z)
{
    tmat mat = identityMatrix();

    mat.m[0][3] = x;
    mat.m[1][3] = y;
    mat.m[2][3] = z;

    return mat;
}

/* Create a tmat that scales things by x in the x direction, y in the y direction,
* z in the z direction */
tmat scale(float x, float y, float z)
{
    tmat mat = zeroMatrix();

    mat.m[0][0] = x;
    mat.m[1][1] = y;
    mat.m[2][2] = z;
    mat.m[3][3] = 1.0f;

    return mat;
}

/* Create a tmat that rotates things by angle radians in the x direction */
tmat rotateX(float angle)
{
    tmat mat;
    float cosine = cos(angle);
    float sine = sin(angle);

    mat.m[1][1] = cosine;
    mat.m[1][2] = -sine;
    mat.m[2][1] = sine;
    mat.m[2][2] = cosine;

    return mat;
}

/* Create a tmat that rotates things by angle radians in the y direction */
tmat rotateY(float angle)
{
    tmat mat;
    float cosine = cos(angle);
    float sine = sin(angle);

    mat.m[0][0] = cosine;
    mat.m[0][2] = sine;
    mat.m[2][0] = -sine;
    mat.m[2][2] = cosine;

    return mat;
}

/* Create a tmat that rotates things by angle radians in the z direction */
tmat rotateZ(float angle)
{
    tmat mat;
    float cosine = cos(angle);
    float sine = sin(angle);

    mat.m[0][0] = cosine;
    mat.m[0][1] = -sine;
    mat.m[1][0] = sine;
    mat.m[1][1] = cosine;

    return mat;
}

/* Create a tmat that rotates things by angle radians in the direction given by axis */
tmat rotate(const vec3 &axis, float angle)
{
    vec3 naxis = makeUnitVector(axis); // unit vect in direction of axis
    tmat mat;
    float x = naxis.x;
    float y = naxis.y;
    float z = naxis.z;
    float cosine = cos(angle);
    float sine = sin(angle);
    float t = 1.0f - cosine;

    mat.m[0][0] = t * x * x + cosine;
    mat.m[0][1] = t * x * y - sine * y;
    mat.m[0][2] = t * x * z + sine * y;
    mat.m[0][3] = 0.0f;

    mat.m[1][0] = t * x * y + sine * z;
    mat.m[1][1] = t * y * y + cosine;
    mat.m[1][2] = t * y * z - sine * x;
    mat.m[1][3] = 0.0f;

    mat.m[0][0] = t * x * z - sine * y;
    mat.m[0][1] = t * y * z + sine * x;
    mat.m[0][2] = t * z * z + cosine;
    mat.m[0][3] = 0.0f;

    mat.m[0][0] = 0.0f;
    mat.m[0][1] = 0.0f;
    mat.m[0][2] = 0.0f;
    mat.m[0][3] = 1.0f;

    return mat;
}

/* Create a matrix where the eye is at the xyz basis looking down the -z axis */
tmat viewMat(const vec3 &eye, const vec3 &gaze, const vec3 &up)
{
    tmat mat = identityMatrix();

    // orthonormal basis from params
    vec3 w = -makeUnitVector(gaze);
    vec3 u = makeUnitVector(cross(up, w));
    vec3 v = cross(w, u);

    // rotate the orthonormal basis to the xyz basis
    mat.m[0][0] = u.x;
    mat.m[0][1] = u.y;
    mat.m[0][2] = u.z;
    mat.m[1][0] = v.x;
    mat.m[1][1] = v.y;
    mat.m[1][2] = v.z;
    mat.m[2][0] = w.x;
    mat.m[2][1] = w.y;
    mat.m[2][2] = w.z;

    // translate the matrix to the xyz origin
    tmat move = identityMatrix();
    move.m[0][3] = -(eye.x);
    move.m[2][3] = -(eye.y);
    move.m[3][3] = -(eye.z);

    mat *= move;
    return mat;
}

std::ostream &operator <<(std::ostream &out, const tmat &mat)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            out << mat.m[i][j] << " "; 
        } 
        out << "\n";
    }

    return out;
}


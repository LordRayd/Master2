#ifndef __MATH_INTERSECTIONS_H__	// routine qui sert à pas inclure 2 fois le même fichier
#define __MATH_INTERSECTIONS_H__	//

#include "Common.h"

////////////////////////////////////////////////////////////
/// Ray
////////////////////////////////////////////////////////////
class Ray
{
public:
	Ray(const Vec3& origin, const Vec3& Direction)
		:m_origin(origin), m_direction(glm::normalize(Direction)) {}

	inline const Vec3& Origin() const { return m_origin; }
	inline const Vec3& Direction() const { return m_direction; }

private:
	Vec3 m_origin		;
	Vec3 m_direction	;
};

////////////////////////////////////////////////////////////
/// Quadrilateral
////////////////////////////////////////////////////////////
class Quadrilateral
{
public:
	Quadrilateral(){}

	inline Quadrilateral(const Vec3& v_00, const Vec3& v_10, const Vec3& v_11, const Vec3& v_01)
	{
		vertices[0] = v_00; vertices[1] = v_10; vertices[2] = v_11; vertices[3] = v_01;
	}
	inline const Vec3& v_00() const { return vertices[0]; }
	inline const Vec3& v_10() const { return vertices[1]; }
	inline const Vec3& v_11() const { return vertices[2]; }
	inline const Vec3& v_01() const { return vertices[3]; }

private:
	Vec3 vertices[4];
};



////////////////////////////////////////////////////////////
/// Appartenance d'un point a un triangle: 
/// point(u,v) = (1-u-v)*p0 + u*p1 + v*p2
/// ou p0, p1, p2 sont les vertices du triangle
/// avec: u >= 0, v >= 0 et u + v <= 1.0
///
/// équation paramétrique d’une droite
/// point(t) = p + t * d où p est un point de la droite et d est un vecteur directeur de la droite
///
/// Si il existe un point p appartenant à la fois à la droite et au triangle 
/// p + t * d = (1-u-v) * p0 + u * p1 + v * p2
///
////////////////////////////////////////////////////////////
static bool RayTriangleIntersect(	const Vec3& p, const Vec3& d,
							const Vec3& p0, const Vec3& p1, const Vec3& p2,
							Vec3& intersection)
{
	Vec3 e1(p1-p0), e2(p2-p0), h(glm::cross(d, e2)), s(p-p0), q(glm::cross(s, e1));
	
	float a(glm::dot(e1, h));
	if ( a > -0.00001f && a < 0.00001f ) return false ;
	
	float f(1.0f/a), u(f * glm::dot(s,h));
	if ( u < -0.01f || u > 1.1f )			return false ;

	float v(f * glm::dot(d, q));	
	if ( v < -0.01f || u + v > 1.1f )		return false ;


	// at this stage we can compute t to find out where
	// the intersection point is on the line
	float t(f * glm::dot(e2, q));
	
	// ray intersection
	if ( t > 0.00001f )
	{
		intersection = p + t * d;
		return true; 
	}
	
	// no ray intersection
	return false; 
}

static bool RayTriangleIntersect(	const Ray& r,
							const Vec3& p0, const Vec3& p1, const Vec3& p2,
							Vec3& intersection)
{
	return RayTriangleIntersect(r.Origin(), r.Direction(), p0, p1, p2, intersection);
}


// intersect_RayTriangle(): intersect a ray with a 3D triangle
//    Input:  a ray R, and a triangle T
//    Output: I = intersection point (when it exists)
//    Return: -1 = triangle is degenerate (a segment or point)
//             0 = disjoint (no intersect)
//             1 = intersect in unique point I1
//             2 = are in the same plane
/*static int RayTriangleIntersect( const Ray R, const Vec3& p0, const Vec3& p1, const Vec3& p2, Vec3& I )
{
	#define SMALL_NUM  0.00000001 // anything that avoids division overflow

    Vec3    u, v, n;				// triangle vectors
    Vec3    dir, w0, w;				// ray vectors
    float   r, a, b;				// params to calc ray-plane intersect

    // get triangle edge vectors and plane normal
    u = p1 - p0;
    v = p2 - p0;
    n = u * v;						// cross product
    if (n == Vec3(0.0f))			// triangle is degenerate
        return -1;					// do not deal with this case

    dir = R.Direction();			// ray direction vector
    w0 = R.Origin() - p0;
	a = -glm::dot(n,w0);
    b = glm::dot(n,dir);
    if (fabs(b) < SMALL_NUM) {     // ray is parallel to triangle plane
        if (a == 0)                // ray lies in triangle plane
            return 2;
        else return 0;             // ray disjoint from plane
    }

    // get intersect point of ray with triangle plane
    r = a / b;
    if (r < 0.0)                   // ray goes away from triangle
        return 0;                  // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect

    I = R.Origin() + r * dir;           // intersect point of ray and plane

    // is I inside T?
    float    uu, uv, vv, wu, wv, D;
    uu = glm::dot(u,u);
    uv = glm::dot(u,v);
    vv = glm::dot(v,v);
    w = I - p0;
    wu = glm::dot(w,u);
    wv = glm::dot(w,v);
    D = uv * uv - uu * vv;

    // get and test parametric coords
    float s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)        // I is outside T
        return 0;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
        return 0;

    return 1;                      // I is in T
}
*/

////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////
static bool RayQuadIntersect(const Quadrilateral& q, const Ray& r, float& u, float& v, float& t)
{
  static const float eps = float(10e-6);

  // Rejects rays that are parallel to Q, and rays that intersect the plane of
  // Q either on the left of the line V00V01 or on the right of the line V00V10.

  Vec3 E_01(q.v_10() - q.v_00()), E_03(q.v_01() - q.v_00()), P(glm::cross(r.Direction(), E_03));
  float det = glm::dot(E_01, P);
  if (std::abs(det) < eps) return false;

  float inv_det(float(1.0) / det);
  Vec3 T(r.Origin() - q.v_00());
  float alpha(glm::dot(T, P) * inv_det);
  if (alpha < float(0.0)) return false;

  // if (alpha > float(1.0)) return false; // Uncomment if VR is used.
  Vec3 Q(glm::cross(T, E_01));
  float beta(glm::dot(r.Direction(), Q) * inv_det);
  if (beta < float(0.0)) return false; 
  // if (beta > float(1.0)) return false; // Uncomment if VR is used.

  if ((alpha + beta) > float(1.0)) 
  {
    // Rejects rays that intersect the plane of Q either on the
    // left of the line V11V10 or on the right of the line V11V01.

    Vec3 E_23(q.v_01() - q.v_11()), E_21(q.v_10() - q.v_11()), P_prime(glm::cross(r.Direction(), E_21));
    float det_prime(glm::dot(E_23, P_prime));
    if (std::abs(det_prime) < eps) return false;

    float inv_det_prime = float(1.0) / det_prime;
    Vec3 T_prime = r.Origin() - q.v_11();
    float alpha_prime = glm::dot(T_prime, P_prime) * inv_det_prime;
    if (alpha_prime < float(0.0)) return false;

    Vec3 Q_prime = glm::cross(T_prime, E_23);
    float beta_prime = glm::dot(r.Direction(), Q_prime) * inv_det_prime;
    if (beta_prime < float(0.0)) return false;
  }

  // Compute the ray parameter of the intersection point, and
  // reject the ray if it does not hit Q.

  t = glm::dot(E_03, Q) * inv_det;
  if (t < float(0.0)) return false; 

  // Compute the barycentric coordinates of the fourth vertex.
  // These do not depend on the ray, and can be precomputed
  // and stored with the quadrilateral.  

  float alpha_11, beta_11;
  Vec3 E_02 = q.v_11() - q.v_00();
  Vec3 n = glm::cross(E_01, E_03);

  if ((std::abs(n.x) >= std::abs(n.y))
    && (std::abs(n.x) >= std::abs(n.z))) {

    alpha_11 = ((E_02.y * E_03.z) - (E_02.z * E_03.y)) / n.x;
    beta_11  = ((E_01.y * E_02.z) - (E_01.z * E_02.y)) / n.x;
  }
  else if ((std::abs(n.y) >= std::abs(n.x))
    && (std::abs(n.y) >= std::abs(n.z))) {  

    alpha_11 = ((E_02.z * E_03.x) - (E_02.x * E_03.z)) / n.y;
    beta_11  = ((E_01.z * E_02.x) - (E_01.x * E_02.z)) / n.y;
  }
  else {

    alpha_11 = ((E_02.x * E_03.y) - (E_02.y * E_03.x)) / n.z;
    beta_11  = ((E_01.x * E_02.y) - (E_01.y * E_02.x)) / n.z;
  }

  // Compute the bilinear coordinates of the intersection point.

  if (std::abs(alpha_11 - float(1.0)) < eps) {    

    // Q is a trapezium.
    u = alpha;
    if (std::abs(beta_11 - float(1.0)) < eps) v = beta; // Q is a parallelogram.
    else v = beta / ((u * (beta_11 - float(1.0))) + float(1.0)); // Q is a trapezium.
  }
  else if (std::abs(beta_11 - float(1.0)) < eps) {

    // Q is a trapezium.
    v = beta;
    u = alpha / ((v * (alpha_11 - float(1.0))) + float(1.0));
  }
  else {

    float A = float(1.0) - beta_11;
    float B = (alpha * (beta_11 - float(1.0)))
      - (beta * (alpha_11 - float(1.0))) - float(1.0);
    float C = alpha;
    float D = (B * B) - (float(4.0) * A * C);
    float Q = float(-0.5) * (B + ((B < float(0.0) ? float(-1.0) : float(1.0))
      * std::sqrt(D)));
    u = Q / A;
    if ((u < float(0.0)) || (u > float(1.0))) u = C / Q;
    v = beta / ((u * (beta_11 - float(1.0))) + float(1.0)); 
  }

  return true;
}

////////////////////////////////////////////////////////////
// compute proj of point p on plane (n,x0)
////////////////////////////////////////////////////////////
static Vec3 ProjPoint2PLane(const Vec3& n, const Vec3& x0, const Vec3& p)
{
	// first find plane equation
	float	d(-n.x*x0.x -n.y*x0.y -n.z*x0.z),
			x(-d/n.x), y(-d/n.y), z(-d/n.z);
	// next compute projection of point
	float e( (n.x*p.x + n.y*p.y + n.z*p.z + d)  / sqrt(n.x*n.x + n.y*n.y + n.z*n.z));
	return Vec3(p.x - n.x * e, p.y - n.y * e, p.z - n.z * e);	
	//float distance = abs(n.x*p.x + n.y*p.y + n.z*p.z + d) / sqrt(n.x*n.x + n.y*n.y + n.z*n.z));
}




#define _x_ 0
#define _y_ 1
#define _z_ 2

#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0];

#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2];

#define FINDMINMAX(x0,x1,x2,min,max) \
  min = max = x0;   \
  if(x1<min) min=x1;\
  if(x1>max) max=x1;\
  if(x2<min) min=x2;\
  if(x2>max) max=x2;

static int planeBoxOverlap(float normal[3],float d, float maxbox[3])
{
  int q;
  float vmin[3],vmax[3];
  for(q=_x_;q<=_z_;q++)
  {
    if(normal[q]>0.0f)
    {
      vmin[q]=-maxbox[q];
      vmax[q]=maxbox[q];
    }
    else
    {
      vmin[q]=maxbox[q];
      vmax[q]=-maxbox[q];
    }
  }
  if(DOT(normal,vmin)+d>0.0f) return 0;
  if(DOT(normal,vmax)+d>=0.0f) return 1;

  return 0;
}


/*======================== _x_-tests ========================*/
#define AXISTEST_X01(a, b, fa, fb)             \
    p0 = a*v0[_y_] - b*v0[_z_];                    \
    p2 = a*v2[_y_] - b*v2[_z_];                    \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
    rad = fa * boxhalfsize[_y_] + fb * boxhalfsize[_z_];   \
    if(min>rad || max<-rad) return 0;

#define AXISTEST_X2(a, b, fa, fb)              \
    p0 = a*v0[_y_] - b*v0[_z_];                    \
    p1 = a*v1[_y_] - b*v1[_z_];                    \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
    rad = fa * boxhalfsize[_y_] + fb * boxhalfsize[_z_];   \
    if(min>rad || max<-rad) return 0;

/*======================== _y_-tests ========================*/
#define AXISTEST_Y02(a, b, fa, fb)             \
    p0 = -a*v0[_x_] + b*v0[_z_];                   \
    p2 = -a*v2[_x_] + b*v2[_z_];                       \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
    rad = fa * boxhalfsize[_x_] + fb * boxhalfsize[_z_];   \
    if(min>rad || max<-rad) return 0;

#define AXISTEST_Y1(a, b, fa, fb)              \
    p0 = -a*v0[_x_] + b*v0[_z_];                   \
    p1 = -a*v1[_x_] + b*v1[_z_];                       \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
    rad = fa * boxhalfsize[_x_] + fb * boxhalfsize[_z_];   \
    if(min>rad || max<-rad) return 0;

/*======================== _z_-tests ========================*/

#define AXISTEST_Z12(a, b, fa, fb)             \
    p1 = a*v1[_x_] - b*v1[_y_];                    \
    p2 = a*v2[_x_] - b*v2[_y_];                    \
        if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;} \
    rad = fa * boxhalfsize[_x_] + fb * boxhalfsize[_y_];   \
    if(min>rad || max<-rad) return 0;

#define AXISTEST_Z0(a, b, fa, fb)              \
    p0 = a*v0[_x_] - b*v0[_y_];                \
    p1 = a*v1[_x_] - b*v1[_y_];                    \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
    rad = fa * boxhalfsize[_x_] + fb * boxhalfsize[_y_];   \
    if(min>rad || max<-rad) return 0;

static int TriBoxOverlap(float boxcenter[3],float boxhalfsize[3],float triverts[3][3])
{

  /*    use separating axis theorem to test overlap between triangle and box */
  /*    need to test for overlap in these directions: */
  /*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
  /*       we do not even need to test these) */
  /*    2) normal of the triangle */
  /*    3) crossproduct(edge from tri, {x,y,z}-directin) */
  /*       this gives 3x3=9 more tests */
   float v0[3],v1[3],v2[3];
   float min,max,d,p0,p1,p2,rad,fex,fey,fez;
   float normal[3],e0[3],e1[3],e2[3];

   /* This is the fastest branch on Sun */
   /* move everything so that the boxcenter is in (0,0,0) */
   SUB(v0,triverts[0],boxcenter);
   SUB(v1,triverts[1],boxcenter);
   SUB(v2,triverts[2],boxcenter);

   /* compute triangle edges */
   SUB(e0,v1,v0);      /* tri edge 0 */
   SUB(e1,v2,v1);      /* tri edge 1 */
   SUB(e2,v0,v2);      /* tri edge 2 */

   /* Bullet 3:  */
   /*  test the 9 tests first (this was faster) */
   fex = fabs(e0[_x_]);
   fey = fabs(e0[_y_]);
   fez = fabs(e0[_z_]);
   AXISTEST_X01(e0[_z_], e0[_y_], fez, fey);
   AXISTEST_Y02(e0[_z_], e0[_x_], fez, fex);
   AXISTEST_Z12(e0[_y_], e0[_x_], fey, fex);

   fex = fabs(e1[_x_]);
   fey = fabs(e1[_y_]);
   fez = fabs(e1[_z_]);
   AXISTEST_X01(e1[_z_], e1[_y_], fez, fey);
   AXISTEST_Y02(e1[_z_], e1[_x_], fez, fex);
   AXISTEST_Z0(e1[_y_], e1[_x_], fey, fex);

   fex = fabs(e2[_x_]);
   fey = fabs(e2[_y_]);
   fez = fabs(e2[_z_]);
   AXISTEST_X2(e2[_z_], e2[_y_], fez, fey);
   AXISTEST_Y1(e2[_z_], e2[_x_], fez, fex);
   AXISTEST_Z12(e2[_y_], e2[_x_], fey, fex);

   /* Bullet 1: */
   /*  first test overlap in the {x,y,z}-directions */
   /*  find min, max of the triangle each direction, and test for overlap in */
   /*  that direction -- this is equivalent to testing a minimal AABB around */
   /*  the triangle against the AABB */

   /* test in _x_-direction */
   FINDMINMAX(v0[_x_],v1[_x_],v2[_x_],min,max);
   if(min>boxhalfsize[_x_] || max<-boxhalfsize[_x_]) return 0;

   /* test in _y_-direction */
   FINDMINMAX(v0[_y_],v1[_y_],v2[_y_],min,max);
   if(min>boxhalfsize[_y_] || max<-boxhalfsize[_y_]) return 0;

   /* test in _z_-direction */
   FINDMINMAX(v0[_z_],v1[_z_],v2[_z_],min,max);
   if(min>boxhalfsize[_z_] || max<-boxhalfsize[_z_]) return 0;

   /* Bullet 2: */
   /*  test if the box intersects the plane of the triangle */
   /*  compute plane equation of triangle: normal*x+d=0 */
   CROSS(normal,e0,e1);
   d=-DOT(normal,v0);  /* plane eq: normal.x+d=0 */
   if(!planeBoxOverlap(normal,d,boxhalfsize)) return 0;

   return 1;   /* box and triangle overlaps */
}


#endif // __MATH_INTERSECTIONS_H__ fin de la routine
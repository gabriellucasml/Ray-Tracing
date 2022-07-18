#ifndef _SURFEL_
#define _SURFEL_


#include <memory>
#include "vec3.h"
#include "primitive.hpp"
#include "bounds3.hpp"
#include "../../Vector/vec3.h"
#include "bounds3.h"

class Primitive;

class Surfel{
public:
    vec3 p;        //!< Contact point.
    vec3 n;       //!< The surface normal.
    vec3 wo;      //!< Outgoing direction of light, which is -ray.
    float time;     //!< Time of contact.
    point3 uv;     //!< Parametric coordinate (u,v) of the hit surface.
    Primitive *primitive=nullptr; //!< Pointer to the primitive.
    std::shared_ptr<Bounds3> bound;

    Surfel() {}
    Surfel( const vec3&p,const vec3&n, const vec3&wo, float time,
            const point3& uv, Primitive *pri )
            : p{p}, n{n}, wo{wo}, time{time}, uv{uv}, primitive{pri}
    {/* empty */};
};

#endif

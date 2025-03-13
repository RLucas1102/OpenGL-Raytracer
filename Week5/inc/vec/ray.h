/********************************************************************************
 * Name: Lucas Robenolt
 * 
 * Last Updated: 3/12/2025
 * 
 * Email: robenoltl@gmail.com
 *
 * Purpose: The ray class is an extension of the vec3 class. With this class, a 
 * 			ray with an origin and direction can be created to later be used
 * 			within the camera class. Rays will be shot from the camera's origin
 * 			and into the scene in order to tell if objects were hit within the 
 * 			scene.
 *          
 ********************************************************************************/

#ifndef RAY_H
#define RAY_H

#include <vec/vec3.h>

class ray
{
	private:
		// Member variables
		// ----------------
		vec3 _org;
		vec3 _dir;

	public:
		// Constructors
		// ------------
		ray() : _org(0, 0, 0), _dir(0, 0, 0) {}

		ray(const vec3& origin, const vec3& direction) {
			_org = origin;
			_dir = direction;
		}

		// Accessors
		// ---------
		vec3 getOrigin() const { return _org; }

		vec3 getDirection() const { return _dir; }

		// Mutators
		// --------
		void setOrigin(const vec3& origin) { _org = origin; } 

		void setDirection(const vec3& direction) { _dir = direction; }

		// Useful functions
		// ----------------
		vec3 at(float t) const {
			return add(_org, multiply(_dir, t)); // P(t) = A + tb
		}

};

#endif
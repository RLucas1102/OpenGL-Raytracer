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
			_dir = direction.normalize();

		}

		// Accessors
		// ---------
		vec3 getOrigin() const {
			return _org;
		}

		vec3 getDirection() const {
			return _dir;
		}

		// Mutators
		// --------
		void setOrigin(const vec3& origin) {
			_org = origin;
		} 

		void setDirection(const vec3& direction) {
			_dir = direction.normalize();
		}

		// Useful functions
		// ----------------
		vec3 at(float t) {
			return add(_org, multiply(_dir, t)); // P(t) = A + tb
		}

};

#endif
#include "Entity.hpp"
#include "Utils.hpp"

class Projectile : public Entity {
private:
	ProjType type;
	float speed = 4.0f;//frka!

public:
	Projectile();

	Projectile(bool b);

	~Projectile();

	bool moveToTarget(const Coords& t);
	Projectile(Coords start, ProjType p);

	void Update() override;

	ProjType getType() const { return type; }
};




#include "Entity.hpp"

class Ally : public Entity{
private:
	short hp = 100;
public:
	Ally();
	Ally(const short x, const short y);

	Ally(Coords c);

	~Ally();

	void Update();
};
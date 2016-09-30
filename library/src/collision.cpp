#include "collision.hpp"
#include "vector2.hpp"

Collision::Collision(const Vector2 &t, const bool &v, const bool c)
{
	_translation = t;
	_touching = v;
	_contained = c;
}

bool Collision::IsTouching() const
{
	return _touching;
}

bool Collision::IsContained() const
{
	return _contained;
}

const Vector2& Collision::GetTranslation() const
{
	return _translation;
}

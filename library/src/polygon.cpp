#include "projection.hpp"
#include "collision.hpp"
#include "polygon.hpp"
#include "segment.hpp"
#include "circle.hpp"

#include <cmath>
#include <iostream>
#include <limits>
#include <algorithm>

Polygon::Polygon() : Shape(), _side(0)
{
}

const AxesVec& Polygon::GetAxes() const
{
	return _axes;
}

const std::vector<Segment>& Polygon::GetSides() const
{
	return _side;
}

const Vector2 Polygon::NearestVertex(const Vector2 &p) const
{
	Precision_t dist = std::numeric_limits<Precision_t>::infinity();
	Vector2 v;

	for (unsigned i = 0; i < GetPointCount(); i++)
	{
		Precision_t temp = p.GetDistance(GetTransformedPoint(i));

		if (temp < dist)
		{
			dist = temp;
			v = GetTransformedPoint(i);
		}
	}

	return v;
}

void Polygon::ReCalc()
{
	Precision_t x = 0;
	Precision_t y = 0;

	_axes.clear();
	_side.clear();

	for (unsigned i = 0; i < GetPointCount(); i++)
	{
		x += _points[i].x;
		y += _points[i].y;

		const Vector2 p1 = GetPoint(i);
		const Vector2 p2 =  GetPoint(i + 1 == GetPointCount() ? 0 : i + 1);

		const Segment l(p1, p2);

		bool parallel = false;

		for (auto && ln : _side)
		{
			if (l.IsParallel(ln))
			{
				parallel = true;
				break;
			}
		}

		_side.push_back(l);

		if (!parallel)
		{
			const Vector2 edge = p1 - p2;
			const Axis normal = edge.Perpendicular().Normal();
			_axes.push_back(normal);
		}
	}

	// FIXME: HAX FiX this //
	if (_center == Vector2(0, 0))
		_center = Vector2(x / GetPointCount(), y / GetPointCount());
}

const Projection Polygon::Project(const Axis &a) const
{
	Precision_t min = a.Dot(GetTransformedPoint(0));
	Precision_t max = min;

	for (unsigned i = 1; i < GetPointCount(); i++)
	{
		Precision_t prj = a.Dot(GetTransformedPoint(i));

		if (prj < min)
			min = prj;

		else if (prj > max)
			max = prj;
	}

	return Projection(min, max);
}

const bool Polygon::TriangleContains(const Vector2 &p, const Vector2 &a, const Vector2 &b, const Vector2 &c) const
{
	// Compute vectors
	Vector2 v0 = c - a;
	Vector2 v1 = b - a;
	Vector2 v2 = p - a;

	// Compute dot products
	Precision_t dot00 = v0.Dot(v0);
	Precision_t dot01 = v0.Dot(v1);
	Precision_t dot02 = v0.Dot(v2);
	Precision_t dot11 = v1.Dot(v1);
	Precision_t dot12 = v1.Dot(v2);

	// Compute barycentric coordinates
	Precision_t invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	Precision_t u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	Precision_t v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// Check if point is in triangle
	return (u >= 0) && (v >= 0) && (u + v <= 1);
}


const bool Polygon::Contains(const Vector2 &v) const
{
	if (GetPointCount() == 3)
		return TriangleContains(v, GetPoint(0) + GetPos(), GetPoint(1) + GetPos(), GetPoint(2) + GetPos());

	else
	{
		for (unsigned i = 0; i < GetPointCount(); i++)
		{
			int c;

			if (i != GetPointCount() - 1)
				c = i + 1;

			else
				c = 0;

			if (TriangleContains(v, GetPoint(i) + GetPos(), GetCenter() + GetPos(), GetPoint(c) + GetPos()))
				return true;
		}
	}

	return false;
}

const bool Polygon::Contains(const Segment &s) const
{
	return (Contains(s.GetTransformedPoint(0)) && Contains(s.GetTransformedPoint(1)));
}

const bool Polygon::Contains(const Circle &c) const
{
	const Vector2 center = c.GetPos();

	if (!Contains(center))
		return false;

	for(auto && s : GetSides())
	{
		const Segment l(s.GetPoint(0) + GetPos(), s.GetPoint(1) + GetPos());
		const Precision_t dist = l.DistancePoint(center);

		if (dist <= c.GetRadius())
			return false;
	}

	return true;
}

const bool Polygon::Contains(const Polygon &p) const
{
	for (unsigned v = 0; v < p.GetPointCount(); v++)
	{
		if (!Contains(p.GetPoint(v) + p.GetPos()))
			return false;
	}

	return true;
}

const bool Polygon::Intersects(const Segment &s) const
{
	AxesVec axes(GetAxes());
	axes.push_back(s.GetAxis());

	for (auto && axis : axes)
	{
		const Projection pA = s.Project(axis);
		const Projection pB = Project(axis);

		if (!pA.IsOverlap(pB))
			return false;
	}

	return true;
}

const bool Polygon::Intersects(const Circle &c) const
{
	const Axis ax = (NearestVertex(c.GetPos()) - c.GetPos()).Normal();

	AxesVec axes(GetAxes());
	axes.push_back(ax);

	for (auto && axis : axes)
	{
		const Projection pA = c.Project(axis);
		const Projection pB = Project(axis);

		if (!pA.IsOverlap(pB))
			return false;
	}

	return true;
}

const bool Polygon::Intersects(const Polygon &p) const
{
	const AxesVec A = GetAxes();
	const AxesVec B = p.GetAxes();

	AxesVec axes;

	axes.reserve(A.size() + B.size());
	axes.insert(axes.end(), A.begin(), A.end());
	axes.insert(axes.end(), B.begin(), B.end());

	for (auto && axis : axes)
	{
		const Projection pA = p.Project(axis);
		const Projection pB = Project(axis);

		if (!pA.IsOverlap(pB))
			return false;
	}

	return true;
}

const std::vector<Vector2> Polygon::GetIntersections(const Segment &s) const
{
	std::vector<Vector2> intersections(0);

	if (!Contains(s))
	{
		for (auto && side : GetSides())
		{
			// Absolute position of the side
			const Segment sideT(side.GetPoint(0) + GetPos(), side.GetPoint(1) + GetPos());

			auto vec = s.GetIntersections(sideT);

			if (vec.size() > 0)
			{
				const Vector2 i = vec[0];

				if (s.Contains(i) && sideT.Contains(i))
					intersections.push_back(i);
			}
		}
	}

	return intersections;
}

const std::vector<Vector2> Polygon::GetIntersections(const Circle &c) const
{
	std::vector<Vector2> intersections(0);

	if (!Contains(c) && !c.Contains(*this))
	{
		std::vector<Segment> sides;

		for (auto && side : GetSides())
		{
			const Segment seg(side.GetPoint(0) + GetPos(), side.GetPoint(1) + GetPos());

			if (c.Intersects(seg) && !c.Contains(seg))
				sides.push_back(seg);
		}

		for (auto && side : sides)
		{
			const std::vector<Vector2> intercepts = c.GetIntersections(side);

			for (auto && p : intercepts)
				intersections.push_back(p);
		}

	}

	return intersections;
}

const std::vector<Vector2> Polygon::GetIntersections(const Polygon &p) const
{
	std::vector<Vector2> intersections(0);

	for (auto && side : p.GetSides())
	{
		const Segment sideT(side.GetPoint(0) + p.GetPos(), side.GetPoint(1) + p.GetPos());

		for (auto && i : GetIntersections(sideT))
		{
			intersections.push_back(i);
		}
	}

	return intersections;
}

const Vector2 Polygon::GetTranslation(const Segment &s) const
{
	Precision_t Overlap = std::numeric_limits<Precision_t>::infinity();
	Axis smallest;
	Vector2 translation;

	AxesVec axes(GetAxes());
	axes.push_back(s.GetAxis());

	for (auto && axis : axes)
	{
		const Projection pA = s.Project(axis);
		const Projection pB = Project(axis);

		if (pA.IsOverlap(pB))
		{
			const Precision_t o = pA.GetOverlap(pB);

			if (o < Overlap)
			{
				Overlap = o;
				smallest = axis;
			}
		}
	}

	translation = smallest * (Overlap + 1);


	/** FIXME: This doesn't work
	 *
	Vector2 distance = (s.GetCenter() + s.GetPos()) - (GetCenter() + GetPos());

	if (translation.Dot(distance) < 0)
		translation = -translation;
	*
	*/

	// This does work but is hacks
	Segment test(s.GetTransformedPoint(0), s.GetTransformedPoint(1));
	test.Move(translation);

	if (Intersects(test))
		translation = -translation;

	return translation;
}

const Vector2 Polygon::GetTranslation(const Circle &c) const
{
	Precision_t Overlap = std::numeric_limits<Precision_t>::infinity();
	Axis smallest;
	Vector2 translation;

	AxesVec axes(GetAxes());
	axes.push_back((NearestVertex(c.GetPos()) - c.GetPos()).Normal());

	for (auto && axis : axes)
	{
		const Projection pA = c.Project(axis);
		const Projection pB = Project(axis);

		if (pA.IsOverlap(pB))
		{
			const Precision_t o = pA.GetOverlap(pB);

			if (o < Overlap)
			{
				Overlap = o;
				smallest = axis;
			}
		}
	}

	translation = smallest * (Overlap + 1);

	Vector2 distance = (c.GetPos()) - (GetCenter() + GetPos());

	if ((translation.Dot(distance) < 0) || Contains(c))
		translation = -translation;

	return translation;
}

const Vector2 Polygon::GetTranslation(const Polygon &p) const
{
	Precision_t Overlap = std::numeric_limits<Precision_t>::infinity();

	Axis smallest;
	Vector2 translation;
	AxesVec axes;

	const AxesVec A = GetAxes();
	const AxesVec B = p.GetAxes();

	axes.reserve(A.size() + B.size());
	axes.insert(axes.end(), A.begin(), A.end());
	axes.insert(axes.end(), B.begin(), B.end());

	for (auto && axis : axes)
	{
		const Projection pA = p.Project(axis);
		const Projection pB = Project(axis);

		if (pA.IsOverlap(pB))
		{
			const Precision_t o = pA.GetOverlap(pB);

			if (o < Overlap)
			{
				Overlap = o;
				smallest = axis;
			}
		}
	}

	translation = smallest * (Overlap + 1);

	Vector2 distance = (p.GetCenter() + p.GetPos()) - (GetCenter() + GetPos());

	if ((translation.Dot(distance) < 0) || Contains(p))
		translation = -translation;

	return translation;
}

const Collision Polygon::GetCollision(const Segment &s) const
{
	bool doesIntersect = false;

	// Determine if this polygon contains
	// the segment "s"
	bool contains = false;//Contains(s);

	// The segment "s" cannot
	// contain this polygon
	bool contained = false;

	// Intersection points
	std::vector<Vector2> intersects(0);

	// Translation is the vector to be applied to segment "s"
	// in order to seperate it from the circle
	Vector2 translation(0, 0);
	Precision_t Overlap = std::numeric_limits<Precision_t>::infinity();
	Axis smallest;

	AxesVec axes(GetAxes());
	axes.push_back(s.GetAxis());

	for (auto && axis : axes)
	{
		const Projection pA = s.Project(axis);
		const Projection pB = Project(axis);

		// No Collision
		if (!pA.IsOverlap(pB))
			return Collision(doesIntersect, intersects, contains, contained, translation);

		else
		{
			const Precision_t o = pA.GetOverlap(pB);

			if (o < Overlap)
			{
				Overlap = o;
				smallest = axis;
			}
		}
	}

	translation = smallest * (Overlap + 1);

	/** FIXME: This doesn't work
	 *
	Vector2 distance = (s.GetCenter() + s.GetPos()) - (GetCenter() + GetPos());

	if (translation.Dot(distance) < 0)
		translation = -translation;
	*
	*/

	// This does work but is hacks
	Segment test(s.GetTransformedPoint(0), s.GetTransformedPoint(1));
	test.Move(translation);

	if (Intersects(test))
		translation = -translation;

	doesIntersect = true;
	contains = Contains(s);

	return Collision(doesIntersect, intersects, contains, contained, translation);
}

const Collision Polygon::GetCollision(const Circle &c) const
{
	bool doesIntersect = false;

	// Determine if this polygon contains
	// the circle "c"
	bool contains = false;

	// Determine if the circle "c"
	// contains this polygon
	bool contained = false;

	// Intersection points
	std::vector<Vector2> intersects(0);

	// Translation is the vector to be applied to segment "s"
	// in order to seperate it from the circle
	Vector2 translation(0, 0);
	Precision_t Overlap = std::numeric_limits<Precision_t>::infinity();
	Axis smallest;

	const Axis ax = (NearestVertex(c.GetPos()) - c.GetPos()).Normal();

	AxesVec axes = GetAxes();
	axes.push_back(ax);

	for (auto && axis : axes)
	{
		const Projection pA = c.Project(axis);
		const Projection pB = Project(axis);

		// No Collision
		if (!pA.IsOverlap(pB))
			return Collision(doesIntersect, intersects, contains, contained, translation);

		else
		{
			const Precision_t o = pA.GetOverlap(pB);

			if (o < Overlap)
			{
				Overlap = o;
				smallest = axis;
			}
		}
	}

	doesIntersect = true;
	contains = Contains(c);
	contained = c.Contains(*this);

	translation = smallest * (Overlap + 1);

	Vector2 distance = (GetCenter() + GetPos()) - c.GetPos();

	if (translation.Dot(distance) < 0)
		translation = -translation;

	return Collision(doesIntersect, intersects, contains, contained, translation);
}

const Collision Polygon::GetCollision(const Polygon &p) const
{
	bool doesIntersect = false;

	// Determine if this polygon contains
	// the circle "c"
	bool contains = false;

	// Determine if the polygon "p"
	// contains this polygon
	bool contained = false;

	// Intersection points
	std::vector<Vector2> intersects(0);

	// Translation is the vector to be applied to segment "s"
	// in order to seperate it from the circle
	Vector2 translation(0, 0);
	Precision_t Overlap = std::numeric_limits<Precision_t>::infinity();
	Axis smallest;

	const AxesVec A = GetAxes();
	const AxesVec B = p.GetAxes();

	AxesVec axes;
	axes.reserve(A.size() + B.size());
	axes.insert(axes.end(), A.begin(), A.end());
	axes.insert(axes.end(), B.begin(), B.end());

	for (auto && axis : axes)
	{
		const Projection pA = p.Project(axis);
		const Projection pB = Project(axis);

		// No Collision
		if (!pA.IsOverlap(pB))
			return Collision(doesIntersect, intersects, contains, contained, translation);

		else
		{
			const Precision_t o = pA.GetOverlap(pB);

			if (o < Overlap)
			{
				Overlap = o;
				smallest = axis;
			}
		}
	}

	doesIntersect = true;
	contains = Contains(p);
	contained = p.Contains(*this);

	translation = smallest * (Overlap + 1);

	Vector2 distance = (p.GetCenter() + p.GetPos()) - (GetCenter() + GetPos());

	if ((translation.Dot(distance) < 0) || contains)
		translation = -translation;

	return Collision(doesIntersect, intersects, contains, contained, translation);
}

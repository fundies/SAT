#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "vector2.hpp"

class Projection;
class Collision;
class Circle;
class Polygon;
class Segment;

//!  A class representing an abstract geometric shape. */
class Shape
{
public:
	//! Gets the center of this shape.
	/*!
		\return The center of this shape.
	*/
	virtual const Vector2& GetCenter() const = 0;

	//! Sets the number of points in this shape.
	/*!
		\param c The number of points in this shape.
	*/
	virtual void SetPointCount(const unsigned &c) = 0;

	//! Sets the point of this shape at the given index to the new point.
	/*!
		\param i The index of the point.
		\param p The new point to replace the old point with.
	*/
	virtual void SetPoint(const unsigned &i, const Vector2 &p) = 0;

	//! Gets the number of points in this shape.
	/*!
		\return The number of points in this shape.
	*/
	virtual const unsigned GetPointCount() const = 0;

	//! Gets the point of this shape at the given index.
	/*!
		\param i The index of the point.
		\return The point of this shape at the given index.
	*/
	virtual const Vector2& GetPoint(const unsigned &i) const = 0;

	//! Projects the circle onto the given axis and returns the projection.
	/*!
		\param a The axis to project the circle onto.
		\return The projection of the circle onto the axis.
	*/
	virtual const Projection Project(const Axis &a) const = 0;

	//! Checks if this shape contains the given vector and returns the result.
	/*!
		\param v The vector to check for containment in this shape.
		\return Whether this shape contains the given vector.
	*/
	virtual const bool Contains(const Vector2 &v) const = 0;

	//! Checks if this shape contains the given segment and returns the result.
	/*!
		This function will not check if the given circle contains this segment, GetCollision() can be used for that.
		\param s The segment to check for containment in this shape.
		\return Whether this shape contains the given segment.
		\sa GetCollision()
	*/
	virtual const bool Contains(const Segment &s) const = 0;

	//! Checks if this shape contains the given circle and returns the result.
	/*!
		This function will not check if the given circle contains this shape, GetCollision() can be used for that.
		\param c The circle to check for containment in this shape.
		\return Whether this shape contains the given circle.
		\sa GetCollision()
	*/
	virtual const bool Contains(const Circle &c) const = 0;

	//! Checks if this shape contains the given polygon and returns the result.
	/*!
		This function will not check if the given polygon contains this shape, GetCollision() can be used for that.
		\param p The polygon to check for containment in this shape.
		\return Whether this shape contains the given polygon.
		\sa GetCollision()
	*/
	virtual const bool Contains(const Polygon &p) const = 0;

	//! Checks if this shape intersects the given segment and returns the result.
	/*!
		\param s The segment to check for intersection with this shape.
		\return Whether this shape intersects the given segment.
		\sa GetCollision()
	*/
	virtual const bool Intersects(const Segment &s) const = 0;

	//! Checks if this shape intersects the given circle and returns the result.
	/*!
		\param c The circle to check for intersection with this shape.
		\return Whether this shape intersects the given circle.
		\sa GetCollision()
	*/
	virtual const bool Intersects(const Circle &c) const = 0;

	//! Checks if this shape intersects the given polygon and returns the result.
	/*!
		\param p The polygon to check for intersection with this shape.
		\return Whether this shape intersects the given polygon.
		\sa GetCollision()
	*/
	virtual const bool Intersects(const Polygon &p) const = 0;

	//! Gets the intersection points of this shape and the given segment.
	/*!
		\param s A segment intersecting this shape.
		\return list of intersections between this shape and the given segment.
		\sa GetCollision()
	*/
	virtual const std::vector<Vector2> GetIntersections(const Segment &s) const = 0;

	//! Gets the intersection points of this shape and the given circle.
	/*!
		\param c A circle intersecting this shape.
		\return list of intersections between this shape and the given circle.
		\sa GetCollision()
	*/
	virtual const std::vector<Vector2> GetIntersections(const Circle &c) const = 0;

	//! Gets the intersection points of this shape and the given polygon.
	/*!
		\param p A polygon intersecting this shape.
		\return list of intersections between this shape and the given polygon.
		\sa GetCollision()
	*/
	virtual const std::vector<Vector2> GetIntersections(const Polygon &p) const = 0;

	//! Gets the minimum vector to be applied to the given segment's position
	//! in order to seperate it from this shape.
	/*!
		\param c A segment intersecting this shape.
		\return the minimum translation vector.
		\sa GetCollision()
	*/
	virtual const Vector2 GetTranslation(const Segment &s) const = 0;

	//! Gets the minimum vector to be applied to the given circle's position
	//! in order to seperate it from this shape.
	/*!
		\param c A circle intersecting this shape.
		\return the minimum translation vector.
		\sa GetCollision()
	*/
	virtual const Vector2 GetTranslation(const Circle &c) const = 0;

	//! Gets the minimum vector to be applied to the given polygon's position
	//! in order to seperate it from this shape.
	/*!
		\param p A polygon intersecting this shape.
		\return the minimum translation vector.
		\sa GetCollision()
	*/
	virtual const Vector2 GetTranslation(const Polygon &p) const = 0;

	//! Gets the collision of this shape with the given shape and returns the result.
	/*!
		\param s The shape to check for collision with this shape.
		\return The collision result including the minimum translation vector.
		\sa Contains()
	*/
	virtual const Collision GetCollision(const Shape &s) const = 0;

	//! Gets the collision of this shape with the given segment and returns the result.
	/*!
		\param s The segment to check for collision with this shape.
		\return The collision result including the minimum translation vector.
		\sa Contains()
	*/
	virtual const Collision GetCollision(const Segment &s) const = 0;

	//! Gets the collision of this shape with the given circle and returns the result.
	//! Unlike the Contains() function, this function will also check if the given circle contains this shape.
	/*!
		\param c The circle to check for collision with this shape.
		\return The collision result including the minimum translation vector.
		\sa Contains()
	*/
	virtual const Collision GetCollision(const Circle &c) const = 0;

	//! Gets the collision of this shape with the given polygon and returns the result.
	/*!
		Unlike the Contains() function, this function will also check if the given polygon contains this shape.
		\param p The polygon to check for collision with this shape.
		\return The collision result including the minimum translation vector.
		\sa Contains()
	*/
	virtual const Collision GetCollision(const Polygon &p) const = 0;

	//FIXME: Document / Remove these
	virtual void ReCalc() = 0;
	virtual void Move(const Vector2 &o) = 0;
	virtual const Vector2& GetPos() const = 0;
	//virtual const Vector2& GetCenter() const = 0;
	//void SetPos(const Vector2& p) = 0;

};

#endif

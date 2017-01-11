#ifndef SHAPE_IMPL_HPP
#define SHAPE_IMPL_HPP

#include "shape.hpp"

class ShapeImpl : virtual public Shape
{
public:
	//! Constructs a default shape.
	/*!
		This shape's points must be added manually.
	*/
	ShapeImpl();

	//! Constructs a default shape with center coordinate.
	/*!
		\param c The center point of the shape.
		This shape's points must be added manually.
	*/
	ShapeImpl(const Vector2 c);

	//! Gets the center of this shape.
	/*!
		\return The center of this shape.
	*/
	virtual const Vector2& GetCenter() const;

	//! Sets the number of points in this shape.
	/*!
		\param c The number of points in this shape.
	*/
	virtual void SetPointCount(const unsigned &c);

	//! Sets the point of this shape at the given index to the new point.
	/*!
		\param i The index of the point.
		\param p The new point to replace the old point with.
	*/
	virtual void SetPoint(const unsigned &i, const Vector2 &p);

	//! Gets the number of points in this shape.
	/*!
		\return The number of points in this shape.
	*/
	virtual const unsigned GetPointCount() const;

	//! Gets the point of this shape at the given index.
	/*!
		\param i The index of the point.
		\return The point of this shape at the given index.
	*/
	virtual const Vector2& GetPoint(const unsigned &i) const;

	//! Gets the points this shape is composed of.
	/*!
		\return The points this shape is composed of.
	*/
	virtual const std::vector<Vector2>& GetPoints() const;

	//! Method used to caculate displacment of two shapes
	/*!
		\param axes Axes used in calculations.
		\param a Shape a;
		\param b Shape b;
	*/
	virtual const Vector2 CalcDisplacement(const AxesVec &axes, const Shape &a, const Shape &b) const;

	//! Method required to be called after updating the geometry of a shape.
	/*!
	*/
	virtual void ReCalc() override;

protected:

	std::vector<Vector2> _points; /*!< The points this shape is composed of. */
	Precision_t _rotation; /*!< The rotation of this shape. */
	Vector2 _pos; /*!< The position of this shape. */
	Vector2 _center; /*!< The center of this shape. */
};

#endif

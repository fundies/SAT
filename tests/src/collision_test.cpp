#include "collision.hpp"

#include <gtest/gtest.h>

TEST(Collision, DefaultConstructor)
{
	Collision c;

	EXPECT_FALSE(c.Overlaps());
	EXPECT_FALSE(c.AcontainsB());
	EXPECT_FALSE(c.BcontainsA());
	EXPECT_EQ(0, c.GetIntersects().size());
	EXPECT_FLOAT_EQ(0, c.GetDisplacement().x);
	EXPECT_FLOAT_EQ(0, c.GetDisplacement().y);
}

TEST(Collision, FullConstructor)
{
	Collision c(true, std::vector<Vector2>(1), true, true, Vector2(-5, -7));

	EXPECT_TRUE(c.Overlaps());
	EXPECT_TRUE(c.AcontainsB());
	EXPECT_TRUE(c.BcontainsA());
	EXPECT_EQ(1, c.GetIntersects().size());
	EXPECT_FLOAT_EQ(-5, c.GetDisplacement().x);
	EXPECT_FLOAT_EQ(-7, c.GetDisplacement().y);
}

TEST(Collision, Negate)
{
	std::vector<Vector2> pt(2);
	pt[0] = Vector2(5, 5);
	pt[1] = Vector2(-10, -10);

	Collision c(true, pt, false, true, Vector2(-5, -7));

	c = -c;

	auto i = c.GetIntersects();

	EXPECT_TRUE(c.Overlaps());
	EXPECT_TRUE(c.AcontainsB());
	EXPECT_FALSE(c.BcontainsA());
	EXPECT_EQ(2, c.GetIntersects().size());
	EXPECT_FLOAT_EQ(-10, i[0].x);
	EXPECT_FLOAT_EQ(-10, i[0].y);
	EXPECT_FLOAT_EQ(5, i[1].x);
	EXPECT_FLOAT_EQ(5, i[1].y);
	EXPECT_FLOAT_EQ(5, c.GetDisplacement().x);
	EXPECT_FLOAT_EQ(7, c.GetDisplacement().y);
}

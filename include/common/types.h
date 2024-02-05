#ifndef TYPES_H_
#define TYPES_H_

#include <vector>

namespace sierpinski {

static constexpr int kTriangleVertices = 3; /**< Triangle vertex count. */

struct Point2D;

using LineSegment = std::vector<Point2D>; /**< Points on a line segment. */

/**
 * \brief A point in 2D space.
 */
struct Point2D {
  int x = 0; /**< x coordinate. */
  int y = 0; /**< y coordinate. */

  auto operator<=>(const Point2D&) const = default;
};

/**
 * \brief A triangle as seen on a 2D plane.
 */
struct Triangle {
  Point2D vertices[kTriangleVertices]; /**< Triangle vertice coordinates. */
};

}  // namespace sierpinski

#endif

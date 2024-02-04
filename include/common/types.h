#ifndef TYPES_H_
#define TYPES_H_

namespace sierpinski {

static constexpr int kTriangleVertices = 3; /**< Triangle vertex count. */

/**
 * \brief A point in 2D space.
 */
struct Point2D {
  int x = 0; /**< x coordinate. */
  int y = 0; /**< y coordinate. */
};

/**
 * \brief A triangle as seen on a 2D plane.
 */
struct Triangle {
  Point2D vertices[kTriangleVertices]; /**< Triangle vertice coordinates. */
};

}  // namespace sierpinski

#endif

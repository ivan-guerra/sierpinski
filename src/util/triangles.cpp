#include "util/triangles.h"

#include "common/types.h"

namespace sierpinski {
namespace util {

Triangle CreateCenteredTriangle(const Point2D& center, int dist) noexcept {
  Triangle centered_triangle;
  /* top vertex */
  centered_triangle.vertices[0] = {.x = center.x, .y = center.y - dist};
  /* bottom left vertex */
  centered_triangle.vertices[1] = {.x = center.x - dist * 2,
                                   .y = center.y + dist};
  /*bottom right vertex */
  centered_triangle.vertices[2] = {.x = center.x + dist * 2,
                                   .y = center.y + dist};

  return centered_triangle;
}

LineSegment CreateLineSegment(const Point2D& start,
                              const Point2D& end) noexcept {
  LineSegment line_segment = {start, end};
  Point2D curr = start;
  int i = 0;
  while (curr != end) {
    int cnt = 0; /* Number of coordinate updates done this iteration. */

    if (curr.x > end.x) {
      curr.x--;
      cnt++;
    } else if (curr.x < end.x) {
      curr.x++;
      cnt++;
    }
    if (curr.y > end.y) {
      curr.y--;
      cnt++;
    } else if (curr.y < end.y) {
      curr.y++;
      cnt++;
    }

    /* The logic that follows guarantees that the points are equally spaced on
     * screen. Without this logic, the points on y = x or x = y lines would be
     * compacted whereas points on lines of the form y = mx + b would rendered
     * as if there were spaces between them. */
    if (1 == cnt) { /* Only one coordinate update so we share an axis. */
      if (i & 1) {  /* Save only every other point on this line. */
        line_segment.push_back(curr);
      }
    } else if (2 ==
               cnt) { /* Two coordinate updates means we're on a sloped line. */
      line_segment.push_back(curr);
    }
    i++;
  }
  return line_segment;
}

}  // namespace util
}  // namespace sierpinski

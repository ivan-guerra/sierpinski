#ifndef TRIANGLES_H_
#define TRIANGLES_H_

#include "common/types.h"

namespace sierpinski {
namespace util {

/**
 * \brief Create a Triangle centered about the point \p center.
 * \param [in] center The point about which the returned Triangle will be
 *                    centered.
 * \param [in] dist   The distance of each triangle vertex from the
 *                    center point.
 * \returns A Triangle with vertices \p dist distance away from the central
 *          point \p center.
 */
[[nodiscard]] Triangle CreateCenteredTriangle(const Point2D& center,
                                              int dist) noexcept;

/**
 * \brief Create a LineSegment from \p start to \p end.
 * \param [in] start Start point of the line segment.
 * \param [in] end   End point of the line segment.
 * \returns A LineSegment containing points between \p start and \p end.
 *          When rendered on screen, the points will all appear equidistant.
 *          However, the points in the returned LineSegment are not necessarily
 *          equally spaced. This is due to differences in rendering chars
 *          vertically/horizontally versus diagonally.
 */
[[nodiscard]] LineSegment CreateLineSegment(const Point2D& start,
                                            const Point2D& end) noexcept;

}  // namespace util
}  // namespace sierpinski

#endif

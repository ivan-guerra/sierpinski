#ifndef SCREEN_H_
#define SCREEN_H_

#include <optional>
#include <string>

#include "common/types.h"

namespace sierpinski {
namespace graphics {

/**
 * \brief Supported char colors.
 */
enum Color {
  kRed = 1,
  kGreen,
  kBlue,
  kYellow,
  kMagenta,
  kCyan,
  kWhite,
};

/**
 * \brief Screen Dimensions
 */
struct ScreenDimension {
  int width = 0;  /**< Screen width */
  int height = 0; /**< Screen height */
};

/**
 * \brief Initialize the ncurses screen.
 * \returns The dimensions of the screen (i.e., terminal window).
 */
[[nodiscard]] std::optional<ScreenDimension> InitScreen() noexcept;

/**
 * \brief Cleanup ncurses window(s).
 */
void TerminateScreen() noexcept;

/**
 * \brief Render \p symbol at position \p pos on the screen.
 * \param [in] pos    Position at which the character will be rendered on
 *                    screen.
 * \param [in] symbol The character that will be rendered.
 * \param [in] color  The color of the printed character.
 */
void DrawChar(const sierpinski::common::Point2D& pos, char symbol,
              Color color = Color::kWhite) noexcept;

/**
 * \brief Draw the string \p str at position \p pos on the screen.
 * \param [in] str The string to be printed.
 * \param [in] pos Position at which the string will be printed.
 */
void DrawStr(const std::string& str,
             const sierpinski::common::Point2D& pos) noexcept;

/**
 * \brief Return \c true if the user pressed any key.
 */
[[nodiscard]] bool UserPressedKey() noexcept;

}  // namespace graphics
}  // namespace sierpinski

#endif

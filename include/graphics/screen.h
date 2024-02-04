#ifndef SCREEN_H_
#define SCREEN_H_

#include <optional>
#include <vector>

#include "common/types.h"

namespace sierpinski {
namespace graphics {

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
 * \brief Clear the screen.
 */
void Clear() noexcept;

/**
 * \brief Set the ncurses input delay.
 * \details Setting the delay too high can cause the application to appear laggy
 *          whereas setting the delay too low can waste CPU cycles.
 * \param [in] delay_ms Input delay in milliseconds.
 */
void EnableInputDelay(int delay_ms) noexcept;

/**
 * \brief Clear input delay settings previously set by EnableInputDelay().
 */
void DisableInputDelay() noexcept;

/**
 * \brief Render all triangles in \p triangles on screen.
 * \param [in] triangles A set of Sierpinski triangles to be rendered on screen.
 */
void DrawObject(const std::vector<Triangle>& triangles) noexcept;

/**
 * \brief Print a help message on screen.
 * \param [in] screen_dim Screen (i.e., terminal) dimensions.
 */
void DrawInstructions(const ScreenDimension& screen_dim) noexcept;

/**
 * \brief Return \c true if the user pressed 'q' to quit the app.
 */
[[nodiscard]] std::optional<bool> UserRequestedToQuit() noexcept;

}  // namespace graphics
}  // namespace sierpinski

#endif

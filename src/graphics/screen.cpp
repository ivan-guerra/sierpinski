#include "graphics/screen.h"

#include <curses.h>
#include <ncurses.h>

#include <optional>

namespace sierpinski {
namespace graphics {

std::optional<ScreenDimension> InitScreen() noexcept {
  if (!::initscr()) {
    return std::nullopt;
  }
  /*
   * In the conditional below, we try to set the following ncurses settings:
   *   (1) Disable line buffering.
   *   (2) Enable the keypad so we can exit via keypress.
   *   (3) Disable character echoing.
   *   (4) Hide the cursor.
   */
  if (ERR == ::cbreak() || ERR == ::keypad(stdscr, TRUE) || ERR == ::noecho() ||
      ERR == ::curs_set(0)) {
    return std::nullopt;
  }

  /* Fetch the screen dimensions. */
  ScreenDimension screen_dim = {.width = 0, .height = 0};

  /* Funny, getmaxyx() macro has an undefined return value. */
  getmaxyx(stdscr, screen_dim.height, screen_dim.width);

  return screen_dim;
}

void TerminateScreen() noexcept { ::endwin(); }

void Clear() noexcept { ::clear(); }

void EnableInputDelay(int delay_ms) noexcept { ::timeout(delay_ms); }

void DisableInputDelay() noexcept { ::timeout(-1); }

void DrawObject(const std::vector<Triangle>& triangles) noexcept {
  for (const Triangle& triangle : triangles) {
    for (const Point2D& vertex : triangle.vertices) {
      mvaddch(vertex.y, vertex.x, '*');
    }
  }
  ::refresh();
}

void DrawInstructions(const ScreenDimension& screen_dim) noexcept {
  ::mvprintw(screen_dim.height - 1, 0, "%s", "press q to quit");
  ::refresh();
}

std::optional<bool> UserRequestedToQuit() noexcept {
  int c = ::getch();
  return (ERR == c) ? std::nullopt : std::optional<bool>(c == 'q');
}

}  // namespace graphics
}  // namespace sierpinski

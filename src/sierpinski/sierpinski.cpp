#include <cstdlib>
#include <iostream>
#include <random>
#include <string>

#include "common/types.h"
#include "graphics/screen.h"

static void PrintErrAndExit(const std::string& err_msg) noexcept {
  std::cerr << "error: " << err_msg;
  std::exit(EXIT_FAILURE);
}

[[nodiscard]] static int GetRandomInt(int min, int max) noexcept {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(min, max);

  return distr(gen);
}

[[nodiscard]] static sierpinski::graphics::Color GetRandColor() noexcept {
  static const std::vector<sierpinski::graphics::Color> kColors = {
      sierpinski::graphics::Color::kRed,
      sierpinski::graphics::Color::kGreen,
      sierpinski::graphics::Color::kBlue,
      sierpinski::graphics::Color::kYellow,
      sierpinski::graphics::Color::kMagenta,
      sierpinski::graphics::Color::kCyan,
      sierpinski::graphics::Color::kWhite,
  };
  return kColors[GetRandomInt(0, kColors.size() - 1)];
}

/* DrawSierpinskiTriangles() implements the Chaos Game approach to
 * generating the triangles:
 * https://en.wikipedia.org/wiki/Sierpi%C5%84ski_triangle#Chaos_game */
static void DrawSierpinskiTriangles(
    const sierpinski::graphics::ScreenDimension& screen_dim) noexcept {
  sierpinski::common::Triangle base;
  base.vertices[0] = {.x = 0, .y = 0};
  base.vertices[1] = {.x = screen_dim.width / 2, .y = screen_dim.height};
  base.vertices[2] = {.x = screen_dim.width, .y = 0};

  int xi = GetRandomInt(0, screen_dim.height);
  int yi = GetRandomInt(0, screen_dim.width);
  sierpinski::graphics::DrawChar({.x = xi, .y = yi}, '*', GetRandColor());

  int index = 0;
  const int kMaxIter = 10000;
  for (int i = 0; i < kMaxIter; ++i) {
    index = GetRandomInt(0, std::numeric_limits<int>::max()) %
            sierpinski::common::kTriangleVertices;

    xi = (xi + base.vertices[index].x) / 2;
    yi = (yi + base.vertices[index].y) / 2;

    sierpinski::graphics::DrawChar({.x = xi, .y = yi}, '*', GetRandColor());
  }
}

int main() {
  std::optional<sierpinski::graphics::ScreenDimension> screen_dim =
      sierpinski::graphics::InitScreen();
  if (!screen_dim) {
    PrintErrAndExit("failed to initialize screen");
  }

  /* Show us the Sierpinski Triangles! */
  DrawSierpinskiTriangles(*screen_dim);

  /* Print a banner telling the user how to exit. */
  sierpinski::graphics::DrawStr("press any key to quit",
                                {.x = 0, .y = screen_dim->height - 1});

  /* Wait for the user to press a key before cleaning up. */
  while (!sierpinski::graphics::UserPressedKey()) {
  }

  /* Cleanup. */
  sierpinski::graphics::TerminateScreen();

  std::exit(EXIT_SUCCESS);
}

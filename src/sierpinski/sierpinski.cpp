#include <getopt.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

#include "common/types.h"
#include "graphics/screen.h"
#include "util/triangles.h"

static void PrintUsage() noexcept {
  std::cout << "usage: sierpinski [OPTION]..." << std::endl;
  std::cout << "an ncurses rendering of sierpinski's triangle" << std::endl;
  std::cout << "\t-d, --degree\tfractal degree (default 2)" << std::endl;
  std::cout << "\t-h, --help\tprint this help page" << std::endl;
}

static void PrintErrAndExit(const std::string& err_msg) noexcept {
  std::cerr << "error: " << err_msg << std::endl;
  std::exit(EXIT_FAILURE);
}

[[nodiscard]] static unsigned int ParseUnsignedInt(const char* optarg) {
  unsigned int value = 0;
  try {
    value = std::stoul(optarg);
  } catch (const std::invalid_argument& e) {
    PrintErrAndExit("cannot convert '" + std::string(optarg) + "' to uint");
  } catch (const std::out_of_range& e) {
    PrintErrAndExit(std::string(optarg) + " is out of uint range");
  }
  return value;
}

static void Sierpinski(const sierpinski::Triangle& triangle,
                       int degree) noexcept {
  sierpinski::graphics::DrawTriangle(triangle);
  if (degree) {
    const sierpinski::Point2D& v0 = triangle.vertices[0];
    const sierpinski::Point2D& v1 = triangle.vertices[1];
    const sierpinski::Point2D& v2 = triangle.vertices[2];

    sierpinski::Triangle s1;
    s1.vertices[0] = v0;
    s1.vertices[1] = sierpinski::util::MidPoint(v0, v1);
    s1.vertices[2] = sierpinski::util::MidPoint(v0, v2);

    sierpinski::Triangle s2;
    s2.vertices[0] = v1;
    s2.vertices[1] = sierpinski::util::MidPoint(v0, v1);
    s2.vertices[2] = sierpinski::util::MidPoint(v1, v2);

    sierpinski::Triangle s3;
    s3.vertices[0] = v2;
    s3.vertices[1] = sierpinski::util::MidPoint(v2, v1);
    s3.vertices[2] = sierpinski::util::MidPoint(v0, v2);

    Sierpinski(s1, degree - 1);
    Sierpinski(s2, degree - 1);
    Sierpinski(s3, degree - 1);
  }
}

static void DrawSierpinskiTriangles(unsigned int degree) noexcept {
  /* Setup a blank screen. */
  std::optional<sierpinski::graphics::ScreenDimension> screen_dim =
      sierpinski::graphics::InitScreen();
  if (!screen_dim) {
    PrintErrAndExit("failed to initialize screen");
  }

  /* Construction of the initial triangle shown on the screen. This is
   * the fractal with degree = 0. The code below tries to make the initial
   * triangle fill the entire screen. */
  sierpinski::Point2D screen_center = {.x = screen_dim->width / 2,
                                       .y = screen_dim->height / 2};
  int max_dist =
      std::min(screen_dim->width / 2 - 1, screen_dim->height / 2 - 1);
  sierpinski::Triangle triangle =
      sierpinski::util::CreateCenteredTriangle(screen_center, max_dist);

  /* The actual algorithm magic happens here. */
  Sierpinski(triangle, degree);

  /* Place prompt telling the user how to exit. */
  sierpinski::graphics::DrawInstructions(*screen_dim);

  /* Wait for the user to press a key before cleaning up. */
  while (!sierpinski::graphics::UserPressedKey()) {
  }

  /* Cleanup. */
  sierpinski::graphics::TerminateScreen();
}

int main(int argc, char** argv) {
  struct option long_options[] = {
      {"degree", required_argument, 0, 'd'},
      {"help", no_argument, 0, 'h'},
      {0, 0, 0, 0},
  };

  int opt = 0;
  int long_index = 0;
  unsigned int degree = 2;
  while (-1 != (opt = ::getopt_long(argc, argv, "hd:s:r:",
                                    static_cast<struct option*>(long_options),
                                    &long_index))) {
    switch (opt) {
      case 'd':
        degree = ParseUnsignedInt(optarg);
        break;
      case 'h':
        PrintUsage();
        std::exit(EXIT_SUCCESS);
      case '?':
        std::cerr << "error: unknown option -> " << opt << std::endl;
        std::exit(EXIT_FAILURE);
    }
  }

  DrawSierpinskiTriangles(degree);

  std::exit(EXIT_SUCCESS);
}

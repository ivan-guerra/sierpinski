#include <getopt.h>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <thread>

#include "common/types.h"
#include "graphics/screen.h"

static void PrintUsage() noexcept {
  std::cout << "usage: sierpinski [OPTION]..." << std::endl;
  std::cout << "an ncurses rendering of sierpinski's triangle" << std::endl;
  std::cout << "\t-i, --max-iterations\tmax number of simulation iterations "
               "(default 10000)"
            << std::endl;
  std::cout << "\t-r, --refresh-rate\tdelay between iterations in microseconds "
               "(default 100)"
            << std::endl;
  std::cout << "\t-h, --help\t\tprint this help page" << std::endl;
}

static void PrintErrAndExit(const std::string& err_msg) noexcept {
  std::cerr << "error: " << err_msg;
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
    const sierpinski::graphics::ScreenDimension& screen_dim,
    unsigned int max_iterations, unsigned int refresh_rate_usec) noexcept {
  sierpinski::common::Triangle base;
  base.vertices[0] = {.x = 0, .y = 0};
  base.vertices[1] = {.x = screen_dim.width / 2, .y = screen_dim.height};
  base.vertices[2] = {.x = screen_dim.width, .y = 0};

  int xi = GetRandomInt(0, screen_dim.height);
  int yi = GetRandomInt(0, screen_dim.width);
  sierpinski::graphics::DrawChar({.x = xi, .y = yi}, '*', GetRandColor());

  int index = 0;
  for (unsigned int i = 0; i < max_iterations; ++i) {
    index = GetRandomInt(0, std::numeric_limits<int>::max()) %
            sierpinski::common::kTriangleVertices;

    xi = (xi + base.vertices[index].x) / 2;
    yi = (yi + base.vertices[index].y) / 2;

    sierpinski::graphics::DrawChar({.x = xi, .y = yi}, '*', GetRandColor());

    std::this_thread::sleep_for(std::chrono::microseconds(refresh_rate_usec));
  }
}

static void RunDrawLoop(unsigned int max_iterations,
                        unsigned int refresh_rate_usec) noexcept {
  std::optional<sierpinski::graphics::ScreenDimension> screen_dim =
      sierpinski::graphics::InitScreen();
  if (!screen_dim) {
    PrintErrAndExit("failed to initialize screen");
  }

  /* Show us the Sierpinski Triangles! */
  DrawSierpinskiTriangles(*screen_dim, max_iterations, refresh_rate_usec);

  /* Print a banner telling the user how to exit. */
  sierpinski::graphics::DrawStr("press any key to quit",
                                {.x = 0, .y = screen_dim->height - 1});

  /* Wait for the user to press a key before cleaning up. */
  while (!sierpinski::graphics::UserPressedKey()) {
  }

  /* Cleanup. */
  sierpinski::graphics::TerminateScreen();
}

int main(int argc, char** argv) {
  struct option long_options[] = {
      {"max-iterations", required_argument, 0, 'i'},
      {"refresh-rate", required_argument, 0, 'r'},
      {"help", no_argument, 0, 'h'},
      {0, 0, 0, 0},
  };

  int opt = 0;
  int long_index = 0;
  unsigned int max_iterations = 10000;
  unsigned int refresh_rate_usec = 100;
  while (-1 != (opt = ::getopt_long(argc, argv, "hi:r:",
                                    static_cast<struct option*>(long_options),
                                    &long_index))) {
    switch (opt) {
      case 'i':
        max_iterations = ParseUnsignedInt(optarg);
        break;
      case 'r':
        refresh_rate_usec = ParseUnsignedInt(optarg);
        break;
      case 'h':
        PrintUsage();
        std::exit(EXIT_SUCCESS);
      case '?':
        std::cerr << "error: unknown option -> " << opt << std::endl;
        std::exit(EXIT_FAILURE);
    }
  }

  RunDrawLoop(max_iterations, refresh_rate_usec);

  std::exit(EXIT_SUCCESS);
}

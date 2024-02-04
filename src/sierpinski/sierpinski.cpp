#include <getopt.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "common/types.h"
#include "graphics/screen.h"
#include "util/triangles.h"

static void PrintUsage() noexcept {
  std::cout << "usage: sierpinski [OPTION]..." << std::endl;
  std::cout << "an ncurses rendering of sierpinski's triangle" << std::endl;
  std::cout << "\t-d, --degree\t\tfractal degree" << std::endl;
  std::cout << "\t-r, --refresh-rate\tscreen refresh rate in milliseconds"
            << std::endl;
  std::cout << "\t-h, --help\t\tprint this help page" << std::endl;
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

static void RunDrawLoop(unsigned int degree,
                        unsigned int refresh_rate_ms) noexcept {
  /* Setup a blank screen. */
  std::optional<sierpinski::graphics::ScreenDimension> screen_dim =
      sierpinski::graphics::InitScreen();
  if (!screen_dim) {
    PrintErrAndExit("failed to initialize screen");
  }

  /* The refresh rate drives how long we wait for user input before increasing
   * the degree of the fractal and drawing it on screen. */
  sierpinski::graphics::EnableInputDelay(refresh_rate_ms);

  std::vector<sierpinski::Triangle> triangles(
      1,
      sierpinski::util::CreateCenteredTriangle(
          {.x = screen_dim->width / 2, .y = screen_dim->height / 2}, degree));
  while (!sierpinski::graphics::UserRequestedToQuit()) {
    sierpinski::graphics::DrawTriangles(triangles);
    sierpinski::graphics::DrawInstructions(*screen_dim);
  }

  /* Cleanup. */
  sierpinski::graphics::DisableInputDelay();
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
  unsigned int degree = 8;
  unsigned int refresh_rate_ms = 100;
  while (-1 != (opt = ::getopt_long(argc, argv, "hd:s:r:",
                                    static_cast<struct option*>(long_options),
                                    &long_index))) {
    switch (opt) {
      case 'd':
        degree = ParseUnsignedInt(optarg);
        break;
      case 'r':
        refresh_rate_ms = ParseUnsignedInt(optarg);
        break;
      case 'h':
        PrintUsage();
        std::exit(EXIT_SUCCESS);
      case '?':
        std::cerr << "error: unknown option -> " << opt << std::endl;
        std::exit(EXIT_FAILURE);
    }
  }

  RunDrawLoop(degree, refresh_rate_ms);

  std::exit(EXIT_SUCCESS);
}

#include <getopt.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

static void PrintUsage() noexcept {
  std::cout << "usage: sierpinski [OPTION]..." << std::endl;
  std::cout << "an ncurses rendering of sierpinski's triangle" << std::endl;
  std::cout << "\t-d, --degree\t\tfractal degree" << std::endl;
  std::cout << "\t-s, --side-length\tinitial triangle side length" << std::endl;
  std::cout << "\t-r, --refresh-rate\tscreen refresh rate in seconds"
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

int main(int argc, char** argv) {
  struct option long_options[] = {
      {"degree", required_argument, 0, 'd'},
      {"help", no_argument, 0, 'h'},
      {0, 0, 0, 0},
  };

  int opt = 0;
  int long_index = 0;
  unsigned int degree = 0;
  unsigned int side_length = 10;
  unsigned int refresh_rate_sec = 1;
  while (-1 != (opt = ::getopt_long(argc, argv, "hd:s:r:",
                                    static_cast<struct option*>(long_options),
                                    &long_index))) {
    switch (opt) {
      case 'd':
        degree = ParseUnsignedInt(optarg);
        break;
      case 's':
        side_length = ParseUnsignedInt(optarg);
        break;
      case 'r':
        refresh_rate_sec = ParseUnsignedInt(optarg);
        break;
      case 'h':
        PrintUsage();
        std::exit(EXIT_SUCCESS);
      case '?':
        std::cerr << "error: unknown option -> " << opt << std::endl;
        std::exit(EXIT_FAILURE);
    }
  }
  std::exit(EXIT_SUCCESS);
}

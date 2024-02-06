# sierpinski

https://github.com/ivan-guerra/sierpinski/assets/13009513/b6e31e62-b2a4-45eb-a7a9-85709cbfd73c

`sierpinski` is a Linux only application that renders the [Sierpinski
triangle][1] in your terminal. This implementation takes the [Chaos Game][2]
approach to generating the triangle.

### Building

To build the project locally, you will need the following libraries and tools
installed:

* CMake3.27+
* C++ compiler supporting C++20 features
* [Doxygen][3]
* Ncurses Developer Libs

To build, change directory to `scripts/linux/` and run `build.sh`.

After a successful build, you will find the binary installed to `sierpinski/bin/`.

### Program Usage

Below is the program usage message that can be seen by running `sierpinski
--help`:

```text
usage: sierpinski [OPTION]...
an ncurses rendering of sierpinski's triangle
	-i, --max-iterations	max number of simulation iterations (default 10000)
	-r, --refresh-rate	delay between iterations in microseconds (default 100)
	-h, --help		print this help page
```

You can control the speed at which the generation occurs using the
`--refresh-rate` option. The `--max-iterations` option allows one to specify how
many points are generated in total.

### Doxygen Docs

This project is documented using Doxygen. Doxygen docs are built automatically
by the Linux build script. Docs can be found under `sierpinski/docs/html/`.

[1]: https://en.wikipedia.org/wiki/Sierpi%C5%84ski_triangle
[2]: https://en.wikipedia.org/wiki/Sierpi%C5%84ski_triangle#Chaos_game
[3]: https://www.doxygen.nl/


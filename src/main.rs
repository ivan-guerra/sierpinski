use clap::Parser;

#[derive(Parser, Debug)]
#[command(version, about, long_about = None)]
struct Args {
    #[arg(
        short = 'i', 
        long,
        default_value_t = 10_000, 
        value_parser = clap::value_parser!(u32).range(1..=1_000_000),
        help = "max number of simulation iterations")]
    max_iterations: u32,

    #[arg(
        short = 'r', 
        long,
        default_value_t = 1, 
        value_parser = clap::value_parser!(u64).range(1..=3_600_000),
        help = "delay between iterations in milliseconds")]
    refresh_rate_msec: u64,

}

fn main() {
    let args = Args::parse();
    let config = sierpinski::Config::new(
        sierpinski::ScreenDimension::new(800, 600),
        args.max_iterations,
        args.refresh_rate_msec,
    );

    sierpinski::draw_sierpinski_triangles(&config);
}

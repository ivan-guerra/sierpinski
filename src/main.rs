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
        default_value_t = 100, 
        value_parser = clap::value_parser!(u64).range(1..=1_000_000),
        help = "delay between iterations in microseconds")]
    refresh_rate_msec: u64,

}

fn main() {
    let args = Args::parse();
    let screen_dim = crossterm::terminal::size().unwrap();
    let config = sierpinski::Config::new(
        sierpinski::ScreenDimension::new(screen_dim.0, screen_dim.1),
        args.max_iterations,
        args.refresh_rate_msec,
    );

    if let Err(e) = sierpinski::run_draw_loop(&config) {
        eprintln!("error: {}", e);
        std::process::exit(1);
    } 
}

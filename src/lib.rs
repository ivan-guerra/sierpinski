use std::thread::sleep;
use std::time::Duration;

pub struct ScreenDimension {
    pub width: u32,
    pub height: u32,
}

impl ScreenDimension {
    pub fn new(width: u32, height: u32) -> ScreenDimension {
        ScreenDimension { width, height }
    }
}

pub struct Config {
    pub screen_dim: ScreenDimension,
    pub max_iterations: u32,
    pub refresh_rate_msec: u64,
}

impl Config {
    pub fn new(screen_dim: ScreenDimension, max_iterations: u32, refresh_rate_msec: u64) -> Config {
        Config {
            screen_dim,
            max_iterations,
            refresh_rate_msec,
        }
    }
}

struct Point {
    x: u32,
    y: u32,
}

impl Point {
    fn new(x: u32, y: u32) -> Point {
        Point { x, y }
    }
}

struct Triangle {
    vertices: [Point; 3],
}

impl Triangle {
    fn new(p1: Point, p2: Point, p3: Point) -> Triangle {
        Triangle {
            vertices: [p1, p2, p3],
        }
    }
}

pub fn draw_sierpinski_triangles(config: &Config) {
    let base = Triangle::new(
        Point::new(0, 0),
        Point::new(config.screen_dim.width / 2, config.screen_dim.height),
        Point::new(config.screen_dim.width, 0),
    );

    let mut xi = rand::random::<u32>() % config.screen_dim.height;
    let mut yi = rand::random::<u32>() % config.screen_dim.width;

    println!("xi: {}, yi: {}", xi, yi);

    for _ in 0..config.max_iterations {
        let index = rand::random::<usize>() % 3;

        xi = (xi + base.vertices[index].x) / 2;
        yi = (yi + base.vertices[index].y) / 2;

        println!("xi: {}, yi: {}", xi, yi);

        sleep(Duration::from_millis(config.refresh_rate_msec));
    }
}

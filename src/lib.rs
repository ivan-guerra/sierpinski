use crossterm::{
    cursor::{Hide, MoveTo, Show},
    event::{self, Event, KeyCode},
    style::{Color, Print, SetForegroundColor},
    terminal::{self, Clear, ClearType, EnterAlternateScreen, LeaveAlternateScreen},
    ExecutableCommand,
};
use std::error::Error;
use std::io::{stdout, Write};
use std::thread::sleep;
use std::time::Duration;

pub struct ScreenDimension {
    pub width: u16,
    pub height: u16,
}

impl ScreenDimension {
    pub fn new(width: u16, height: u16) -> ScreenDimension {
        ScreenDimension { width, height }
    }
}

pub struct Config {
    pub screen_dim: ScreenDimension,
    pub max_iterations: u32,
    pub refresh_rate_usec: u64,
}

impl Config {
    pub fn new(screen_dim: ScreenDimension, max_iterations: u32, refresh_rate_usec: u64) -> Config {
        Config {
            screen_dim,
            max_iterations,
            refresh_rate_usec,
        }
    }
}

struct Point {
    x: u16,
    y: u16,
}

impl Point {
    fn new(x: u16, y: u16) -> Point {
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

fn draw_asterisk_with_rand_color(position: &Point) -> Result<(), Box<dyn Error>> {
    let mut stdout = stdout();
    let colors = [
        Color::Red,
        Color::Green,
        Color::Yellow,
        Color::Blue,
        Color::Magenta,
        Color::Cyan,
        Color::White,
    ];
    let color = colors[rand::random::<usize>() % colors.len()];

    stdout.execute(MoveTo(position.x, position.y))?;
    stdout.execute(SetForegroundColor(color))?;
    stdout.execute(Print("*"))?;

    Ok(())
}

fn draw_sierpinski_triangles(config: &Config) -> Result<(), Box<dyn Error>> {
    let base = Triangle::new(
        Point::new(0, 0),
        Point::new(config.screen_dim.width / 2, config.screen_dim.height),
        Point::new(config.screen_dim.width, 0),
    );

    let mut xi = rand::random::<u16>() % config.screen_dim.height;
    let mut yi = rand::random::<u16>() % config.screen_dim.width;

    draw_asterisk_with_rand_color(&Point::new(xi, yi))?;

    for _ in 0..config.max_iterations {
        let index = rand::random::<usize>() % 3;

        xi = (xi + base.vertices[index].x) / 2;
        yi = (yi + base.vertices[index].y) / 2;

        draw_asterisk_with_rand_color(&Point::new(xi, yi))?;

        sleep(Duration::from_micros(config.refresh_rate_usec));
    }

    Ok(())
}

pub fn run_draw_loop(config: &Config) -> Result<(), Box<dyn Error>> {
    let mut stdout = stdout();

    // Enter raw mode, alternate screen, clear it, and hide the cursor
    terminal::enable_raw_mode()?;
    stdout.execute(EnterAlternateScreen)?;
    stdout.execute(Clear(ClearType::All))?;
    stdout.execute(Hide)?;

    // Draw the Sierpinski triangles
    draw_sierpinski_triangles(config)?;

    // Add the text "Press 'q' to quit" to the bottom of the screen
    stdout.execute(MoveTo(0, config.screen_dim.height - 1))?;
    stdout.execute(SetForegroundColor(Color::White))?;
    stdout.execute(Print("press 'q' to quit"))?;

    // Flush to make sure everything is printed
    stdout.flush()?;

    // Wait for a key press to exit
    loop {
        if event::poll(std::time::Duration::from_millis(100))? {
            // Check if an event is a key press
            if let Event::Key(key_event) = event::read()? {
                if key_event.code == KeyCode::Char('q') || key_event.code == KeyCode::Esc {
                    break;
                }
            }
        }
    }

    // Reset terminal state before exit
    stdout.execute(Clear(ClearType::All))?;
    stdout.execute(Show)?;
    stdout.execute(LeaveAlternateScreen)?;
    terminal::disable_raw_mode()?;

    Ok(())
}

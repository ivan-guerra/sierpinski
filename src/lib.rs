//! A Sierpinski triangle visualization in the terminal.
//!
//! This module implements the chaos game method to generate
//! a Sierpinski triangle pattern using ASCII characters and
//! terminal colors.
//!
//! # Example
//!
//! ```rust
//! use sierpinski::{Config, ScreenDimension};
//!
//! let config = Config {
//!     screen_dim: ScreenDimension { width: 80, height: 24 },
//!     max_iterations: 1000,
//!     refresh_rate_usec: 10000,
//! };
//!
//! run_draw_loop(&config)?;
//! ```
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

/// Represents the dimensions of a terminal screen in characters.
///
/// The coordinate system starts at (0, 0) in the top-left corner,
/// with x increasing to the right and y increasing downward.
pub struct ScreenDimension {
    /// Width of the screen in characters/columns (x-axis), starting from 0.
    pub width: u16,
    /// Height of the screen in rows (y-axis), starting from 0.
    pub height: u16,
}

impl ScreenDimension {
    pub fn new(width: u16, height: u16) -> ScreenDimension {
        ScreenDimension { width, height }
    }
}

/// Configuration for the Sierpinski triangle visualization.
pub struct Config {
    /// The dimensions of the terminal screen.
    pub screen_dim: ScreenDimension,
    /// Maximum number of points to plot.
    pub max_iterations: u32,
    /// Delay between plotting points in microseconds.
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

/// Generates and displays a Sierpinski triangle using the chaos game method.
///
/// # Algorithm
///
/// 1. Creates a base triangle using the screen dimensions.
/// 2. Starts with a random point.
/// 3. Repeatedly:
///    - Selects a random vertex of the base triangle.
///    - Moves halfway from current point to selected vertex.
///    - Plots the new point with a random color.
///
/// # Arguments
///
/// * `config` - Configuration settings including screen dimensions,
///             iteration count, and refresh rate.
///
/// # Returns
///
/// * `Result<(), Box<dyn Error>>` - Ok if successful, Error if drawing fails.
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

/// Runs the main drawing loop for the Sierpinski triangle visualization.
///
/// # Details
///
/// This function:
/// 1. Sets up the terminal in raw mode with an alternate screen.
/// 2. Draws the Sierpinski triangle pattern.
/// 3. Displays a quit message.
/// 4. Waits for 'q' or Esc key to exit.
/// 5. Restores the terminal state.
///
/// # Arguments
///
/// * `config` - Configuration settings including screen dimensions,
///             iteration count, and refresh rate.
///
/// # Returns
///
/// * `Result<(), Box<dyn Error>>` - Ok if successful, Error if terminal operations fail.
///
/// # Errors
///
/// Returns an error if:
/// - Terminal mode changes fail.
/// - Drawing operations fail.
/// - Event polling fails.
pub fn run_draw_loop(config: &Config) -> Result<(), Box<dyn Error>> {
    let mut stdout = stdout();

    // Enter raw mode, alternate screen, clear it, and hide the cursor.
    terminal::enable_raw_mode()?;
    stdout.execute(EnterAlternateScreen)?;
    stdout.execute(Clear(ClearType::All))?;
    stdout.execute(Hide)?;

    // Draw the Sierpinski triangles.
    draw_sierpinski_triangles(config)?;

    // Add the text "Press 'q' to quit" to the bottom of the screen.
    stdout.execute(MoveTo(0, config.screen_dim.height - 1))?;
    stdout.execute(SetForegroundColor(Color::White))?;
    stdout.execute(Print("press 'q' to quit"))?;

    // Flush to make sure everything is printed.
    stdout.flush()?;

    // Wait for a key press to exit.
    loop {
        if event::poll(std::time::Duration::from_millis(100))? {
            // Check if an event is a key press.
            if let Event::Key(key_event) = event::read()? {
                if key_event.code == KeyCode::Char('q') || key_event.code == KeyCode::Esc {
                    break;
                }
            }
        }
    }

    // Reset terminal state before exit.
    stdout.execute(Clear(ClearType::All))?;
    stdout.execute(Show)?;
    stdout.execute(LeaveAlternateScreen)?;
    terminal::disable_raw_mode()?;

    Ok(())
}

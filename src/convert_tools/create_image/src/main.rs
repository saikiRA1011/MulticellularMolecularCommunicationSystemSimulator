extern crate csv;

use serde::Deserialize;
use std::fs::File;
use std::process;
use std::{error::Error, vec};
// 基本的なアイテムをインポート
use nannou::{draw, prelude::*};

fn captured_frame_path(app: &App, frame: &Frame) -> std::path::PathBuf {
    app.project_path()
        .expect("failed to locate `project_path`")
        .join("../../../image")
        .join(format!("cells_{:05}", frame.nth()))
        .with_extension("png")
}

fn fopen(path: &str) -> Result<File, Box<dyn Error>> {
    let file = File::open(path)?;
    Ok(file)
}

#[derive(Debug, Deserialize)]
struct Cell {
    id: u32,
    cell_type: String,
    x: f32,
    y: f32,
    z: f32,
    vx: f32,
    vy: f32,
    vz: f32,
    r: f32,
    contact: u32,
    contact_IDs: String,
}

fn csv_parse(file: File) -> Result<Vec<Cell>, Box<dyn Error>> {
    let mut note_list = Vec::new();

    // タブ文字区切りでパースする
    let mut rdr = csv::ReaderBuilder::new().delimiter(b'\t').from_reader(file);
    // let mut rdr = csv::Reader::from_reader(file);
    for result in rdr.records() {
        match result {
            Ok(_) => (),
            Err(err) => {
                println!("error: {}", err);
                println!("Failed to parse csv file.");
                process::exit(1);
            }
        }

        let record = result?.deserialize(None)?;
        note_list.push(record);
    }

    Ok(note_list)
}

// アプリケーションの状態を定義
struct Model {
    step: i32,
    max_step: u32,
    label_height: f32,
}

// アプリケーションの開始
fn main() {
    nannou::app(model)
        .update(update)
        .simple_window(view)
        .size(1024, 1024 + 30)
        .run();
}

// Model のインスタンスを生成
fn model(_app: &App) -> Model {
    let step = -1; // 0から開始するためにインクリメントを前提として-1で初期化
    let max_step = 100;
    let label_height = 30.0;
    Model {
        step,
        max_step,
        label_height,
    }
}

fn update(_app: &App, _model: &mut Model, _update: Update) {
    _model.step += 1;

    if _model.step as u32 >= _model.max_step {
        _app.quit();
    }
}

// 画面の描画処理
fn view(_app: &App, _model: &Model, _frame: Frame) {
    let draw = _app.draw();

    // 背景を白に設定
    draw.background().color(WHITE);

    // 画面に円を描画
    draw.ellipse()
        .x_y(0.0, 0.0 + _model.label_height / 2.0)
        .w_h(1024.0, 1024.0)
        .stroke(BLACK)
        .no_fill()
        .stroke_weight(2.0);

    draw.line()
        .start(pt2(0.0, (1024.0 + _model.label_height) / 2.0))
        .end(pt2(0.0, -1024.0 / 2.0 + _model.label_height / 2.0))
        .stroke_weight(2.0)
        .color(BLACK);
    draw.line()
        .start(pt2(1024.0 / 2.0, 0.0 + _model.label_height))
        .end(pt2(-1024.0 / 2.0, 0.0 + _model.label_height))
        .stroke_weight(2.0)
        .color(BLACK);
    draw.line()
        .start(pt2(
            -1024.0 / 2.0,
            -1024.0 / 2.0 + _model.label_height / 2.0,
        ))
        .end(pt2(1024.0 / 2.0, -1024.0 / 2.0 + _model.label_height / 2.0))
        .stroke_weight(2.0)
        .color(BLACK);

    // ラベル欄(右下)に文字を描画
    draw.text(&format!("step: {:>05}", _model.step))
        .x_y(0.0, -1024.0 / 2.0 + 5.0)
        .font_size(24)
        .color(BLACK);

    let file_path = &format!("../../../result/cells_{:>05}", _model.step);

    // match fopen(file_path) {
    //     Ok(_) => println!("Successfully opened csv file {}.", file_path),
    //     Err(err) => {
    //         println!("error: {}", err);
    //         println!("Failed to open csv file {}", file_path);
    //         process::exit(1);
    //     }
    // }
    match csv_parse(fopen(file_path).unwrap()) {
        Ok(cells) => {
            println!("Successfully parsed csv file {}.", file_path);

            for cell in cells {
                // TODO: 円の描画はやたらと遅い。なんとか考えてみる
                draw.ellipse()
                    .x_y(cell.x, cell.y + _model.label_height / 2.0)
                    .w_h(2.0 * cell.r, 2.0 * cell.r)
                    .stroke(GREEN)
                    .no_fill()
                    .stroke_weight(2.0);

                // 直接円を書くのではなく、線分で多角形を描画してみる。でもこれも遅い
                // let radius = cell.r; // store the radius of the circle we want to make
                // let points = (0..=360).step_by(36).map(|i| {
                //     // map over an array of integers from 0 to 360 to represent the degrees in a circle

                //     let radian = deg_to_rad((i) as f32); // convert each degree to radians
                //     let x = cell.x + radian.sin() * radius; // get the sine of the radian to find the x-co-ordinate of
                //                                             // this point of the circle, and multiply it by the radius
                //     let y = cell.y + _model.label_height / 2.0 + radian.cos() * radius; // do the same with cosine to find the y co-ordinate
                //     (pt2(x, y), GREEN) // construct and return a point object with a color
                // });
                // draw.polyline() // create a PathStroke Builder object
                //     .weight(2.0)
                //     .points_colored(points); // tell our PathStroke Builder to draw lines connecting our array of points
            }
        }
        Err(err) => {
            println!("Failed to parse csv file: {}", err);
            process::exit(1);
        }
    }

    draw.to_frame(_app, &_frame).unwrap(); // 描画の実行

    // 画面をキャプチャ
    let file_path = captured_frame_path(_app, &_frame);
    _app.main_window().capture_frame(file_path);
}

use std::collections::{HashMap, HashSet};
use std::env::args;
use std::fs::File;
use std::io::{LineWriter, Write};
use std::path::Path;
use std::time::Instant;

use crossterm::event::{self, Event, KeyCode, KeyEventKind};
use itertools::Itertools;
use lance_creator::model::{Force, Model, ModelForce};
use rand::Rng;
use ratatui::layout::{Alignment, Constraint, Layout, Rect};
use ratatui::style::Stylize;
use ratatui::widgets::{Block, Borders, Paragraph};
use ratatui::{DefaultTerminal, Frame};
use sorted_vec::SortedVec;

const MAX_ATTEMPTS: u16 = 50000;

struct DrawRects {
    title_area: Rect,
    gunnery_area: Rect,
    piloting_area: Rect,
    min_bv_area: Rect,
    max_bv_area: Rect,
    force_size_area: Rect,
    unit_type_area: Rect,
    era_area: Rect,
    faction_area: Rect,
    status_area: Rect
}

fn main() -> std::io::Result<()> {
    // let args: Vec<String> = args().collect();
    // let program = args[0].clone();
    // let program = Path::new(&program);
    // let program = program.file_name().unwrap().to_str().unwrap();

    // let filenames = vec![String::from("mechs.json")];
    // let mut loaded_models: Vec<Model> = Vec::new();
    // for filename in filenames {
    //     let file = match File::open(&filename) {
    //         Ok(f) => f,
    //         Err(e) => {
    //             println!("Error opening {}: {}", filename, e);
    //             return terminate(&program, &opts, 1);
    //         },
    //     };

    //     let mut file_models: Vec<Model> = match serde_json::from_reader(file) {
    //         Ok(m) => m,
    //         Err(e) => {
    //             println!("Error reading {}: {}", filename, e);
    //             return terminate(&program, &opts, 1);
    //         },
    //     };

    //     loaded_models.append(&mut file_models);
    // }
 
    // if loaded_models.is_empty() {
    //     println!("No files provided or files provided were empty");
    //     return terminate(&program, &opts, 1);
    // }

    // let model_forces: HashSet<ModelForce> = match generate_model_forces(num_forces, loaded_models, force_size, min_bv, max_bv, required_mech, skill) {
    //     Ok(m) => m,
    //     Err(e) => {
    //         println!("Failed to generate model forces: {}", e);
    //         return terminate(&program, &opts, 1);
    //     },
    // };

    // if model_forces.len() < num_forces {
    //     println!(
    //         "Only found {} forces within the BV range",
    //         model_forces.len()
    //     );
    // }

    // let path = Path::new("forces.csv");
    // let mut output_file = match File::create(path) {
    //     Ok(file) => file,
    //     Err(e) => panic!("Couldn't create {}: {}", path.display(), e),
    // };

    // let now = Instant::now();
    // generate_variants_and_write_out(&mut output_file, model_forces, min_bv, max_bv);

    let terminal = ratatui::init();
    let result = run(terminal);
    ratatui::restore();
    result
}

fn run(mut terminal: DefaultTerminal) -> std::io::Result<()> {
    loop {
        terminal.draw(draw_options)?;
        // process keystrokes
        if let Event::Key(key) = event::read()? {
            // handle the keys
            if key.kind == KeyEventKind::Press && key.code == KeyCode::Char('q') {
                break Ok(());
            }
        }
    }
}

fn draw_options(frame: &mut Frame) {
    let rects = calculate_layout(frame.area());

    // Title
    frame.render_widget(
        Paragraph::new("Lance Creator.  Press q to quit")
            .alignment(Alignment::Center),
        rects.title_area,
    );

    // Gunnery
    frame.render_widget(
        Block::new()
            .title("gunnery")
            .borders(Borders::ALL), 
        rects.gunnery_area,
    );

    // Piloting
    frame.render_widget(
        Block::new()
            .title("piloting")
            .borders(Borders::ALL), 
        rects.piloting_area,
    );

    // Min BV
    frame.render_widget(
        Block::new()
            .title("min BV")
            .borders(Borders::ALL), 
        rects.min_bv_area,
    );

    // Max BV
    frame.render_widget(
        Block::new()
            .title("max BV")
            .borders(Borders::ALL), 
        rects.max_bv_area,
    );

    // Force Size
    frame.render_widget(
        Block::new()
            .title("force size")
            .borders(Borders::ALL), 
        rects.force_size_area,
    );

    // Unit type
    frame.render_widget(
        Block::new()
            .title("unit types")
            .borders(Borders::ALL), 
        rects.unit_type_area,
    );

    // Era
    frame.render_widget(
        Block::new()
            .title("era")
            .borders(Borders::ALL), 
        rects.era_area,
    );

    // Faction
    frame.render_widget(
        Block::new()
            .title("faction")
            .borders(Borders::ALL), 
        rects.faction_area,
    );

    // Status bar
    frame.render_widget(
        Paragraph::new("This is where the status would be"),
        rects.status_area,
    );
}

fn calculate_layout(area: Rect) -> DrawRects {
    let main_layout = Layout::vertical([Constraint::Length(1), Constraint::Min(0), Constraint::Length(1)]);
    let field_layout = Layout::vertical([Constraint::Max(4), Constraint::Min(0)]);
    let [title_area, main_area, status_area] = main_layout.areas(area);
    let main_areas: [Rect; 2] = field_layout.areas(main_area);

    let top_layout = Layout::horizontal([Constraint::Fill(1); 5]);
    let top_area = main_areas[0];
    let [gunnery_area, piloting_area, min_bv_area, max_bv_area, force_size_area] = top_layout.areas(top_area);

    let bottom_layout = Layout::horizontal([Constraint::Fill(1); 3]);
    let bottom_area = main_areas[1];
    let [unit_type_area, era_area, faction_area] = bottom_layout.areas(bottom_area);

    DrawRects{
        title_area,
        gunnery_area,
        piloting_area,
        min_bv_area,
        max_bv_area,
        force_size_area,
        unit_type_area,
        era_area,
        faction_area,
        status_area
    }
}

fn generate_variants_and_write_out<W: Write>(output: &mut W, model_forces: HashSet<ModelForce>, min_bv: u32, max_bv: u32) {
    let mut output_file = LineWriter::new(output);
    for model_force in model_forces {
        let mut mech_vecs = Vec::new();
        for model in model_force.models.iter() {
            mech_vecs.push(model.variants.clone());
        }

        let combinations = mech_vecs
            .iter()
            .multi_cartesian_product()
            .collect::<Vec<_>>();
        for mech_vec in combinations.iter() {
            let mut force = Force {
                mechs: SortedVec::new(),
            };
            for mech in mech_vec {
                force.mechs.push((*mech).clone());
            }

            let bv = force.bv();
            if bv < min_bv || bv > max_bv {
                continue;
            }

            match output_file.write_all(force.to_csv().as_bytes()) {
                Ok(_) => (),
                Err(e) => panic!("Failed to write line to file: {}", e),
            }
        }
    }
}

fn generate_model_forces(num_forces: usize, loaded_models: Vec<Model>, force_size: usize, min_bv: u32, max_bv: u32, skill: u8) -> Result<HashSet<ModelForce>, &'static str> {
    let mut rng = rand::thread_rng();
    let mut attempts = 0;
    let mut model_forces = HashSet::new();
    let skill_mul = *get_skill_map().get(&skill).expect("Skill was already validated");
    while model_forces.len() < num_forces && attempts < MAX_ATTEMPTS {
        attempts += 1;
        let mut force = ModelForce {
            models: SortedVec::new(),
        };
        let mut copied_models = loaded_models.clone();
        for i in 0..force_size {
            let n = rng.gen_range(0..copied_models.len());
            let mut model = copied_models.swap_remove(n);
            model.adjust_bv(skill_mul);
            force.models.insert(model);
        }

        if force.max_bv() < min_bv || force.min_bv() > max_bv {
            continue;
        }

        println!("Created valid force");
        model_forces.insert(force);
    }

    return Ok(model_forces)
}

fn get_skill_map() -> HashMap<u8, f64> {
    let skill_map = HashMap::from([
        (00, 2.42),
        (01, 2.31),
        (02, 2.21),
        (03, 2.10),
        (04, 1.93),
        (05, 1.75),
        (06, 1.68),
        (07, 1.59),
        (08, 1.50),
        (10, 2.21),
        (11, 2.11),
        (12, 2.02),
        (13, 1.92),
        (14, 1.76),
        (15, 1.60),
        (16, 1.54),
        (17, 1.46),
        (18, 1.38),
        (20, 1.93),
        (21, 1.85),
        (22, 1.76),
        (23, 1.68),
        (24, 1.54),
        (25, 1.40),
        (26, 1.35),
        (27, 1.28),
        (28, 1.21),
        (30, 1.66),
        (31, 1.58),
        (32, 1.51),
        (33, 1.44),
        (34, 1.32),
        (35, 1.20),
        (36, 1.16),
        (37, 1.10),
        (38, 1.04),
        (40, 1.38),
        (41, 1.32),
        (42, 1.26),
        (43, 1.20),
        (44, 1.10),
        (45, 1.00),
        (46, 0.95),
        (47, 0.90),
        (48, 0.85),
        (50, 1.31),
        (51, 1.19),
        (52, 1.13),
        (53, 1.08),
        (54, 0.99),
        (55, 0.90),
        (56, 0.86),
        (57, 0.81),
        (58, 0.77),
        (60, 1.24),
        (61, 1.12),
        (62, 1.07),
        (63, 1.02),
        (64, 0.94),
        (65, 0.85),
        (66, 0.81),
        (67, 0.77),
        (68, 0.72),
        (70, 1.17),
        (71, 1.06),
        (72, 1.01),
        (73, 0.96),
        (74, 0.88),
        (75, 0.80),
        (76, 0.76),
        (77, 0.72),
        (78, 0.68),
        (80, 1.10),
        (81, 0.99),
        (82, 0.95),
        (83, 0.90),
        (84, 0.83),
        (85, 0.75),
        (86, 0.71),
        (87, 0.68),
        (88, 0.64),
    ]);

    skill_map
}

#[cfg(test)]
mod tests {
    use crate::*;
    use lance_creator::model::Mech;
    use rstest::{rstest, fixture};

    // #[fixture]
    // fn model_forces() -> HashSet<ModelForce> {
    //     let first_mech_first_variant = Mech{
    //         name: String::from("a"),
    //         variant: String::from("1"),
    //         bv: 100
    //     };
    //     let first_mech_second_variant = Mech{
    //         name: String::from("a"),
    //         variant: String::from("2"),
    //         bv: 5000
    //     };
    //     let first_mech: Model = Model {
    //         name: String::from("a"),
    //         count: 1,
    //         variants: vec![first_mech_first_variant, first_mech_second_variant]
    //     };

    //     let second_mech_first_variant = Mech{
    //         name: String::from("b"),
    //         variant: String::from("1"),
    //         bv: 10
    //     };
    //     let second_mech_second_variant = Mech{
    //         name: String::from("b"),
    //         variant: String::from("2"),
    //         bv: 500
    //     };
    //     let second_mech: Model = Model {
    //         name: String::from("b"),
    //         count: 1,
    //         variants: vec![second_mech_first_variant, second_mech_second_variant]
    //     };


    //     let first_mech = ModelForce{
    //         models: SortedVec::from_unsorted(vec![first_mech, second_mech])
    //     };

    //     let mut ret = HashSet::new();
    //     ret.insert(first_mech);
    //     ret
    // }

    // #[rstest]
    // #[case::bv_too_low(0, 10, "")]
    // #[case::a1_and_b1(0, 110, "a 1,b 1,110\n")]
    // #[case::a1_and_b(0, 600, "a 1,b 1,110\na 1,b 2,600\n")]
    // #[case::a2_and_b1(5000, 5100, "a 2,b 1,5010\n")]
    // #[case::a2_and_b(5000, 10000, "a 2,b 1,5010\na 2,b 2,5500\n")]
    // #[case::a_and_b(0, 10000, "a 1,b 1,110\na 1,b 2,600\na 2,b 1,5010\na 2,b 2,5500\n")]
    // #[case::bv_too_high(10000, 50000, "")]
    // fn test_generate_variants(model_forces: HashSet<ModelForce>, #[case] min_bv: u32, #[case] max_bv: u32, #[case] expected: String) {
    //     let mut writer: Vec<u8> = Vec::new();
    //     generate_variants_and_write_out(&mut writer, model_forces, min_bv, max_bv);
    //     let result = String::from_utf8(writer).unwrap();
    //     assert_eq!(expected, result);
    // }
}
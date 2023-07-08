use std::collections::HashSet;
use std::env::args;
use std::fs::File;
use std::io::{LineWriter, Write};
use std::path::Path;
use std::process::exit;
use std::time::Instant;

use getopts::Options;
use itertools::Itertools;
use lance_creator::model::{Force, Model, ModelForce};
use rand::Rng;
use sorted_vec::SortedVec;
use spinners::{Spinner, Spinners};

const MAX_ATTEMPTS: u16 = 5000;

fn main() {
    let args: Vec<String> = args().collect();
    let program = args[0].clone();

    let mut opts = Options::new();
    opts.reqopt("f", "file", "Input file of mechs", "FILE");
    opts.optopt("b", "bv", "Target BV (default: 5000)", "BV");
    opts.optopt("m", "minBv", "Minimum acceptable BV (default: 4900)", "BV");
    opts.optopt(
        "s",
        "size",
        "How many units in a force (default: 4, max: 6",
        "SIZE",
    );
    opts.optopt(
        "n",
        "numForces",
        "How many distinct sets of model to create options for (default: 3, max: 5)",
        "FORCES",
    );
    opts.optflag("h", "help", "Print this help menu");

    let matches = match opts.parse(&args[1..]) {
        Ok(m) => m,
        Err(_) => return terminate(&program, &opts, 0),
    };

    if matches.opt_present("h") {
        terminate(&program, &opts, 0);
    }

    let file = File::open(matches.opt_str("f").unwrap()).unwrap();
    let loaded_models: Vec<Model> = serde_json::from_reader(file).unwrap();

    let force_size = match matches.opt_get_default("s", 4) {
        Ok(s) if s > 6 => return terminate(&program, &opts, 1),
        Ok(s) => s,
        Err(_) => return terminate(&program, &opts, 1),
    };

    let min_bv = match matches.opt_get_default("m", 4900) {
        Ok(m) => m,
        Err(_) => return terminate(&program, &opts, 1),
    };

    let max_bv = match matches.opt_get_default("b", 5000) {
        Ok(b) => b,
        Err(_) => return terminate(&program, &opts, 1),
    };

    let num_forces = match matches.opt_get_default("n", 3) {
        Ok(f) if f > 5 => return terminate(&program, &opts, 1),
        Ok(f) => f,
        Err(_) => return terminate(&program, &opts, 1),
    };

    let mut rng = rand::thread_rng();
    let mut model_forces: HashSet<ModelForce> = HashSet::new();
    let mut attempts = 0;
    while model_forces.len() < num_forces && attempts < MAX_ATTEMPTS {
        attempts += 1;
        let mut force = ModelForce {
            models: SortedVec::new(),
        };
        let mut copied_models = loaded_models.clone();
        for i in 0..force_size {
            let n = rng.gen_range(0..copied_models.len());
            let model = copied_models.get_mut(n).unwrap();
            if model.count > 1 {
                force.models.insert(model.clone());
                model.count -= 1;
            } else {
                let model = copied_models.swap_remove(i);
                force.models.insert(model);
            }
        }

        if force.max_bv() < min_bv || force.min_bv() > max_bv {
            continue;
        }

        println!("Created valid force");
        model_forces.insert(force);
    }

    if model_forces.len() < num_forces {
        println!(
            "Only found {} forces within the BV range",
            model_forces.len()
        );
    }

    let path = Path::new("forces.csv");
    let output_file = match File::create(path) {
        Ok(file) => file,
        Err(e) => panic!("Couldn't create {}: {}", path.display(), e),
    };

    let mut output_file = LineWriter::new(output_file);

    // TODO: output total time taken
    // TODO: some sort of "working on it" output so we don't think we're frozen
    let now = Instant::now();
    let mut spinner = Spinner::new(Spinners::Line, "Generating force variants".into());
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

    spinner.stop_with_message(format!("Created forces in {}s", now.elapsed().as_secs()));
}

fn terminate(program: &str, opts: &Options, code: i32) {
    print_usage(program, opts);
    exit(code);
}

fn print_usage(program: &str, opts: &Options) {
    let brief = format!("Usage: {} FILE [options]", program);
    print!("{}", opts.usage(&brief));
}

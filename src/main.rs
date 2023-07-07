use std::collections::HashSet;
use std::fs::File;
use std::io::{Write, LineWriter};
use std::path::Path;

use itertools::Itertools;
use lance_creator::model::{Force, Model, ModelForce};
use rand::Rng;
use sorted_vec::SortedVec;

const MAX_ATTEMPTS: u16 = 5000;

fn main() {
    let mut rng = rand::thread_rng();

    let file = File::open("is.json").unwrap();
    let loaded_models: Vec<Model> = serde_json::from_reader(file).unwrap();

    // TODO: comand line params instead of fixed
    // TODO: hard limit of 6 because the time gets to be ages
    let force_size = 6;
    let min_bv = 5900;
    let max_bv = 6000;

    let mut model_forces: HashSet<ModelForce> = HashSet::new();
    let mut attempts = 0;
    while model_forces.len() < 5 && attempts < MAX_ATTEMPTS {
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

        if force.min_bv() < max_bv || force.max_bv() > min_bv {
            model_forces.insert(force);
        }

        attempts += 1;
    }

    let path = Path::new("forces.csv");
    let output_file = match File::create(&path) {
        Ok(file) => file,
        Err(e) => panic!("Couldn't create {}: {}", path.display(), e),
    };

    let mut output_file = LineWriter::new(output_file);

    // TODO: output total time taken
    // TODO: some sort of "working on it" output so we don't think we're frozen
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
                force.mechs.push(mech.clone().clone());
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

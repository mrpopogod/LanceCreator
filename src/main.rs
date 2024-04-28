use std::collections::{HashMap, HashSet};
use std::env::args;
use std::fs::File;
use std::io::{LineWriter, Write};
use std::path::Path;
use std::process::exit;
use std::time::Instant;

use getopts::{Options, Fail};
use itertools::Itertools;
use lance_creator::model::{Force, Model, ModelForce, Params};
use rand::Rng;
use sorted_vec::SortedVec;
use spinners::{Spinner, Spinners};

const MAX_ATTEMPTS: u16 = 5000;

fn main() {
    let args: Vec<String> = args().collect();
    let program = args[0].clone();
    let program = Path::new(&program);
    let program = program.file_name().unwrap().to_str().unwrap();

    let opts = get_opts();

    let matches = match opts.parse(&args[1..]) {
        Ok(m) => m,
        Err(e) => {
            match e {
                Fail::ArgumentMissing(a) => println!("Argument missing for option: {}", a),
                Fail::UnrecognizedOption(a) => println!("Unrecognized option: {}", a),
                Fail::OptionMissing(a) => println!("Option missing: {}", a),
                Fail::OptionDuplicated(a) => println!("Option specified multiple times: {}", a),
                Fail::UnexpectedArgument(a) => println!("Unexpected argument for option: {}", a),
            }
            return terminate(&program, &opts, 0);
        },
    };

    if matches.opt_present("h") {
        return terminate(&program, &opts, 0);
    }

    let (min_bv, max_bv, force_size, num_forces, skill) = match validate_opts(&matches) {
        Ok(value) => value.into(),
        Err(_) => return terminate(&program, &opts, 1),
    };

    let required_mech = matches.opt_str("r");

    let filenames = matches.opt_strs("f");
    let mut loaded_models: Vec<Model> = Vec::new();
    for filename in filenames {
        let file = match File::open(&filename) {
            Ok(f) => f,
            Err(e) => {
                println!("Error opening {}: {}", filename, e);
                return terminate(&program, &opts, 1);
            },
        };

        let mut file_models: Vec<Model> = match serde_json::from_reader(file) {
            Ok(m) => m,
            Err(e) => {
                println!("Error reading {}: {}", filename, e);
                return terminate(&program, &opts, 1);
            },
        };

        loaded_models.append(&mut file_models);
    }
 
    if loaded_models.is_empty() {
        println!("No files provided or files provided were empty");
        return terminate(&program, &opts, 1);
    }

    let model_forces: HashSet<ModelForce> = match generate_model_forces(num_forces, loaded_models, force_size, min_bv, max_bv, required_mech, skill) {
        Ok(m) => m,
        Err(e) => {
            println!("Failed to generate model forces: {}", e);
            return terminate(&program, &opts, 1);
        },
    };

    if model_forces.len() < num_forces {
        println!(
            "Only found {} forces within the BV range",
            model_forces.len()
        );
    }

    let path = Path::new("forces.csv");
    let mut output_file = match File::create(path) {
        Ok(file) => file,
        Err(e) => panic!("Couldn't create {}: {}", path.display(), e),
    };

    let now = Instant::now();
    let mut spinner = Spinner::new(Spinners::Line, "Generating force variants".into());
    generate_variants_and_write_out(&mut output_file, model_forces, min_bv, max_bv);

    spinner.stop_with_message(format!("Created forces in {}s", now.elapsed().as_secs()));
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

fn generate_model_forces(num_forces: usize, loaded_models: Vec<Model>, force_size: usize, min_bv: u32, max_bv: u32, required_mech: Option<String>, skill: u8) -> Result<HashSet<ModelForce>, &'static str> {
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
            if i == 0 && required_mech.is_some() {
                let required_mech_name = required_mech.as_ref().unwrap();
                match copied_models.iter().position(|m| m.name == *required_mech_name) {
                    Some(pos) => {
                        let model = copied_models.get_mut(pos).unwrap();
                        if model.count > 1 {
                            let mut cloned_model = model.clone();
                            cloned_model.adjust_bv(skill_mul);
                            force.models.insert(cloned_model);
                            model.count -= 1;
                        } else {
                            let mut model = copied_models.swap_remove(pos);
                            model.adjust_bv(skill_mul);
                            force.models.insert(model.clone());
                        }
                        
                        continue;
                    },
                    None => return Err("Required mech not in input file"),
                }
            }

            let n = rng.gen_range(0..copied_models.len());
            let model = copied_models.get_mut(n).unwrap();
            if model.count > 1 {
                let mut cloned_model = model.clone();
                cloned_model.adjust_bv(skill_mul);
                force.models.insert(cloned_model);
                model.count -= 1;
            } else {
                let mut model = copied_models.swap_remove(i);
                model.adjust_bv(skill_mul);
                force.models.insert(model);
            }
        }

        if force.max_bv() < min_bv || force.min_bv() > max_bv {
            continue;
        }

        println!("Created valid force");
        model_forces.insert(force);
    }

    return Ok(model_forces)
}

fn validate_opts(matches: &getopts::Matches) -> Result<Params, ()> {
    let min_bv = match matches.opt_get_default("m", 4900) {
        Ok(m) => m,
        Err(e) => {
            println!("Invalid min bv: {}", e);
            return Err(())
        },
    };
    let max_bv = match matches.opt_get_default("b", 5000) {
        Ok(b) => b,
        Err(e) => {
            println!("Invalid max bv: {}", e);
            return Err(())
        },
    };
    let force_size = match matches.opt_get_default("s", 4) {
        Ok(s) if s > 6 => return Err(()),
        Ok(s) => s,
        Err(e) => {
            println!("Invalid force size: {}", e);
            return Err(())
        },
    };
    let num_forces = match matches.opt_get_default("n", 3) {
        Ok(f) if f > 5 => return Err(()),
        Ok(f) => f,
        Err(e) => {
            println!("Invalid num forces: {}", e);
            return Err(())
        },
    };
    if min_bv > max_bv {
        println!("Min bv cannot be greater than max bv");
        return Err(())
    }
    let skill = match matches.opt_get_default("k", 45) {
        Ok(k) => k,
        Err(e) => {
            println!("Invalid skill: {}", e);
            return Err(())
        }
    };
    if !get_skill_map().contains_key(&skill) {
        println!("Gunnery and piloting must between 0 and 8");
        return Err(())
    }
    Ok(Params{ min_bv: min_bv, max_bv: max_bv, force_size: force_size, num_forces: num_forces, skill })
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

fn get_opts() -> Options {
    let mut opts = Options::new();
    opts.optflag("h", "help", "This help text");
    opts.optmulti("f", "file", "Input file(s) of mechs", "");
    opts.optopt("b", "bv", "Target BV (default: 5000)", "");
    opts.optopt("m", "minBv", "Minimum acceptable BV (default: 4900)", "");
    opts.optopt(
        "s",
        "size",
        "How many units in a force (default: 4, max: 6",
        "",
    );
    opts.optopt(
        "n",
        "numForces",
        "How many distinct sets of model to create options for (default: 3, max: 5)",
        "",
    );
    opts.optflag("h", "help", "Print this help menu");
    opts.optopt(
        "r",
        "requiredMech",
        "Base chassis that needs to be in the results (default: none)",
        "",
    );
    opts.optopt(
        "k",
        "skill",
        "Pilot skill to bump all mechs to, in the form of PG (e.g. 4/5 is 45)",
        ""
    );
    opts
}

fn terminate(program: &str, opts: &Options, code: i32) {
    print_usage(program, opts);
    exit(code);
}

fn print_usage(program: &str, opts: &Options) {
    let brief = format!("Usage: {} -f FILE [options]", program);
    print!("{}", opts.usage(&brief));
}

#[cfg(test)]
mod tests {
    use crate::*;
    use lance_creator::model::Mech;
    use rstest::{rstest, fixture};

    #[rstest]
    #[case::no_args(vec![], false, false)]
    #[case::no_optional_args(vec!["-f", "foo"], true, true)]
    #[case::only_optional_args(vec!["-b", "5000", "-m" , "3000", "-s", "3", "-n", "5"], false, false)]
    #[case::valid_optional_args(vec!["-f", "foo", "b", "5000", "-m", "3000", "-s", "3", "-n", "5"], true, true)]
    #[case::invalid_force_size_args(vec!["-f", "foo", "-s", "7", "-n", "5"], true, false)]
    #[case::invalid_num_forces_args(vec!["-f", "foo", "-s", "3", "-n", "6"], true, false)]
    #[case::invalid_min_bv(vec!["-f", "foo", "-m", "6000"], true, false)]
    #[case::invalid_max_bv(vec!["-f", "foo", "-b", "4000"], true, false)]
    #[case::invalid_bv(vec!["-f", "foo", "-m", "6000", "-b", "4000"], true, false)]
    fn test_valid_options(#[case] args: Vec<&str>, #[case] is_parseable: bool, #[case] is_valid: bool) {
        let opts = get_opts();
        let matches = match opts.parse(args) {
            Ok(m) => {
                assert!(is_parseable);
                m
            }
            Err(_) => {
                assert!(!is_parseable);
                return;
            }
        };

        if !is_parseable {
            return
        }

        match validate_opts(&matches) {
            Ok(_) => {
                assert!(is_valid);
            }
            Err(_) => {
                assert!(!is_valid);
            }
        }
    }

    #[fixture]
    fn model_forces() -> HashSet<ModelForce> {
        let first_mech_first_variant = Mech{
            name: String::from("a"),
            variant: String::from("1"),
            bv: 100
        };
        let first_mech_second_variant = Mech{
            name: String::from("a"),
            variant: String::from("2"),
            bv: 5000
        };
        let first_mech: Model = Model {
            name: String::from("a"),
            count: 1,
            variants: vec![first_mech_first_variant, first_mech_second_variant]
        };

        let second_mech_first_variant = Mech{
            name: String::from("b"),
            variant: String::from("1"),
            bv: 10
        };
        let second_mech_second_variant = Mech{
            name: String::from("b"),
            variant: String::from("2"),
            bv: 500
        };
        let second_mech: Model = Model {
            name: String::from("b"),
            count: 1,
            variants: vec![second_mech_first_variant, second_mech_second_variant]
        };


        let first_mech = ModelForce{
            models: SortedVec::from_unsorted(vec![first_mech, second_mech])
        };

        let mut ret = HashSet::new();
        ret.insert(first_mech);
        ret
    }

    #[rstest]
    #[case::bv_too_low(0, 10, "")]
    #[case::a1_and_b1(0, 110, "a 1,b 1,110\n")]
    #[case::a1_and_b(0, 600, "a 1,b 1,110\na 1,b 2,600\n")]
    #[case::a2_and_b1(5000, 5100, "a 2,b 1,5010\n")]
    #[case::a2_and_b(5000, 10000, "a 2,b 1,5010\na 2,b 2,5500\n")]
    #[case::a_and_b(0, 10000, "a 1,b 1,110\na 1,b 2,600\na 2,b 1,5010\na 2,b 2,5500\n")]
    #[case::bv_too_high(10000, 50000, "")]
    fn test_generate_variants(model_forces: HashSet<ModelForce>, #[case] min_bv: u32, #[case] max_bv: u32, #[case] expected: String) {
        let mut writer: Vec<u8> = Vec::new();
        generate_variants_and_write_out(&mut writer, model_forces, min_bv, max_bv);
        let result = String::from_utf8(writer).unwrap();
        assert_eq!(expected, result);
    }
}
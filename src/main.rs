use std::fs::File;

use lance_creator::model::{Model, Force};
use rand::Rng;
use sorted_vec::SortedVec;

fn main() {
    println!("Hello, world!");
    let file = File::open("is.json").unwrap();
    let models: Vec<Model> = serde_json::from_reader(file).unwrap();
    let mut rng = rand::thread_rng();
    let n = rng.gen_range(0..models.len());
    let model = models.get(n).unwrap();
    let mech = model.variants.get(0).unwrap().clone();
    let mechs = vec![mech];
    let force = Force{ mechs: SortedVec::from_unsorted(mechs) };
    println!("{}", force.to_csv());
}

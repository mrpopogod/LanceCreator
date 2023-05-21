use std::fs::File;

use lance_creator::model::Model;
use rand::Rng;

fn main() {
    println!("Hello, world!");
    let file = File::open("is.json").unwrap();
    let models: Vec<Model> = serde_json::from_reader(file).unwrap();
    let mut rng = rand::thread_rng();
    let n = rng.gen_range(0..models.len());
    
}

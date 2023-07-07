use std::fmt::{Formatter, Display};

use serde::Deserialize;
use sorted_vec::SortedVec;

#[derive(Clone, Deserialize, PartialEq, PartialOrd, Eq, Ord)]
pub struct Mech {
    #[serde(skip)]
    pub name: String,
    pub variant: String,
    pub bv: u32,
}

#[derive(Deserialize)]
struct ModelShadow {
    name: String,
    count: u8,
    variants: Vec<Mech>
}

pub struct ModelValidationError;

impl Display for ModelValidationError {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "somehow got error when shadow should have just populated")
    }
}

#[derive(Deserialize, PartialEq, PartialOrd)]
#[serde(try_from = "ModelShadow")]
pub struct Model {
    pub name: String,
    pub count: u8,
    pub variants: Vec<Mech>,
}

impl TryFrom<ModelShadow> for Model {
    type Error = ModelValidationError;

    fn try_from(value: ModelShadow) -> Result<Self, Self::Error> {
        let mut model = Model {
            name: value.name,
            count: value.count,
            variants: value.variants
        };

        for mech in model.variants.iter_mut() {
            mech.name = model.name.clone();
        }

        Ok(model)
    }
}

impl Model {
    pub fn min_bv(&self) -> u32 {
        let mut min = u32::MAX;
        for mech in self.variants.iter() {
            if mech.bv < min {
                min = mech.bv;
            }
        }

        min
    }

    pub fn max_bv(&self) -> u32 {
        let mut max = u32::MIN;
        for mech in self.variants.iter() {
            if mech.bv > max {
                max = mech.bv;
            }
        }

        max
    }
}

#[derive(PartialEq, PartialOrd)]
pub struct Force {
    pub mechs: SortedVec<Mech>
}

impl Force {
    pub fn to_csv(&self) -> String {
        let mut retval = String::new();
        let mut bv = 0;
        for mech in self.mechs.iter() {
            bv += mech.bv;
            retval += &mech.name;
            retval += " ";
            retval += &mech.variant;
            retval += ",";
        }

        retval += &bv.to_string();
        retval
    }
}
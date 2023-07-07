use std::fmt::{Formatter, Display};

use serde::Deserialize;
use sorted_vec::SortedVec;

#[derive(Clone, Deserialize, PartialEq, PartialOrd, Eq, Hash)]
pub struct Mech {
    #[serde(skip)]
    pub name: String,
    pub variant: String,
    pub bv: u32,
}

impl Ord for Mech {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        let name_cmp = self.name.cmp(&other.name);
        match name_cmp {
            std::cmp::Ordering::Equal => self.variant.cmp(&other.variant),
            _ => name_cmp,
        }
    }
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

#[derive(Clone, Deserialize, PartialEq, PartialOrd, Eq, Hash)]
#[serde(try_from = "ModelShadow")]
pub struct Model {
    pub name: String,
    pub count: u8,
    pub variants: Vec<Mech>,
}

impl Ord for Model {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        self.name.cmp(&other.name)
    }
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

#[derive(PartialEq, PartialOrd, Eq, Hash)]
pub struct ModelForce {
    pub models: SortedVec<Model>
}

impl ModelForce {
    pub fn min_bv(&self) -> u32 {
        let mut min = 0;
        for model in self.models.iter() {
            min += model.min_bv();
        }

        min
    }

    pub fn max_bv(&self) -> u32 {
        let mut max = 0;
        for model in self.models.iter() {
            max += model.max_bv();
        }

        max
    }
}

#[derive(PartialEq, PartialOrd)]
pub struct Force {
    pub mechs: SortedVec<Mech>
}

impl Force {
    pub fn bv(&self) -> u32 {
        let mut bv = 0;
        for mech in self.mechs.iter() {
            bv += mech.bv;
        }

        bv
    }

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
        retval += "\n";
        retval
    }
}
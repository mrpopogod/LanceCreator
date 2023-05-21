use serde::Deserialize;
use sorted_vec::SortedVec;

#[derive(Deserialize, PartialEq, PartialOrd, Eq, Ord)]
pub struct Mech {
    #[serde(skip)]
    pub name: String,
    pub variant: String,
    pub bv: u32,
}

#[derive(Deserialize, PartialEq, PartialOrd)]
pub struct Model {
    pub name: String,
    pub count: u8,
    pub variants: Vec<Mech>,
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
    pub mechs: SortedVec<Mech>,
    pub bv: u32,
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
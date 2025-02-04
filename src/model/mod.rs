use std::cmp::Ordering;
use std::collections::BTreeMap;
use std::fmt::{Display, Formatter};

use serde::Deserialize;
use sorted_vec::SortedVec;

#[derive(Copy, Clone, Deserialize, PartialEq, Eq, Hash, PartialOrd, Ord)]
pub enum Era {
    AgeOfWar,
    StarLeague,
    EarlySuccessionWar,
    LateSuccessionWarLosTech,
    LateSuccessionWarRenaissance,
    ClanInvasion,
    CivilWar,
    Jihad,
    EarlyRepublic,
    LateRepublic,
    DarkAge,
    IlClan,
}

#[derive(Copy, Clone, Deserialize, PartialEq, Eq, Hash, PartialOrd, Ord)]
pub enum Faction {
    CapellanConfederation,
    ComStar,
    DraconisCombine,
    FederatedCommonwealth,
    FederatedSuns,
    FreeRasalhagueRepublic,
    FreeWorldsLeague,
    FreeWorldsLeagueDuchyOfAndurien,
    FreeWorldsLeaugeDuchyOfTamarindAbbey,
    FreeWorldsLeageNonAlignedWorlds,
    FreeWorldsLeagueOrienteProtectorate,
    FreeWorldsLeagueRegulanFiefs,
    FreeWorldsLeagueRimCommonality,
    InnerSphereGeneral,
    LyranAlliance,
    LyranCommonwealth,
    RepublicOfTheSphere,
    StIvesCompact,
    TamarPact,
    TerranHegemony,
    VesperMarches,
    WordOfBlake,
    AlyinaMercantileLeague,
    ClanDiamondShark,
    ClanGhostBear,
    ClanHellsHorses,
    ClanJadeFalcon,
    ClanNovaCat,
    ClanProtectorate,
    ClanSeaFox,
    ClanSmokeJaguar,
    ClanSnowRaven,
    ClanWolf,
    ClanWolfInExile,
    ISClanGeneral,
    RasalhagueDominion,
    RavenAlliance,
    StarLeagueClanJadeFalcon,
    StarLeagueClanSmokeJaguar,
    StarLeagueClanWolf,
    WolfEmpire,
    ClanBloodSpirit,
    ClanBurrock,
    ClanCloudCobra,
    ClanCoyote,
    ClanFireMandrill,
    ClanGoliathScorpion,
    ClanIceHellion,
    ClanMongoose,
    ClanStarAdder,
    ClanSteelViper,
    ClanStoneLion,
    ClanWidowmaker,
    ClanWolverine,
    HWClanGeneral,
    Society,
    StarLeagueInExile,
    CalderonProtectorate,
    CircinusFederation,
    EscorpionImperio,
    FiltveltCoalition,
    FroncReaches,
    MagistracyOfCanopus,
    MarianHegemony,
    OutworldsAlliance,
    PeripheryGeneral,
    Pirates,
    RimWorldsRepublicHomeGuard,
    RimWorldsRepublicTerranCorps,
    ScorpionEmpire,
    TaurianConcordat,
    KellHounds,
    Mercenary,
    WolfsDragoons,
}

impl Faction {
    // TODO: this needs to take in era as well, since some Clans swap from HW to IS during CI
    pub fn get_general(&self) -> Faction {
        match self {
            Faction::CapellanConfederation => Faction::InnerSphereGeneral,
            Faction::ComStar => Faction::InnerSphereGeneral,
            Faction::DraconisCombine => Faction::InnerSphereGeneral,
            Faction::FederatedCommonwealth => Faction::InnerSphereGeneral,
            Faction::FederatedSuns => Faction::InnerSphereGeneral,
            Faction::FreeRasalhagueRepublic => Faction::InnerSphereGeneral,
            Faction::FreeWorldsLeague => Faction::InnerSphereGeneral,
            Faction::FreeWorldsLeagueDuchyOfAndurien => Faction::InnerSphereGeneral,
            Faction::FreeWorldsLeaugeDuchyOfTamarindAbbey => Faction::InnerSphereGeneral,
            Faction::FreeWorldsLeageNonAlignedWorlds => Faction::InnerSphereGeneral,
            Faction::FreeWorldsLeagueOrienteProtectorate => Faction::InnerSphereGeneral,
            Faction::FreeWorldsLeagueRegulanFiefs => Faction::InnerSphereGeneral,
            Faction::FreeWorldsLeagueRimCommonality => Faction::InnerSphereGeneral,
            Faction::InnerSphereGeneral => Faction::InnerSphereGeneral,
            Faction::LyranAlliance => Faction::InnerSphereGeneral,
            Faction::LyranCommonwealth => Faction::InnerSphereGeneral,
            Faction::RepublicOfTheSphere => Faction::InnerSphereGeneral,
            Faction::StIvesCompact => Faction::InnerSphereGeneral,
            Faction::TamarPact => Faction::InnerSphereGeneral,
            Faction::TerranHegemony => Faction::InnerSphereGeneral,
            Faction::VesperMarches => Faction::InnerSphereGeneral,
            Faction::WordOfBlake => Faction::InnerSphereGeneral,
            Faction::AlyinaMercantileLeague => Faction::ISClanGeneral,
            Faction::ClanDiamondShark => Faction::ISClanGeneral,
            Faction::ClanGhostBear => Faction::ISClanGeneral,
            Faction::ClanHellsHorses => Faction::ISClanGeneral,
            Faction::ClanJadeFalcon => Faction::ISClanGeneral,
            Faction::ClanNovaCat => Faction::ISClanGeneral,
            Faction::ClanProtectorate => Faction::ISClanGeneral,
            Faction::ClanSeaFox => Faction::ISClanGeneral,
            Faction::ClanSmokeJaguar => Faction::ISClanGeneral,
            Faction::ClanSnowRaven => Faction::ISClanGeneral,
            Faction::ClanWolf => Faction::ISClanGeneral,
            Faction::ClanWolfInExile => Faction::ISClanGeneral,
            Faction::ISClanGeneral => Faction::ISClanGeneral,
            Faction::RasalhagueDominion => Faction::ISClanGeneral,
            Faction::RavenAlliance => Faction::ISClanGeneral,
            Faction::StarLeagueClanJadeFalcon => Faction::ISClanGeneral,
            Faction::StarLeagueClanSmokeJaguar => Faction::ISClanGeneral,
            Faction::StarLeagueClanWolf => Faction::ISClanGeneral,
            Faction::WolfEmpire => Faction::ISClanGeneral,
            Faction::ClanBloodSpirit => Faction::HWClanGeneral,
            Faction::ClanBurrock => Faction::HWClanGeneral,
            Faction::ClanCloudCobra => Faction::HWClanGeneral,
            Faction::ClanCoyote => Faction::HWClanGeneral,
            Faction::ClanFireMandrill => Faction::HWClanGeneral,
            Faction::ClanGoliathScorpion => Faction::HWClanGeneral,
            Faction::ClanIceHellion => Faction::HWClanGeneral,
            Faction::ClanMongoose => Faction::HWClanGeneral,
            Faction::ClanStarAdder => Faction::HWClanGeneral,
            Faction::ClanSteelViper => Faction::HWClanGeneral,
            Faction::ClanStoneLion => Faction::HWClanGeneral,
            Faction::ClanWidowmaker => Faction::HWClanGeneral,
            Faction::ClanWolverine => Faction::HWClanGeneral,
            Faction::HWClanGeneral => Faction::HWClanGeneral,
            Faction::Society => Faction::HWClanGeneral,
            Faction::StarLeagueInExile => Faction::HWClanGeneral,
            Faction::CalderonProtectorate => Faction::PeripheryGeneral,
            Faction::CircinusFederation => Faction::PeripheryGeneral,
            Faction::EscorpionImperio => Faction::PeripheryGeneral,
            Faction::FiltveltCoalition => Faction::PeripheryGeneral,
            Faction::FroncReaches => Faction::PeripheryGeneral,
            Faction::MagistracyOfCanopus => Faction::PeripheryGeneral,
            Faction::MarianHegemony => Faction::PeripheryGeneral,
            Faction::OutworldsAlliance => Faction::PeripheryGeneral,
            Faction::PeripheryGeneral => Faction::PeripheryGeneral,
            Faction::Pirates => Faction::PeripheryGeneral,
            Faction::RimWorldsRepublicHomeGuard => Faction::PeripheryGeneral,
            Faction::RimWorldsRepublicTerranCorps => Faction::PeripheryGeneral,
            Faction::ScorpionEmpire => Faction::PeripheryGeneral,
            Faction::TaurianConcordat => Faction::PeripheryGeneral,
            Faction::KellHounds => Faction::Mercenary,
            Faction::Mercenary => Faction::Mercenary,
            Faction::WolfsDragoons => Faction::Mercenary,
        }
    }
}

#[derive(Clone, Deserialize, PartialEq, Eq, Hash)]
pub struct Mech {
    #[serde(skip)]
    pub name: String,
    pub variant: String,
    pub bv: u32,
    pub availability: BTreeMap<Era, Vec<Faction>>,
}

impl PartialOrd for Mech {
    fn partial_cmp(&self, other: &Mech) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Ord for Mech {
    fn cmp(&self, other: &Self) -> Ordering {
        let name_cmp = self.name.cmp(&other.name);
        match name_cmp {
            Ordering::Equal => self.variant.cmp(&other.variant),
            _ => name_cmp,
        }
    }
}

#[derive(Deserialize)]
struct ModelShadow {
    name: String,
    count: u8,
    variants: Vec<Mech>,
}

pub struct ModelValidationError;

impl Display for ModelValidationError {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(
            f,
            "somehow got error when shadow should have just populated"
        )
    }
}

#[derive(Clone, Deserialize, PartialEq, Eq, Hash)]
#[serde(try_from = "ModelShadow")]
pub struct Model {
    pub name: String,
    pub count: u8,
    pub variants: Vec<Mech>,
}

impl PartialOrd for Model {
    fn partial_cmp(&self, other: &Model) -> Option<Ordering> {
        Some(self.cmp(other))
    }
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
            variants: value.variants,
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

    pub fn adjust_bv(&mut self, mul: f64) {
        for mech in self.variants.iter_mut() {
            mech.bv = (mech.bv as f64 * mul).round() as u32;
        }
    }

    pub fn trim_availability(&mut self, era: Era, faction: Faction) {
        self.variants.retain(|m| match m.availability.get(&era) {
            Some(f) => {
                if f.contains(&faction) {
                    true
                } else {
                    f.contains(&faction.get_general())
                }
            }
            None => false,
        });
    }
}

#[derive(PartialEq, PartialOrd, Eq, Hash)]
pub struct ModelForce {
    pub models: SortedVec<Model>,
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
    pub mechs: SortedVec<Mech>,
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

pub struct Params {
    pub min_bv: u32,
    pub max_bv: u32,
    pub force_size: usize,
    pub num_forces: usize,
    pub skill: u8,
}

impl From<Params> for (u32, u32, usize, usize, u8) {
    fn from(p: Params) -> (u32, u32, usize, usize, u8) {
        let Params {
            min_bv,
            max_bv,
            force_size,
            num_forces,
            skill,
        } = p;
        (min_bv, max_bv, force_size, num_forces, skill)
    }
}

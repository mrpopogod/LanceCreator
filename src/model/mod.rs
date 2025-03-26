use std::cmp::Ordering;
use std::collections::BTreeMap;
use std::fmt::{Display, Formatter};
use std::str::FromStr;

use serde::Deserialize;
use sorted_vec::SortedVec;

#[derive(Copy, Clone, Deserialize, PartialEq, Eq, Hash, PartialOrd, Ord, Debug)]
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

impl FromStr for Era {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "AgeOfWar" => Ok(Era::AgeOfWar),
            "StarLeague" => Ok(Era::StarLeague),
            "EarlySuccessionWar" => Ok(Era::EarlySuccessionWar),
            "LateSuccessionWarLosTech" => Ok(Era::LateSuccessionWarLosTech),
            "LateSuccessionWarRenaissance" => Ok(Era::LateSuccessionWarRenaissance),
            "ClanInvasion" => Ok(Era::ClanInvasion),
            "CivilWar" => Ok(Era::CivilWar),
            "Jihad" => Ok(Era::Jihad),
            "EarlyRepublic" => Ok(Era::EarlyRepublic),
            "LateRepublic" => Ok(Era::LateRepublic),
            "DarkAge" => Ok(Era::DarkAge),
            "IlClan" => Ok(Era::IlClan),
            _ => Err(String::from(s)),
        }
    }
}

// TODO: add Star League factions
#[derive(Copy, Clone, Deserialize, PartialEq, Eq, Hash, PartialOrd, Ord, Debug)]
pub enum Faction {
    CapellanConfederation,
    ComStar,
    DraconisCombine,
    FederatedCommonwealth,
    FederatedSuns,
    FreeRasalhagueRepublic,
    FreeWorldsLeague,
    FreeWorldsLeagueDuchyOfAndurien,
    FreeWorldsLeagueDuchyOfTamarindAbbey,
    FreeWorldsLeagueMarikStewartCommonwealth,
    FreeWorldsLeagueNonAlignedWorlds,
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
    StarLeagueSecond,
    StarLeagueGeneral,
    StarLeagueRegular,
    StarLeagueRoyal
}

impl FromStr for Faction {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "CapellanConfederation" => Ok(Faction::CapellanConfederation),
            "ComStar" => Ok(Faction::ComStar),
            "DraconisCombine" => Ok(Faction::DraconisCombine),
            "FederatedCommonwealth" => Ok(Faction::FederatedCommonwealth),
            "FederatedSuns" => Ok(Faction::FederatedSuns),
            "FreeRasalhagueRepublic" => Ok(Faction::FreeRasalhagueRepublic),
            "FreeWorldsLeague" => Ok(Faction::FreeWorldsLeague),
            "FreeWorldsLeagueDuchyOfAndurien" => Ok(Faction::FreeWorldsLeagueDuchyOfAndurien),
            "FreeWorldsLeagueDuchyOfTamarindAbbey" => {
                Ok(Faction::FreeWorldsLeagueDuchyOfTamarindAbbey)
            }
            "FreeWorldsLeagueMarikStewartCommonwealth" => {
                Ok(Faction::FreeWorldsLeagueMarikStewartCommonwealth)
            }
            "FreeWorldsLeagueNonAlignedWorlds" => Ok(Faction::FreeWorldsLeagueNonAlignedWorlds),
            "FreeWorldsLeagueOrienteProtectorate" => {
                Ok(Faction::FreeWorldsLeagueOrienteProtectorate)
            }
            "FreeWorldsLeagueRegulanFiefs" => Ok(Faction::FreeWorldsLeagueRegulanFiefs),
            "FreeWorldsLeagueRimCommonality" => Ok(Faction::FreeWorldsLeagueRimCommonality),
            "InnerSphereGeneral" => Ok(Faction::InnerSphereGeneral),
            "LyranAlliance" => Ok(Faction::LyranAlliance),
            "LyranCommonwealth" => Ok(Faction::LyranCommonwealth),
            "RepublicOfTheSphere" => Ok(Faction::RepublicOfTheSphere),
            "StIvesCompact" => Ok(Faction::StIvesCompact),
            "TamarPact" => Ok(Faction::TamarPact),
            "TerranHegemony" => Ok(Faction::TerranHegemony),
            "VesperMarches" => Ok(Faction::VesperMarches),
            "WordOfBlake" => Ok(Faction::WordOfBlake),
            "AlyinaMercantileLeague" => Ok(Faction::AlyinaMercantileLeague),
            "ClanDiamondShark" => Ok(Faction::ClanDiamondShark),
            "ClanGhostBear" => Ok(Faction::ClanGhostBear),
            "ClanHellsHorses" => Ok(Faction::ClanHellsHorses),
            "ClanJadeFalcon" => Ok(Faction::ClanJadeFalcon),
            "ClanNovaCat" => Ok(Faction::ClanNovaCat),
            "ClanProtectorate" => Ok(Faction::ClanProtectorate),
            "ClanSeaFox" => Ok(Faction::ClanSeaFox),
            "ClanSmokeJaguar" => Ok(Faction::ClanSmokeJaguar),
            "ClanSnowRaven" => Ok(Faction::ClanSnowRaven),
            "ClanWolf" => Ok(Faction::ClanWolf),
            "ClanWolfInExile" => Ok(Faction::ClanWolfInExile),
            "ISClanGeneral" => Ok(Faction::ISClanGeneral),
            "RasalhagueDominion" => Ok(Faction::RasalhagueDominion),
            "RavenAlliance" => Ok(Faction::RavenAlliance),
            "StarLeagueClanJadeFalcon" => Ok(Faction::StarLeagueClanJadeFalcon),
            "StarLeagueClanSmokeJaguar" => Ok(Faction::StarLeagueClanSmokeJaguar),
            "StarLeagueClanWolf" => Ok(Faction::StarLeagueClanWolf),
            "WolfEmpire" => Ok(Faction::WolfEmpire),
            "ClanBloodSpirit" => Ok(Faction::ClanBloodSpirit),
            "ClanBurrock" => Ok(Faction::ClanBurrock),
            "ClanCloudCobra" => Ok(Faction::ClanCloudCobra),
            "ClanCoyote" => Ok(Faction::ClanCoyote),
            "ClanFireMandrill" => Ok(Faction::ClanFireMandrill),
            "ClanGoliathScorpion" => Ok(Faction::ClanGoliathScorpion),
            "ClanIceHellion" => Ok(Faction::ClanIceHellion),
            "ClanMongoose" => Ok(Faction::ClanMongoose),
            "ClanStarAdder" => Ok(Faction::ClanStarAdder),
            "ClanSteelViper" => Ok(Faction::ClanSteelViper),
            "ClanStoneLion" => Ok(Faction::ClanStoneLion),
            "ClanWidowmaker" => Ok(Faction::ClanWidowmaker),
            "ClanWolverine" => Ok(Faction::ClanWolverine),
            "HWClanGeneral" => Ok(Faction::HWClanGeneral),
            "Society" => Ok(Faction::Society),
            "StarLeagueInExile" => Ok(Faction::StarLeagueInExile),
            "CalderonProtectorate" => Ok(Faction::CalderonProtectorate),
            "CircinusFederation" => Ok(Faction::CircinusFederation),
            "EscorpionImperio" => Ok(Faction::EscorpionImperio),
            "FiltveltCoalition" => Ok(Faction::FiltveltCoalition),
            "FroncReaches" => Ok(Faction::FroncReaches),
            "MagistracyOfCanopus" => Ok(Faction::MagistracyOfCanopus),
            "MarianHegemony" => Ok(Faction::MarianHegemony),
            "OutworldsAlliance" => Ok(Faction::OutworldsAlliance),
            "PeripheryGeneral" => Ok(Faction::PeripheryGeneral),
            "Pirates" => Ok(Faction::Pirates),
            "RimWorldsRepublicHomeGuard" => Ok(Faction::RimWorldsRepublicHomeGuard),
            "RimWorldsRepublicTerranCorps" => Ok(Faction::RimWorldsRepublicTerranCorps),
            "ScorpionEmpire" => Ok(Faction::ScorpionEmpire),
            "TaurianConcordat" => Ok(Faction::TaurianConcordat),
            "KellHounds" => Ok(Faction::KellHounds),
            "Mercenary" => Ok(Faction::Mercenary),
            "WolfsDragoons" => Ok(Faction::WolfsDragoons),
            "StarLeagueSecond" => Ok(Faction::StarLeagueSecond),
            "StarLeagueGeneral" => Ok(Faction::StarLeagueGeneral),
            "StarLeagueRegular" => Ok(Faction::StarLeagueRegular),
            "StarLeagueRoyal" => Ok(Faction::StarLeagueRoyal),
            _ => Err(String::from(s)),
        }
    }
}

impl Faction {
    pub fn get_general(&self, era: Era) -> Faction {
        match self {
            Faction::CapellanConfederation => Faction::InnerSphereGeneral,
            Faction::ComStar => match era {
                Era::AgeOfWar => Faction::InnerSphereGeneral,
                Era::StarLeague => Faction::InnerSphereGeneral,
                Era::EarlySuccessionWar => Faction::InnerSphereGeneral,
                Era::LateSuccessionWarLosTech => Faction::ComStar, // ComStar dips in and out of being its own faction
                Era::LateSuccessionWarRenaissance => Faction::ComStar,
                Era::ClanInvasion => Faction::InnerSphereGeneral,
                Era::CivilWar => Faction::InnerSphereGeneral,
                Era::Jihad => Faction::InnerSphereGeneral,
                Era::EarlyRepublic => Faction::InnerSphereGeneral,
                Era::LateRepublic => Faction::ComStar,
                Era::DarkAge => Faction::ComStar,
                Era::IlClan => Faction::ComStar,
            },
            Faction::DraconisCombine => Faction::InnerSphereGeneral,
            Faction::FederatedCommonwealth => Faction::InnerSphereGeneral,
            Faction::FederatedSuns => Faction::InnerSphereGeneral,
            Faction::FreeRasalhagueRepublic => Faction::InnerSphereGeneral,
            Faction::FreeWorldsLeague => Faction::InnerSphereGeneral,
            Faction::FreeWorldsLeagueDuchyOfAndurien => Faction::InnerSphereGeneral,
            Faction::FreeWorldsLeagueDuchyOfTamarindAbbey => Faction::InnerSphereGeneral,
            Faction::FreeWorldsLeagueMarikStewartCommonwealth => Faction::InnerSphereGeneral,
            Faction::FreeWorldsLeagueNonAlignedWorlds => Faction::InnerSphereGeneral,
            Faction::FreeWorldsLeagueOrienteProtectorate => Faction::InnerSphereGeneral,
            Faction::FreeWorldsLeagueRegulanFiefs => Faction::InnerSphereGeneral,
            Faction::FreeWorldsLeagueRimCommonality => Faction::InnerSphereGeneral,
            Faction::InnerSphereGeneral => Faction::InnerSphereGeneral,
            Faction::LyranAlliance => Faction::InnerSphereGeneral,
            Faction::LyranCommonwealth => Faction::InnerSphereGeneral,
            Faction::RepublicOfTheSphere => Faction::InnerSphereGeneral,
            Faction::StIvesCompact => Faction::InnerSphereGeneral,
            Faction::TamarPact => Faction::InnerSphereGeneral,
            Faction::TerranHegemony => Faction::StarLeagueGeneral,
            Faction::VesperMarches => Faction::InnerSphereGeneral,
            Faction::WordOfBlake => Faction::InnerSphereGeneral,
            Faction::AlyinaMercantileLeague => Faction::ISClanGeneral,
            Faction::ClanDiamondShark => match era {
                Era::AgeOfWar => Faction::HWClanGeneral,
                Era::StarLeague => Faction::HWClanGeneral,
                Era::EarlySuccessionWar => Faction::HWClanGeneral,
                Era::LateSuccessionWarLosTech => Faction::HWClanGeneral,
                Era::LateSuccessionWarRenaissance => Faction::HWClanGeneral,
                Era::ClanInvasion => Faction::ISClanGeneral,
                Era::CivilWar => Faction::ISClanGeneral,
                Era::Jihad => Faction::ISClanGeneral,
                Era::EarlyRepublic => Faction::ISClanGeneral,
                Era::LateRepublic => Faction::ISClanGeneral,
                Era::DarkAge => Faction::ISClanGeneral,
                Era::IlClan => Faction::ISClanGeneral,
            },
            Faction::ClanGhostBear => match era {
                Era::AgeOfWar => Faction::HWClanGeneral,
                Era::StarLeague => Faction::HWClanGeneral,
                Era::EarlySuccessionWar => Faction::HWClanGeneral,
                Era::LateSuccessionWarLosTech => Faction::HWClanGeneral,
                Era::LateSuccessionWarRenaissance => Faction::HWClanGeneral,
                Era::ClanInvasion => Faction::ISClanGeneral,
                Era::CivilWar => Faction::ISClanGeneral,
                Era::Jihad => Faction::ISClanGeneral,
                Era::EarlyRepublic => Faction::ISClanGeneral,
                Era::LateRepublic => Faction::ISClanGeneral,
                Era::DarkAge => Faction::ISClanGeneral,
                Era::IlClan => Faction::ISClanGeneral,
            },
            Faction::ClanHellsHorses => match era {
                Era::AgeOfWar => Faction::HWClanGeneral,
                Era::StarLeague => Faction::HWClanGeneral,
                Era::EarlySuccessionWar => Faction::HWClanGeneral,
                Era::LateSuccessionWarLosTech => Faction::HWClanGeneral,
                Era::LateSuccessionWarRenaissance => Faction::HWClanGeneral,
                Era::ClanInvasion => Faction::HWClanGeneral,
                Era::CivilWar => Faction::ISClanGeneral,
                Era::Jihad => Faction::ISClanGeneral,
                Era::EarlyRepublic => Faction::ISClanGeneral,
                Era::LateRepublic => Faction::ISClanGeneral,
                Era::DarkAge => Faction::ISClanGeneral,
                Era::IlClan => Faction::ISClanGeneral,
            },
            Faction::ClanJadeFalcon => match era {
                Era::AgeOfWar => Faction::HWClanGeneral,
                Era::StarLeague => Faction::HWClanGeneral,
                Era::EarlySuccessionWar => Faction::HWClanGeneral,
                Era::LateSuccessionWarLosTech => Faction::HWClanGeneral,
                Era::LateSuccessionWarRenaissance => Faction::HWClanGeneral,
                Era::ClanInvasion => Faction::ISClanGeneral,
                Era::CivilWar => Faction::ISClanGeneral,
                Era::Jihad => Faction::ISClanGeneral,
                Era::EarlyRepublic => Faction::ISClanGeneral,
                Era::LateRepublic => Faction::ISClanGeneral,
                Era::DarkAge => Faction::ISClanGeneral,
                Era::IlClan => Faction::ISClanGeneral,
            },
            Faction::ClanNovaCat => match era {
                Era::AgeOfWar => Faction::HWClanGeneral,
                Era::StarLeague => Faction::HWClanGeneral,
                Era::EarlySuccessionWar => Faction::HWClanGeneral,
                Era::LateSuccessionWarLosTech => Faction::HWClanGeneral,
                Era::LateSuccessionWarRenaissance => Faction::HWClanGeneral,
                Era::ClanInvasion => Faction::ISClanGeneral,
                Era::CivilWar => Faction::ISClanGeneral,
                Era::Jihad => Faction::ISClanGeneral,
                Era::EarlyRepublic => Faction::ISClanGeneral,
                Era::LateRepublic => Faction::ISClanGeneral,
                Era::DarkAge => Faction::ISClanGeneral,
                Era::IlClan => Faction::ISClanGeneral,
            },
            Faction::ClanProtectorate => Faction::ISClanGeneral,
            Faction::ClanSeaFox => match era {
                Era::AgeOfWar => Faction::HWClanGeneral,
                Era::StarLeague => Faction::HWClanGeneral,
                Era::EarlySuccessionWar => Faction::HWClanGeneral,
                Era::LateSuccessionWarLosTech => Faction::HWClanGeneral,
                Era::LateSuccessionWarRenaissance => Faction::HWClanGeneral,
                Era::ClanInvasion => Faction::ISClanGeneral,
                Era::CivilWar => Faction::ISClanGeneral,
                Era::Jihad => Faction::ISClanGeneral,
                Era::EarlyRepublic => Faction::ISClanGeneral,
                Era::LateRepublic => Faction::ISClanGeneral,
                Era::DarkAge => Faction::ISClanGeneral,
                Era::IlClan => Faction::ISClanGeneral,
            },
            Faction::ClanSmokeJaguar => match era {
                Era::AgeOfWar => Faction::HWClanGeneral,
                Era::StarLeague => Faction::HWClanGeneral,
                Era::EarlySuccessionWar => Faction::HWClanGeneral,
                Era::LateSuccessionWarLosTech => Faction::HWClanGeneral,
                Era::LateSuccessionWarRenaissance => Faction::HWClanGeneral,
                Era::ClanInvasion => Faction::ISClanGeneral,
                Era::CivilWar => Faction::ISClanGeneral,
                Era::Jihad => Faction::ISClanGeneral,
                Era::EarlyRepublic => Faction::ISClanGeneral,
                Era::LateRepublic => Faction::ISClanGeneral,
                Era::DarkAge => Faction::ISClanGeneral,
                Era::IlClan => Faction::ISClanGeneral,
            },
            Faction::ClanSnowRaven => match era {
                Era::AgeOfWar => Faction::HWClanGeneral,
                Era::StarLeague => Faction::HWClanGeneral,
                Era::EarlySuccessionWar => Faction::HWClanGeneral,
                Era::LateSuccessionWarLosTech => Faction::HWClanGeneral,
                Era::LateSuccessionWarRenaissance => Faction::HWClanGeneral,
                Era::ClanInvasion => Faction::HWClanGeneral,
                Era::CivilWar => Faction::HWClanGeneral,
                Era::Jihad => Faction::ISClanGeneral,
                Era::EarlyRepublic => Faction::ISClanGeneral,
                Era::LateRepublic => Faction::ISClanGeneral,
                Era::DarkAge => Faction::ISClanGeneral,
                Era::IlClan => Faction::ISClanGeneral,
            },
            Faction::ClanWolf => match era {
                Era::AgeOfWar => Faction::HWClanGeneral,
                Era::StarLeague => Faction::HWClanGeneral,
                Era::EarlySuccessionWar => Faction::HWClanGeneral,
                Era::LateSuccessionWarLosTech => Faction::HWClanGeneral,
                Era::LateSuccessionWarRenaissance => Faction::HWClanGeneral,
                Era::ClanInvasion => Faction::ISClanGeneral,
                Era::CivilWar => Faction::ISClanGeneral,
                Era::Jihad => Faction::ISClanGeneral,
                Era::EarlyRepublic => Faction::ISClanGeneral,
                Era::LateRepublic => Faction::ISClanGeneral,
                Era::DarkAge => Faction::ISClanGeneral,
                Era::IlClan => Faction::ISClanGeneral,
            },
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
            Faction::ClanSteelViper => match era {
                Era::AgeOfWar => Faction::HWClanGeneral,
                Era::StarLeague => Faction::HWClanGeneral,
                Era::EarlySuccessionWar => Faction::HWClanGeneral,
                Era::LateSuccessionWarLosTech => Faction::HWClanGeneral,
                Era::LateSuccessionWarRenaissance => Faction::HWClanGeneral,
                Era::ClanInvasion => Faction::ISClanGeneral, // Steel Viper is an invader during the Invasion, then gets kicked back before Civil War
                Era::CivilWar => Faction::HWClanGeneral,
                Era::Jihad => Faction::HWClanGeneral,
                Era::EarlyRepublic => Faction::HWClanGeneral,
                Era::LateRepublic => Faction::HWClanGeneral,
                Era::DarkAge => Faction::HWClanGeneral,
                Era::IlClan => Faction::HWClanGeneral,
            },
            Faction::ClanStoneLion => Faction::HWClanGeneral,
            Faction::ClanWidowmaker => Faction::HWClanGeneral,
            Faction::ClanWolverine => Faction::HWClanGeneral,
            Faction::HWClanGeneral => Faction::HWClanGeneral,
            Faction::Society => Faction::Society,
            Faction::StarLeagueInExile => Faction::StarLeagueInExile,
            Faction::CalderonProtectorate => Faction::PeripheryGeneral,
            Faction::CircinusFederation => Faction::PeripheryGeneral,
            Faction::EscorpionImperio => Faction::EscorpionImperio,
            Faction::FiltveltCoalition => Faction::PeripheryGeneral,
            Faction::FroncReaches => Faction::PeripheryGeneral,
            Faction::MagistracyOfCanopus => Faction::PeripheryGeneral,
            Faction::MarianHegemony => Faction::PeripheryGeneral,
            Faction::OutworldsAlliance => Faction::PeripheryGeneral,
            Faction::PeripheryGeneral => Faction::PeripheryGeneral,
            Faction::Pirates => Faction::PeripheryGeneral,
            Faction::RimWorldsRepublicHomeGuard => Faction::PeripheryGeneral,
            Faction::RimWorldsRepublicTerranCorps => Faction::PeripheryGeneral,
            Faction::ScorpionEmpire => Faction::ScorpionEmpire,
            Faction::TaurianConcordat => Faction::PeripheryGeneral,
            Faction::KellHounds => Faction::KellHounds,
            Faction::Mercenary => Faction::Mercenary,
            Faction::WolfsDragoons => Faction::WolfsDragoons,
            Faction::StarLeagueSecond => Faction::InnerSphereGeneral,
            Faction::StarLeagueGeneral => Faction::StarLeagueGeneral,
            Faction::StarLeagueRegular => Faction::StarLeagueGeneral,
            Faction::StarLeagueRoyal => Faction::StarLeagueGeneral,
        }
    }
}

#[derive(Clone, Deserialize, PartialEq, Eq, Hash, Debug)]
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

#[derive(Clone, Deserialize, PartialEq, Eq, Hash, Debug)]
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
                    f.contains(&faction.get_general(era))
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
    pub era: Era,
    pub faction: Faction,
}

impl From<Params> for (u32, u32, usize, usize, u8, Era, Faction) {
    fn from(p: Params) -> (u32, u32, usize, usize, u8, Era, Faction) {
        let Params {
            min_bv,
            max_bv,
            force_size,
            num_forces,
            skill,
            era,
            faction,
        } = p;
        (min_bv, max_bv, force_size, num_forces, skill, era, faction)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    use rstest::{fixture, rstest};

    #[fixture]
    fn model() -> Model {
        let mut model = Model {
            name: String::from("Model"),
            count: 1,
            variants: Vec::new(),
        };
        let mut variant1 = Mech {
            name: String::from("Model"),
            variant: String::from("Variant1"),
            bv: 1,
            availability: BTreeMap::new(),
        };
        variant1
            .availability
            .insert(Era::ClanInvasion, vec![Faction::PeripheryGeneral]);
        let mut variant2 = Mech {
            name: String::from("Model"),
            variant: String::from("Variant2"),
            bv: 2,
            availability: BTreeMap::new(),
        };
        variant2
            .availability
            .insert(Era::IlClan, vec![Faction::MagistracyOfCanopus]);

        model.variants.push(variant1);
        model.variants.push(variant2);

        model
    }

    #[rstest]
    #[case::era_not_found(Era::AgeOfWar, Faction::InnerSphereGeneral, 0)]
    #[case::faction_not_found(Era::ClanInvasion, Faction::InnerSphereGeneral, 0)]
    #[case::faction_exact_match(Era::IlClan, Faction::MagistracyOfCanopus, 1)]
    #[case::faction_general(Era::ClanInvasion, Faction::MagistracyOfCanopus, 1)]
    fn validate_trim(
        mut model: Model,
        #[case] era: Era,
        #[case] faction: Faction,
        #[case] count: usize,
    ) {
        model.trim_availability(era, faction);
        assert_eq!(model.variants.len(), count);
    }
}

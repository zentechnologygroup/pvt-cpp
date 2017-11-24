
# include <metadata/pvt-tuner.H>

const DynSetTree<string> VectorDesc::valid_target_names =
  { "pb", "rs", "bob", "coa", "boa", "uod", "uob", "uoa", "uo" };

const DynSetTree<string> PvtData::var_pars =
  { "p", "pb", "uod", "bobp", "uobp", "t" };

const Unit * pb_unit = nullptr;
const Unit * rs_unit = nullptr;
const Unit * bo_unit = nullptr;
const Unit * co_unit = nullptr;
const Unit * uo_unit = nullptr;
const Unit * z_unit = nullptr;

void set_ttuner_units()
{
  pb_unit = &psig::get_instance();
  rs_unit = &SCF_STB::get_instance();
  bo_unit = &RB_STB::get_instance();
  co_unit = &psia_1::get_instance();
  uo_unit = &CP::get_instance();
  z_unit  = &Zfactor::get_instance();
}

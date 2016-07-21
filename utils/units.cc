
# include <units.H>

UnitItemTable Quantity::tbl;

void init_units()
{
  Quantity::tbl.register_item("Temperature", "T", "Measure of hot or cold");
}



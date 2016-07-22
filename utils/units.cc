
# include <units.H>

UnitItemTable physical_quantity_tbl;
UnitItemTable unit_tbl;

void init_units()
{
  physical_quantity_tbl.
    register_item("Temperature", "T", "Measure of hot or cold");
}



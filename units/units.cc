
# include <units.H>

UnitItemTable PhysicalQuantity::tbl;

UnitItemTable Unit::tbl;
DynSetTree<const Unit * const> Unit::unit_tbl;

UnitHashTbl __unit_name_name_tbl;
UnitHashTbl __unit_name_symbol_tbl;
UnitHashTbl __unit_symbol_name_tbl;
UnitHashTbl __unit_symbol_symbol_tbl;
CompoundUnitTbl __compound_unit_tbl;



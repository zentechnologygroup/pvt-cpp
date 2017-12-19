# ifndef SMOOTH_GRID_H
# define SMOOTH_GRID_H

# include <pvt-grid-compute.H>

struct SmoothGrid
{
  PvtGrid grid;

  SmoothGrid(PvtGrid && __grid) : grid(move(__grid))
  {
    grid.make_pb_table();
  }

  enum class ViolationTypes
  {
    AdjustUob_PbInUob, AdjustUob_PbInUoa, AdjustUoa_PbInUob, AdjustUoa_PbInUoa,
    AdjustBob_PbInUob, AdjustBob_PbInUoa, AdjustBoa_PbInUob, AdjustBoa_PbInUoa
  };

  
};



# endif

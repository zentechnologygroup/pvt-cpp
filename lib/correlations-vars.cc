
# include <pvt-correlations.H>

# include <json.hpp>

using json = nlohmann::json;

DynMapTree<string, const Correlation * const> Correlation::tbl;

CorrelationInstantiater __correlations; 

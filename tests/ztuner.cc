
# include <tclap/CmdLine.h>

# include <correlations/pvt-correlations.H>
# include <metadata/metadata-exceptions.H>

using PseudoPair = pair<const Correlation*, const Correlation*>;

# define Declare_Correlations_Set(NAME)			\
  DynList<const Correlation*> NAME##_correlations()	\
  {							\
    return Correlation::array().filter([] (auto p)			\
				       { return p->target_name() == #NAME; }); \
  }

Declare_Correlations_Set(yghc);
Declare_Correlations_Set(zfactor)

# define Declare_Pair_Correlations_Set(PNAME, TNAME, fct_name)	\
  DynList<PseudoPair> fct_name()					\
  {									\
    auto pcorrs =							\
      Correlation::array().filter([] (auto ptr)				\
				  {					\
				    return ptr->target_name() == #PNAME; \
				  });					\
    auto tcorrs =							\
      Correlation::array().filter([] (auto ptr)				\
				  {					\
				    return ptr->target_name() == #TNAME; \
				  });					\
									\
    in_place_sort(pcorrs, [] (auto ptr1, auto ptr2)			\
		  { return ptr1->name < ptr2->name; });			\
    in_place_sort(tcorrs, [] (auto ptr1, auto ptr2)			\
		  { return ptr1->name < ptr2->name; });			\
									\
    auto ret = zip(pcorrs, tcorrs);					\
    if (not ret.all([] (auto & p)					\
		    {							\
		      return p.first->author_name() == p.second->author_name();\
		    }))							\
      ZENTHROW(MismatchAuthors,						\
	       "Pseudo critical correlations does not have the same author"); \
									\
    return ret;								\
}

Declare_Pair_Correlations_Set(ppchc, tpchc, pseudo_correlations)
Declare_Pair_Correlations_Set(adjustedppcm, adjustedtpcm, adjusted_correlations)

int main(int argc, char *argv[])
{
  
  pseudo_correlations().for_each([] (auto & p)
    {
      cout << p.first->name << " " << p.second->name << endl;
    });

  adjusted_correlations().for_each([] (auto & p)
    {
      cout << p.first->name << " " << p.second->name << endl;
    });

}



# include <chrono>
# include <tclap/CmdLine.h>

# include <ah-date.H>
# include <tpl_dynSetTree.H>
# include <tpl_agraph.H>
# include <topological_sort.H>

using namespace std;
using namespace TCLAP;
using namespace Aleph;

struct Goal;

enum class GoalType { Platform, Product, ServiceMarket, Undefined };

struct Member
{
  string name = "NO-NAME";
  DynList<const Goal * const> responsible_goals;
  DynList<const Goal * const> member_goals;
};

struct Goal
{
  mutable string name = "NO-DESCRIPTION";
  const Member * responsible = nullptr;
  mutable string description;
  mutable DynList<string> collective_actions;
  mutable DynList<string> individual_actions;
  mutable time_t start_time, end_time;
  mutable DynList<const Member * const> members;
  mutable GoalType goal_type = GoalType::Undefined;

  Goal() {}

  Goal(const string & __name, const Member * resp, const string & desc,
       const GoalType & type)
    : name(__name), responsible(resp), description(desc), goal_type(type) {}

  void set_start_time(const size_t & dd, const size_t & mm, const size_t & yy)
  {
    start_time = to_time_t(dd, mm, yy);
  }

  void set_end_time(const size_t & dd, const size_t & mm, const size_t & yy)
  {
    end_time = to_time_t(dd, mm, yy);
    if (end_time <= start_time)
      throw domain_error("start time " + to_string(start_time) +
			 " is greater than end time " + to_string(end_time));
  }  
};

struct Plan : public Array_Graph<Graph_Anode<Goal>, Graph_Aarc<>>
{
  Array<Member> members;

  void register_member(const string & name)
  {
    if (members.exists([&name] (auto & m) { return m.name == name; }))
      
  }
};


int main(int argc, char *argv[])
{

  
}

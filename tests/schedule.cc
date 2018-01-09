
# include <chrono>
# include <fstream>
# include <tclap/CmdLine.h>

# include <ah-date.H>
# include <tpl_dynSetTree.H>
# include <tpl_agraph.H>
# include <topological_sort.H>

# include <zen-exceptions.H>

using namespace std;
using namespace TCLAP;
using namespace Aleph;

struct Goal;

enum class GoalType { Platform, Product, ServiceMarket, Undefined };

inline string goal_type_to_string(const GoalType & type)
{
  switch (type)
    {
    case GoalType::Platform: return "Platform";
    case GoalType::Product: return "Product";
    case GoalType::ServiceMarket: return "ServiceMarket";
    default: return "Undefined";
    }
}

inline GoalType string_to_goal_type(const string & type)
{
  if (type == "Platform")
    return GoalType::Platform;
  if (type == "Product")
    return GoalType::Product;
  if (type == "ServiceMarket")
    return GoalType::ServiceMarket;
  return GoalType::Undefined;
}

struct Member
{
  string name = "NO-NAME";
  DynList<const Goal * const> responsible_goals;
  DynList<const Goal * const> member_goals;
  Member() {}
  Member(const string & __name) : name(__name) {}
};

struct Goal
{
  static size_t count;
  mutable size_t id = 0;
  mutable string name = "NO-NAME";
  mutable string responsible = "NO-NAME";
  mutable string description;
  mutable DynList<string> collective_actions;
  mutable DynList<string> individual_actions;
  mutable DynList<string> expected_results;
  mutable time_t start_time, end_time;
  mutable DynList<string> members;
  mutable GoalType goal_type = GoalType::Undefined;

  static DynMapTree<size_t, Goal*> goal_tbl;

  Goal() {}

  Goal(const string & __name, const string & resp, const string & desc,
       const GoalType & type)
    : name(__name), responsible(resp), description(desc), goal_type(type)
  {
    id = count++;
  }

  Goal(const size_t & __id, const string & __name, const string & resp,
       const string & desc, const GoalType & type)
    : id(__id), name(__name), responsible(resp),
      description(desc), goal_type(type)
  {
    if (goal_tbl.contains(id))
      ZENTHROW(DuplicatedName, "id " + to_string(id) + " is already present");
  }

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

  void save(ostream & out) const
  {
    out << id << endl
	<< name << endl
	<< responsible << endl
	<< collective_actions.size() << endl;
    collective_actions.for_each([&out] (auto & s) { out << s << endl; });
    out << individual_actions.size() << endl;
    individual_actions.for_each([&out] (auto & s) { out << s << endl; });
    out << expected_results.size() << endl;
    expected_results.for_each([&out] (auto & s) { out << s << endl; });
    out << start_time << endl
	<< end_time << endl
	<< members.size() << endl;
    members.for_each([&out] (auto & name) { out << name << endl; });
    out << goal_type_to_string(goal_type) << endl;
  }

  Goal(istream & in)
  {
    if (not (in >> id >> name))
      ZENTHROW(InvalidRead, "cannot read goal id or name");
    if (not (in >> responsible))
      ZENTHROW(InvalidRead, "cannot read responsible name");
    if (not (in >> description))
      ZENTHROW(InvalidRead, "cannot read responsible name");

    // Read collective actions
    size_t n;
    if (not (in >> n))
      ZENTHROW(InvalidRead, "cannot read number of collective actions");
    string action;
    for (size_t i = 0; i < n; ++i)
      {
	if (not (in >> action))
	  ZENTHROW(InvalidRead, "cannot read collective action " +
		   to_string(i));
	collective_actions.append(action);
      }

    // read individual actions
    if (not (in >> n))
      ZENTHROW(InvalidRead, "cannot read number of individual actions");
    for (size_t i = 0; i < n; ++i)
      {
	if (not (in >> action))
	  ZENTHROW(InvalidRead, "cannot read individual action " + to_string(i));
	individual_actions.append(action);
      }

    // Read expected results
    if (not (in >> n))
      ZENTHROW(InvalidRead, "cannot read number of expected results");
    string result;
    for (size_t i = 0; i < n; ++i)
      {
	if (not (in >> result))
	  ZENTHROW(InvalidRead, "cannot read expected result " + to_string(i));
	expected_results.append(result);
      }

    if (not (in >> start_time))
      ZENTHROW(InvalidRead, "cannot read start time");
    if (not (in >> end_time))
      ZENTHROW(InvalidRead, "cannot read end time");

    // red members
    if (not (in >> n))
      ZENTHROW(InvalidRead, "cannot read number of members");
    string member;
    for (size_t i = 0; i < n; ++i)
      {
	if (not (in >> member))
	  ZENTHROW(InvalidRead, "cannot read member " + to_string(i));
	expected_results.append(member);
      }
  }
};

DynMapTree<size_t, Goal*> Goal::goal_tbl;

size_t Goal::count = 0;

struct Plan : public Array_Graph<Graph_Anode<Goal>, Graph_Aarc<>>
{
  DynMapTree<string, Member> members;
  DynMapTree<size_t, Plan::Node*> nodes_tbl;

  void register_member(const string & name)
  {
    if (members.contains(name))
      ZENTHROW(DuplicatedName, name + " is already registered");
    members.emplace(name, name);
  }

  size_t register_goal(const string & name, const string & responsible,
		     const string & desc, const GoalType & type)
  {
    auto ptr = members.search(responsible);
    if (ptr == nullptr)
      ZENTHROW(NameNotFound, "member " + responsible + " not found");
    Goal goal = { name, responsible, desc, type };
    Plan::Node * node_ptr = insert_node(move(goal));
    nodes_tbl.insert(node_ptr->get_info().id, node_ptr);

    return goal.id;
  }

  Plan::Node * search_node(const size_t & id) const
  {
    auto p = nodes_tbl.search(id);
    return p != nullptr ? p->second : nullptr;
  }

  void set_priority(const size_t src_id, const size_t tgt_id)
  {
    if (src_id == tgt_id)
      ZENTHROW(DuplicatedName, "src id " + to_string(src_id) + " is equal to " +
	       to_string(tgt_id));

    auto src_ptr = search_node(src_id);
    if (src_ptr == nullptr)
      ZENTHROW(NameNotFound, "src id " + to_string(src_id) + " not found");

    auto tgt_ptr = search_node(tgt_id);
    if (tgt_ptr == nullptr)
      ZENTHROW(NameNotFound, "tgt id " + to_string(tgt_id) + " not found");

    insert_arc(src_ptr, tgt_ptr);
  }
};




int main(int argc, char *argv[])
{

  
}

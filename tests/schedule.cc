
# include <chrono>
# include <fstream>
# include <tclap/CmdLine.h>

# include <ah-string-utils.H>
# include <parse-csv.H>
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

inline string to_string(const GoalType & type)
{
  switch (type)
    {
    case GoalType::Platform: return "Platform";
    case GoalType::Product: return "Product";
    case GoalType::ServiceMarket: return "ServiceMarket";
    default: return "Undefined";
    }
}

inline GoalType to_goal_type(const string & type)
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
  mutable size_t nhours = 0;
  mutable size_t prioriy = 0;

  static DynMapTree<size_t, Goal*> goal_tbl;

  Goal() {}

  Goal(const string & __name, const string & resp, const string & desc,
       const GoalType & type,
       const time_t & __start_time, const time_t & __end_time)
    : name(__name), responsible(resp), description(desc),
      start_time(__start_time), end_time(__end_time), goal_type(type)
  {
    id = count++;
  }

  Goal(const size_t & __id, const string & __name, const string & resp,
       const string & desc, const GoalType & type,
       const time_t & __start_time, const time_t & __end_time)
    : id(__id), name(__name), responsible(resp),
      description(desc), start_time(__start_time), end_time(__end_time),
      goal_type(type)
  {
    if (goal_tbl.contains(id))
      ZENTHROW(DuplicatedName, "id " + to_string(id) + " is already present");

    if (end_time <= start_time)
      throw domain_error("start time " + to_string(start_time) +
			 " is greater than end time " + to_string(end_time));
  }

  ostream & save(ostream & out) const
  {
    out << id << endl
	<< name << endl
	<< responsible << endl
	<< description << endl
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
    return out << to_string(goal_type);
  }

  friend ostream & operator << (ostream & out, const Goal & goal)
  {
    return goal.save(out);
  }

  Goal(istream & in)
  {
    string line;
    if (not getline(in, line))
      ZENTHROW(InvalidRead, "cannot read goal id");
    if (not is_size_t(line))
      ZENTHROW(InvalidRead, line + " is not an id");
    if (not getline(in, name))
      ZENTHROW(InvalidRead, "cannot read goal name");
    if (not getline(in, responsible))
      ZENTHROW(InvalidRead, "cannot read goal responsible");
    if (not getline(in, description))
      ZENTHROW(InvalidRead, "cannot read goal description");

    // Read collective actions
    if (not getline(in, line) or not is_size_t(line))
      ZENTHROW(InvalidRead, "cannot read number of collective actions");
    size_t n = atol(line);
    string action;
    for (size_t i = 0; i < n; ++i)
      {
	if (not getline(in, action))
	  ZENTHROW(InvalidRead, "cannot read collective action " +
		   to_string(i));
	collective_actions.append(action);
      }

    // read individual actions
    if (not getline(in, line) or not is_size_t(line))
      ZENTHROW(InvalidRead, "cannot read number of individual actions");
    n = atol(line);
    for (size_t i = 0; i < n; ++i)
      {
	if (not getline(in, action))
	  ZENTHROW(InvalidRead, "cannot read individual action " + to_string(i));
	individual_actions.append(action);
      }

    // Read expected results
    if (not getline(in, line) or not is_size_t(line))
      ZENTHROW(InvalidRead, "cannot read number of expected results");
    n = atol(line);
    string result;
    for (size_t i = 0; i < n; ++i)
      {
	if (not getline(in, result))
	  ZENTHROW(InvalidRead, "cannot read expected result " + to_string(i));
	expected_results.append(result);
      }

    string date;
    if (not getline(in, date))
      ZENTHROW(InvalidRead, "cannot read start time");
    start_time = atol(date);
    if (not getline(in, date))
      ZENTHROW(InvalidRead, "cannot read end time");
    end_time = atol(date);

    // red members
    if (not getline(in, line))
      ZENTHROW(InvalidRead, "cannot read number of members");
    n = atol(line);
    string member;
    for (size_t i = 0; i < n; ++i)
      {
	if (not getline(in, member))
	  ZENTHROW(InvalidRead, "cannot read member " + to_string(i));
	expected_results.append(member);
      }
    string t;
    if (not getline(in, t))
      ZENTHROW(InvalidRead, "cannot read goal type");
    goal_type = to_goal_type(t);
  }
};

DynMapTree<size_t, Goal*> Goal::goal_tbl;

size_t Goal::count = 0;

struct Plan : public Array_Graph<Graph_Anode<Goal*>, Graph_Aarc<>>
{
  DynMapTree<size_t, Goal*> goals;
  DynMapTree<string, Member> members;
  DynMapTree<size_t, Plan::Node*> nodes_tbl;

  Plan() {}// TODO borrar
  
  ~Plan()
  {
    goals.for_each([] (auto p) { delete p.second; });
  }

  void register_member(const string & name)
  {
    if (members.contains(name))
      ZENTHROW(DuplicatedName, name + " is already registered");
    members.emplace(name, name);
  }

  size_t register_goal(const string & name, const string & responsible,
		       const string & desc, const GoalType & type,
		       const string & start_time, const string & end_time)
  {
    auto ptr = members.search(responsible);
    if (ptr == nullptr)
      ZENTHROW(NameNotFound, "member " + responsible + " not found");
    auto goal = new Goal(name, responsible, desc, type,
			 to_time_t(start_time), to_time_t(end_time));
    goals.insert(goal->id, goal);
    Plan::Node * node_ptr = insert_node(goal);
    nodes_tbl.insert(node_ptr->get_info()->id, node_ptr);

    return goal->id;
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

 private:

  Goal * read_goal(istream & in)
  {
    auto row = csv_read_row(in);
    if (row.is_empty())
      return nullptr;
    auto goal = new Goal;
    goal->id = atol(row[1]);
    goal->name = row[5];
    goal->start_time = to_time_t(row[3], "%d/%m/%Y");
    goal->end_time = to_time_t(row[4], "%d/%m/%Y");
    cout << *goal << endl;

    return goal;
  }

 public:

  Plan(istream & in)
  {
    auto header = csv_read_row(in);
    while (read_goal(in))
      ;
  }
};

Plan plan;

CmdLine cmd = { "schedule", ' ', "0" };

ValueArg<string> file = { "f", "file", "load csv", true, "", "csv-name", cmd };

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  auto file_name = file.getValue();
  if (not exists_file(file.getValue()))
    ZENTHROW(FileNotFound, file_name + " not found");

  ifstream csv(file_name);

  Plan plan(csv);

  return 0;

  plan.register_member("Leandro Leon");
  plan.register_member("Ixhel Mejias");
  plan.register_member("Alberto Valderrama");
  plan.register_member("Eduardo Valderrama");
  plan.register_member("Fernando Montilla");
  plan.register_member("Neylith Quintero");
  plan.register_member("Virginia Buccellato");

  auto id1 = plan.register_goal("Modelo financiero de startup ZEN",
				"Alberto Valderrama",
				"Modelo de negocio, etc", GoalType::Platform,
				"2018:01:09", "2018:01:27");

  stringstream s;
  s << *plan.search_node(id1)->get_info() << endl;

  cout << s.str() << endl
       << endl;

  Goal g(s);

  cout << g << endl;
}

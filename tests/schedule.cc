
# include <chrono>
# include <fstream>
# include <ctime>
# include <tclap/CmdLine.h>

# include <ah-string-utils.H>
# include <parse-csv.H>
# include <ah-date.H>
# include <tpl_dynSetTree.H>
# include <tpl_agraph.H>
# include <io_graph.H>
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
  const string str = tolower(type);
  if (str == "platform" or contains(str, "infrastructure"))
    return GoalType::Platform;
  if (str == "product" or  contains(str, "innovation"))
    return GoalType::Product;
  if (str == "servicemarket" or contains(str, "customer"))
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

  Goal(istream & in)
  {
    string line;
    if (not getline(in, line))                         // id
      ZENTHROW(InvalidRead, "cannot read goal id");
    if (not is_size_t(line))
      ZENTHROW(InvalidRead, line + " is not an id");  

    if (not getline(in, name))                         // name
      ZENTHROW(InvalidRead, "cannot read goal name");

    string t;                                          // goal_type
    if (not getline(in, t))
      ZENTHROW(InvalidRead, "cannot read goal type");
    goal_type = to_goal_type(t);
    
    if (not getline(in, responsible))                  // responsible
      ZENTHROW(InvalidRead, "cannot read goal responsible");

    string date;
    if (not getline(in, date))                         // start_date
      ZENTHROW(InvalidRead, "cannot read start time");
    cout << "date = " << date << endl;
    start_time = atol(date);
    if (not getline(in, date))                         // end_date
      ZENTHROW(InvalidRead, "cannot read end time");
    end_time = atol(date);

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
	  ZENTHROW(InvalidRead, "cannot read individual action " +
		   to_string(i));
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

    // read members
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
  }

  friend ostream & operator << (ostream & out, const Goal & goal)
  {
    out << goal.id << endl
	<< goal.name << endl
	<< to_string(goal.goal_type) << endl
	<< goal.responsible << endl
	<< ctime(&goal.start_time) << endl
	<< ctime(&goal.end_time) << endl;
    if (goal.collective_actions.size() == 0)
      out << "No collective actions" << endl;
    else
      {
	out << "Collective actions:" << endl;
	for (auto a : goal.collective_actions)
	  cout << "    " << a << endl;
      }
    if (goal.individual_actions.size() == 0)
      out << "No individual actions" << endl;
    else
      {
	out << "Individual actions:" << endl;
	for (auto a : goal.individual_actions)
	  cout << "    " << a << endl;
      }
    if (goal.expected_results.size() == 0)
      out << "No expected results defined";
    else
      out << "Expected results:" << endl
	  << join(goal.expected_results.maps([] (auto & s)
					     { return "    " + s; }), "\n");
    return out << to_string(goal.goal_type);
  }
};

DynMapTree<size_t, Goal*> Goal::goal_tbl;

size_t Goal::count = 0;

struct Plan : public Array_Graph<Graph_Anode<Goal*>, Graph_Aarc<>>
{
  struct StoreGoal
  {
    void operator () (ostream & out, Plan&, Plan::Node * p)
    {
      const Goal & goal = *p->get_info();
      out << goal.id << endl
	  << goal.name << endl
	  << to_string(goal.goal_type) << endl
	  << goal.responsible << endl
	  << goal.start_time << endl
	  << goal.end_time << endl;

      out << goal.collective_actions.size() << endl;
      for (auto & a : goal.collective_actions)
	out << a << endl;

      out << goal.individual_actions.size() << endl;
      for (auto & a : goal.individual_actions)
	out << a << endl;
      
      out << goal.expected_results.size() << endl;
      for (auto & r : goal.expected_results)
	out << r << endl;
    }
  };

  struct LoadGoal
  {
    void operator () (istream & in, Plan&, Plan::Node * p)
    {
      Goal * goal = new Goal(in);
      p->get_info() = goal;
    }
  };  
  
  DynMapTree<string, Member> members;
  DynMapTree<size_t, Plan::Node*> nodes_tbl;
  DynMapTree<size_t, DynList<size_t>> deps;

  Plan() {}
  
  ~Plan()
  {
    for_each_node([] (auto ptr) { delete ptr->get_info(); });
  }

  void register_member(const string & name)
  {
    if (members.contains(name))
      ZENTHROW(DuplicatedName, name + " is already registered");
    members.emplace(name, name);
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
    size_t type_idx = 0, id_idx = 1, dep_idx = 2,
      start_date_idx = 3, end_date_idx = 4, name_idx = 5, pri_idx = 6,
      resp_idx = 7, members_idx = 8, nhours_idx = 9;
    auto row = csv_read_row(in);
    if (row.is_empty())
      return nullptr;

    const string & resp_name = row[resp_idx];
    if (not members.has(resp_name))
      members.insert(resp_name, Member(resp_name));

    const size_t goal_id = atol(row[id_idx]);
    if (nodes_tbl.has(goal_id))
      ZENTHROW(DuplicatedName, "duplicated goal id " + to_string(goal_id));

    auto goal = new Goal;
    goal->id = goal_id;
    goal->name = row[name_idx];
    goal->start_time = to_time_t(row[start_date_idx], "%d/%m/%Y");
    goal->end_time = to_time_t(row[end_date_idx], "%d/%m/%Y");
    goal->responsible = resp_name;
    goal->nhours = atol(row[nhours_idx]);
    goal->goal_type = to_goal_type(row[type_idx]);
    goal->members = split_string(row[members_idx], ","); 

    deps.insert(goal_id,
		split_string(row[dep_idx], ",").maps<size_t>([] (auto & s)
							     {
							       return stoi(s);
							     }));
    nodes_tbl.insert(goal_id, insert_node(goal));
    
    return goal;
  }

 public:

  Plan(istream & in)
  {
    auto header = csv_read_row(in);
    while (read_goal(in) != nullptr)
      ;

    for (auto it = get_node_it(); it.has_curr(); it.next())
      {
	auto * p = it.get_curr();
	auto & id = p->get_info()->id;
	auto & deplist = deps[id];
	while (not deplist.is_empty())
	  {
	    auto src_id = deplist.remove();
	    auto src_ptr = nodes_tbl[src_id];
	    insert_arc(src_ptr, p);
	  }
      }
  }
};

//Plan plan;

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

  ostringstream os;

  IO_Graph<Plan, Plan::LoadGoal, Plan::StoreGoal> io(plan);
  io.save_in_text_mode(os);

  //cout << os.str() << endl;

  istringstream is(os.str());

  cout << is.str() << endl;

  Plan p1;
  IO_Graph<Plan, Plan::LoadGoal, Plan::StoreGoal>(p1).
    load_in_text_mode(is);

  return 0;
}
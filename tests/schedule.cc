
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
# include <generate_graph.H>
# include <Tarjan.H>
# include <tpl_dynSetHash.H>

# include <aleph-exceptions.H>

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
      ALEPHTHROW(DuplicatedName, "id " + to_string(id) + " is already present");

    if (end_time <= start_time)
      throw domain_error("start time " + to_string(start_time) +
			 " is greater than end time " + to_string(end_time));
  }

  Goal(istream & in)
  {
    string line;
    if (not getline(in, line))                         // id
      ALEPHTHROW(InvalidRead, "cannot read goal id");
    if (not is_size_t(line))
      ALEPHTHROW(InvalidRead, line + " is not an id");  

    if (not getline(in, name))                         // name
      ALEPHTHROW(InvalidRead, "cannot read goal name");

    string t;                                          // goal_type
    if (not getline(in, t))
      ALEPHTHROW(InvalidRead, "cannot read goal type");
    goal_type = to_goal_type(t);
    
    if (not getline(in, responsible))                  // responsible
      ALEPHTHROW(InvalidRead, "cannot read goal responsible");

    string date;
    if (not getline(in, date))                         // start_date
      ALEPHTHROW(InvalidRead, "cannot read start time");

    start_time = atol(date);
    if (not getline(in, date))                         // end_date
      ALEPHTHROW(InvalidRead, "cannot read end time");
    end_time = atol(date);

    // Read collective actions
    if (not getline(in, line) or not is_size_t(line))
      ALEPHTHROW(InvalidRead, "cannot read number of collective actions");
    size_t n = atol(line);
    string action;
    for (size_t i = 0; i < n; ++i)
      {
	if (not getline(in, action))
	  ALEPHTHROW(InvalidRead, "cannot read collective action " +
		   to_string(i));
	collective_actions.append(action);
      }

    // read individual actions
    if (not getline(in, line) or not is_size_t(line))
      ALEPHTHROW(InvalidRead, "cannot read number of individual actions");
    n = atol(line);
    for (size_t i = 0; i < n; ++i)
      {
	if (not getline(in, action))
	  ALEPHTHROW(InvalidRead, "cannot read individual action " +
		   to_string(i));
	individual_actions.append(action);
      }

    // Read expected results
    if (not getline(in, line) or not is_size_t(line))
      ALEPHTHROW(InvalidRead, "cannot read number of expected results");
    n = atol(line);
    string result;
    for (size_t i = 0; i < n; ++i)
      {
	if (not getline(in, result))
	  ALEPHTHROW(InvalidRead, "cannot read expected result " + to_string(i));
	expected_results.append(result);
      }

    // read members
    if (not getline(in, line))
      ALEPHTHROW(InvalidRead, "cannot read number of members");
    n = atol(line);
    string member;
    for (size_t i = 0; i < n; ++i)
      {
	if (not getline(in, member))
	  ALEPHTHROW(InvalidRead, "cannot read member " + to_string(i));
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
	  out << "    " << a << endl;
      }
    if (goal.individual_actions.size() == 0)
      out << "No individual actions" << endl;
    else
      {
	out << "Individual actions:" << endl;
	for (auto a : goal.individual_actions)
	  out << "    " << a << endl;
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
  using Base = Array_Graph<Graph_Anode<Goal*>, Graph_Aarc<>>;

  Base & get_base() { return *static_cast<Base*>(this); }

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

      out << goal.members.size() << endl;
      for (auto & m : goal.members)
	out << m << endl;
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

  struct ShowNode
  {
    void operator () (const Plan&, Plan::Node * p, ostream & out) const
    {
      const Goal & goal = *p->get_info();
      out << "label = \"" << goal.id << " : " << goal.name
	  << endl
	  << goal.responsible << endl
	  << to_string(goal.start_time, "%d/%m/%Y") << " - "
	  << to_string(goal.end_time, "%d/%m/%Y") << "\"";
    }
  };

  struct ShowArc
  {
    void operator () (const Plan&, Plan::Arc * a, ostream & out) const
    {
      Plan::Node * src = static_cast<Plan::Node*>(a->src_node);
      const Goal & src_goal = *src->get_info();

      Plan::Node * tgt = static_cast<Plan::Node*>(a->tgt_node);
      const Goal & tgt_goal = *tgt->get_info();

      long secs = src_goal.end_time - src_goal.start_time;
      long days = secs / (24*60*60);
      out << "label = \"" << src_goal.id << " --> " << tgt_goal.id << "\n"
	  << days << " days / " << src_goal.nhours << "h hours \"";
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

  Plan::Node * search_node(const size_t & id) const
  {
    auto p = nodes_tbl.search(id);
    return p != nullptr ? p->second : nullptr;
  }

  void set_priority(const size_t src_id, const size_t tgt_id)
  {
    if (src_id == tgt_id)
      ALEPHTHROW(DuplicatedName, "src id " + to_string(src_id) + " is equal to " +
	       to_string(tgt_id));

    auto src_ptr = search_node(src_id);
    if (src_ptr == nullptr)
      ALEPHTHROW(NameNotFound, "src id " + to_string(src_id) + " not found");

    auto tgt_ptr = search_node(tgt_id);
    if (tgt_ptr == nullptr)
      ALEPHTHROW(NameNotFound, "tgt id " + to_string(tgt_id) + " not found");

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
    auto ptr = members.search(resp_name);
    Member & member = ptr == nullptr ?
      members.insert(resp_name, Member(resp_name))->second : ptr->second;

    const size_t goal_id = atol(row[id_idx]);
    if (nodes_tbl.has(goal_id))
      ALEPHTHROW(DuplicatedName, "duplicated goal id " + to_string(goal_id));

    auto goal = new Goal;
    goal->id = goal_id;
    goal->name = row[name_idx];
    goal->start_time = to_time_t(row[start_date_idx], "%d/%m/%Y");
    goal->end_time = to_time_t(row[end_date_idx], "%d/%m/%Y");
    goal->responsible = resp_name;
    goal->nhours = atol(row[nhours_idx]);
    goal->goal_type = to_goal_type(row[type_idx]);
    goal->members = split_string(row[members_idx], ",").
      maps([] (const string & s) { return trim(s); });
    
    member.responsible_goals.append(goal);   

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

    for (auto it = nodes_tbl.get_it(); it.has_curr(); it.next())
      {
	auto & p = it.get_curr();
	Goal * goal = p.second->get_info();
	if (goal->members.exists([] (auto & name) 
	  {
	    return contains(tolower(name), "todos");
	  }))
	  goal->members = members.keys();
	goal->members.maps<Member*>([this, goal] (auto & name)
          {
	    auto ptr = members.search(name);
	    if (ptr == nullptr)
	      
	      ALEPHTHROW(NameNotFound, "member name " + name +
		       " not found for goal id" + to_string(goal->id));
	    return &ptr->second;
	  }).for_each([goal] (Member * member_ptr)
		      {
			member_ptr->member_goals.append(goal);
		      });
      }

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

  void load(istream & in)
  {
    this->~Plan();
    new (this) Plan(in);
  }
};

Plan plan;

CmdLine cmd = { "schedule", ' ', "0" };

ValueArg<string> file = { "f", "file", "load csv", true, "", "csv-name", cmd };

SwitchArg top = { "t", "topological", "generate topological diagram", cmd };

ValueArg<size_t> rank = { "", "rank", "rank number", false, 0, "number", cmd };

SwitchArg ranks = { "r", "ranks", "list ranks", cmd };

SwitchArg resp = { "R", "responsibles", "list responsibles", cmd };

ValueArg<string> member =
  { "m", "member", "list member", false, "", "member-name", cmd };

DynList<DynList<Plan::Node*>> plan_ranks;

void list_members()
{
  if (not resp.getValue())
    return;

  cout << "Responsibles" << endl
       << join(plan.members.keys(), "\n") << endl;
  exit(0);
}

void list_member()
{
  if (not member.isSet())
    return;

  const auto & name = member.getValue();
  auto pair_ptr = plan.members.find_ptr([s = tolower(name)] (auto & p)
    {
      return contains(tolower(p.first), s);
    });
  if (pair_ptr == nullptr)
    {
      cout << "Member name " << name << " not found" << endl;
      exit(0);
    }

  const Member & member = pair_ptr->second;
  DynList<string> header = { "Goal id", "Goal name", "Duration (days)", "Hours" };
  DynList<DynList<string>> resp_goals =
    member.responsible_goals.maps<DynList<string>>([] (const Goal * const goal)
    { 
      return build_dynlist<string>(to_string(goal->id), goal->name,
				   to_string(to_days(goal->end_time -
						     goal->start_time)),
				   to_string(goal->nhours));
    });
  DynList<DynList<string>> participant_goals =
     member.member_goals.maps<DynList<string>>([] (const Goal * const goal)
    { 
      return build_dynlist<string>(to_string(goal->id), goal->name,
				   to_string(to_days(goal->end_time -
						     goal->start_time)), "-");
    });
  DynList<DynList<string>> rows = { header };
  rows.append(resp_goals);
  rows.append(participant_goals);

  auto frows = format_string(rows);

  cout << member.name << endl
       << endl
       << "  Goals where he/she is responsible:" << endl
       << justify_text(to_string(frows.take(resp_goals.size() + 1)), 120, 4)
       << endl
       << endl
       << "  Goals where he/she is member:" << endl
       << justify_text(to_string(frows.get_first()), 120, 4) << endl
       << justify_text(to_string(frows.drop(resp_goals.size() + 1)), 120, 4)
       << endl;
}

void validate_cycles()
{
  Path<Plan::Base> path(plan);
  if (not Tarjan_Connected_Components<Plan::Base>().
      compute_cycle(plan.get_base(), path))
    return;

  cout << "Detected cycle: " << endl
       << join(path.nodes().maps<string>([] (auto p)
          {
	    ostringstream s;
	    s << "    " << p->get_info()->id << " : " << p->get_info()->name;
	    return s.str();
	  }), "\n") << endl;
  exit(0);
}

void list_ranks()
{
  if (not ranks.getValue())
    return;

  plan_ranks = Q_Topological_Sort<Plan>().ranks(plan);
  size_t i = 0;
  for (auto rank_it = plan_ranks.get_it(); rank_it.has_curr();
       rank_it.next(), ++i)
    {
      cout << "Rank " << i << endl;
      for (auto it = rank_it.get_curr().get_it(); it.has_curr(); it.next())
	{
	  auto & goal = it.get_curr()->get_info();
	  cout << "    " << goal->id << " " << goal->name << endl;
	}
      cout << "================================================================"
	   << endl;
    }
  exit(0);
}

void generate_dag()
{
  if (not top.getValue())
    return;
  
  To_Graphviz<Plan, Plan::ShowNode, Plan::ShowArc>().ranks(plan, cout);
  exit(0);
}

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  auto file_name = file.getValue();
  if (not exists_file(file.getValue()))
    ALEPHTHROW(FileNotFound, file_name + " not found");

  ifstream in(file_name);

  plan.load(in);

  validate_cycles();

  list_member();
  list_members();
  list_ranks();  

  generate_dag();
}

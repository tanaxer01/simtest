#include "test_01.hpp"

XBT_LOG_NEW_DEFAULT_CATEGORY(s4u_test, "Messages specific for this s4u example");

Master::Master(std::string dag_file) 
{
  XBT_INFO("Starting master class.");
  dag = sg4::create_DAG_from_dot(dag_file);

}

void Master::operator()() 
{
  int count = 1; 
  for (const auto& host : sg4::Engine::get_instance()->get_all_hosts()) {
    sg4::Actor::create("worker" + std::to_string(count++), host, &worker);
  }
}

static void worker()
{
  XBT_INFO("Creating worker func at %s.", sg4::this_actor::get_host()->get_cname());

}

int main(int argc, char** argv) 
{
  sg4::Engine e(&argc, argv);
  xbt_assert(argc > 2, "Usage: %s plataform_file.xml dag_file.dot\n", argv[0]);

  /* Load the plataform description */
  e.load_platform(argv[1]);

  /* Initial scheduling */
  // TODO: Find "best" host to asign root
  sg4::ActorPtr root = sg4::Actor::create("master", e.host_by_name("Tremblay"), Master(argv[2]));

  /* Run the simulation */ 
  e.run();

  return 0;
}

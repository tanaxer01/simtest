#include <simgrid/s4u.hpp>

namespace sg4 = simgrid::s4u;

XBT_LOG_NEW_DEFAULT_CATEGORY(s4u_test, "Messages specific for this s4u example");


int main(int argc, char* argv[])
{
  sg4::Engine e(&argc, argv);
  xbt_assert(argc > 2, "Usage: %s platform_file deployment_file\n", argv[0]);

  /* Load the platform description and the dag for the applicationand the dag for the application  */
  e.load_platform(argv[1]);

  std::vector<sg4::ActivityPtr> dag = sg4::create_DAG_from_DAX(argv[2]);

  sg4::Host* tremblay = e.host_by_name("Tremblay");
  sg4::Host* jupiter = e.host_by_name("Jupiter");

  dynamic_cast<sg4::Exec*>(dag[0].get())->set_host(tremblay);
  dynamic_cast<sg4::Exec*>(dag[1].get())->set_host(jupiter);

  sg4::Activity::on_completion_cb([](const sg4::Activity& a) {
    bool is_exec = dynamic_cast<sg4::Exec const*>(&a) != nullptr; 

    XBT_INFO("%s '%s' is complete (start time: %f, finish time: %f)", 
      (is_exec)? "Exec" : "Comms",
      a.get_cname(),
      a.get_start_time(), a.get_finish_time());
  });

  


  e.run();
  return 0;
}
#include "simgrid/s4u.hpp"

#include <stdio.h>
#include <string.h>

XBT_LOG_NEW_DEFAULT_CATEGORY(dag_from_dax, "Logging specific to this example");
namespace sg4 = simgrid::s4u;


int main(int argc, char** argv)
{
    sg4::Engine e(&argc, argv);
    e.load_platform(argv[1]);

    std::vector<sg4::ActivityPtr> dag = sg4::create_DAG_from_DAX(argv[2]);

    if (dag.empty()) {
        XBT_ERROR("A problem occurred during DAX parsing (cycle or syntax). Do not continue this test");
        exit(2);
    }


    XBT_INFO("--------- Display all activities of the loaded DAG -----------");
    for (const auto& a : dag) {
        if (dynamic_cast<sg4::Comm*>(a.get()) != nullptr)
            continue;

        XBT_INFO("Exec %s %.0f flops to execute | Dependencies %s; Resources: %s", a->get_cname(), a->get_remaining(), 
                (a->dependencies_solved() ? "solved" : "NOT solved"),
                (a->is_assigned() ? "assigned" : "NOT assigned"));
    }

    XBT_INFO("------------------- Schedule tasks ---------------------------");
    auto hosts = e.get_all_hosts();
    auto count = e.get_host_count();
    int cursor = 0;
    // Schedule end first
    static_cast<sg4::Exec*>(dag.back().get())->set_host(hosts[0]);

    for (const auto& a : dag) {
        if (auto* exec = dynamic_cast<sg4::Exec*>(a.get()); exec != nullptr && exec->get_name() != "end") {
            exec->set_host(hosts[cursor % count]);
            cursor++;
        }

        if (auto* comm = dynamic_cast<sg4::Comm*>(a.get())) {
            const auto* pred = dynamic_cast<sg4::Exec*>((*comm->get_dependencies().begin()).get());
            const auto* succ = dynamic_cast<sg4::Exec*>(comm->get_successors().front().get());
            comm->set_source(pred->get_host())->set_destination(succ->get_host());
        }
    }

    XBT_INFO("------------------- Run the schedule -------------------------");
    double next_step = 0;
    double STEP = 1000;

    while (dag.back()->get_state() != sg4::Activity::State::FINISHED) {
        e.run_until(next_step);
        next_step += STEP;
        
        XBT_INFO("---- STEP %f | %f ----", next_step, e.get_clock());
        for (const auto& a : dag) {
            auto *exec = dynamic_cast<sg4::Exec*>(a.get());
            if (exec == nullptr)
                continue;

            if(next_step > 7000 && exec->get_name() == "1@task1" && exec->get_state() != sg4::Activity::State::FINISHED)
                exec->set_host(sg4::Host::by_name("Jupiter"));

            XBT_INFO("%s - %s; %s %f", exec->get_cname(), exec->get_host()->get_cname(), exec->get_state_str(), exec->get_remaining());
        }

    }


    XBT_INFO("-------------- Summary of executed schedule ------------------");
    for (const auto& a : dag) {

        if (const auto* exec = dynamic_cast<sg4::Exec*>(a.get())) {
            XBT_INFO("[%f->%f] '%s' executed on %s", exec->get_start_time(), exec->get_finish_time(), exec->get_cname(),
            exec->get_host()->get_cname());
        }

        if (const auto* comm = dynamic_cast<sg4::Comm*>(a.get())) {
            XBT_INFO("[%f->%f] '%s' transferred from %s to %s", comm->get_start_time(), comm->get_finish_time(),
            comm->get_cname(), comm->get_source()->get_cname(), comm->get_destination()->get_cname());
        }
    }

    XBT_INFO("TOTAL TIME: %f", e.get_clock());

    return 0;
}

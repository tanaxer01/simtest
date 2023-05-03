#include <simgrid/s4u.hpp>
#include <vector>
namespace sg4 = simgrid::s4u;

class Master {
    public:
    explicit Master(std::string dag_file);
    void operator()();

    // private:
    // long   tasks_count = 0;
    // double comp_cost = 0;
    // long   comm_cost = 0;
    // std::vector<sg4::Mailbox*> workers;
    std::vector<sg4::ActivityPtr> dag;
};

static void worker();
// class Worker {
//     public:
//     explicit Worker(std::vector<std::string> args);
//     void operator()();

//     private:
//     sg4::Mailbox* mailbox = nullptr;
// };
#include "persistence_layer.h"
#include <iostream>
#include "persistence_layer.cpp"

using namespace adaptive_ai::memory;

int main() {
    PersistenceLayer pl("data_store");

    std::string fname = "test.txt";
    std::string content = "Hello Persistence!";

    // write
    auto st = pl.write(fname, content);
    if (st == PersistStatus::Ok) {
        std::cout << "Write OK\n";
    }

    // read
    auto read = pl.read_string(fname);
    if (read) {
        std::cout << "Read: " << *read << "\n";
    }

    // remove
    pl.remove(fname);
    std::cout << "Exists after remove? " << pl.exists(fname) << "\n";

    return 0;
}

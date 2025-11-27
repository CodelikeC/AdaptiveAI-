#include "../core/rollback_evaluator.h"
using namespace adaptive_ai::analysis;
#include "../core/rollback_evaluate.cpp"

int main() {
    RollbackEvaluator eval;
    auto r = eval.evaluate(1, "{\"cpu\":80,\"mem\":4000,\"error\":5}",
                              "{\"cpu\":60,\"mem\":3000,\"error\":2}");
    std::cout << "Final score: " << r.score << "\n";

    eval.updatePolicy("{\"cpuWeight\":0.2,\"memWeight\":0.5,\"errWeight\":0.3}");
    r = eval.evaluate(2, "{\"cpu\":80,\"mem\":4000,\"error\":5}",
                         "{\"cpu\":100,\"mem\":4200,\"error\":8}");
}

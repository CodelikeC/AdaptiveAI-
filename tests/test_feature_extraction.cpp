#include <iostream>
using namespace std ; 

#include <cassert> 
#include "../modules/preprocessing/feature_extraction.h"
#include "../modules/preprocessing/freature_extraction.cpp"

int main ()
{
    cout <<"===========================" << endl;
    cout <<"==Test feature extraction==" << endl; 
    cout <<"===========================" << endl; 

   // Test 1: numeric features
    std::vector<double> nums = {10.0, 20.0, 30.0};
    auto norm = extract_numeric_features(nums);
    assert(norm.size() == nums.size());
    assert(norm.front() == 0.0);
    assert(norm.back() == 1.0);
    std::cout << "[PASS] Numeric normalization test\n";

    // Test 2: identical numeric values
    std::vector<double> same = {5.0, 5.0, 5.0};
    auto norm_same = extract_numeric_features(same);
    for (auto v : norm_same) assert(v == 0.5);
    std::cout << "[PASS] Identical numeric values test\n";

    // Test 3: key-value features
    std::map<std::string,std::string> kv = {
        {"user", "alice"},
        {"role", "admin"}
    };
    auto kv_features = extract_key_value_features(kv);
    assert(kv_features.size() == kv.size());
    for (auto& [k,v] : kv_features) {
        assert(v >= 0.0 && v <= 1.0);
    }
    std::cout << "[PASS] Key-value encoding test\n";

    // Test 4: combined features
    auto fset = extract_features(nums, kv);
    assert(fset.numeric.size() == nums.size());
    assert(fset.key_value.size() == kv.size());
    std::cout << "[PASS] Combined feature set test\n";

    std::cout << "All tests passed!\n";

    return 0; 
}


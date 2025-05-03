#include <iostream>
using namespace std ; 

#include <string> 
#include "../modules/preprocessing/feature_extraction.h"
#include "../modules/preprocessing/freature_extraction.cpp"

#include <algorithm> 

using namespace feature; 

int main ()
{
    cout <<"===========================" << endl;
    cout <<"==Test feature extraction==" << endl; 
    cout <<"===========================" << endl; 

    // Tao mot object cho test feature_extraction.cpp
    FeatureExtractor feature; 
    
    // Du lieu gia lap sau khi clean .. // 
    map<string, string> cleaned_data = {
        {"username", "alice"},
        {"ip_address", "192.168.1.1"},
        {"action", "login"},
        {"resource", "/admin"},
        {"status", "success"}
    };

    // goi ham extract_features .. 
    auto features = feature.extract_features(cleaned_data); 

    // iN ra ket qua ..// 
    for (const auto &[key,value] : features)
    {
        cout <<"Feature:" << key <<"==>" << value << endl;
    }

    // Kiem tra don gian ..// 
    if (features.size() != cleaned_data.size())
    {
        cerr <<"Feature size mismatch" <<endl; 
        return 1; 
    }

    return 0; 
}

#pragma once 
#include <iostream>
#include <string> 

#include <algorithm>
#include <unordered_map> 
#include <vector> 

using namespace std;
namespace preprocessing{
    class DataCleaner
    {
        public: 
        // lam sach mot vector du lieu dang so .. // 
        vector<double> clean_numeric_data(const vector<double> &data);
        // lam sach du lieu dang cap key - value 
        unordered_map<string, string> clean_key_value_data
        (const unordered_map<string, string> &raw_data);

        // Kiem tra xem chuoi co gia tri khong hop le hay khong .. //
        bool is_invalid_string(const string &str); 

        private:
        // chuan hoa mot chuoi (trim, lowercase...)
        string trim(const string &str); 
        // string normalize_string(const string &str); 
    }; 
}
 

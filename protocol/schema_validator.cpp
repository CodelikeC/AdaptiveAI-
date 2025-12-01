#include "schema_validator.h"
#include <iostream> 

#include <algorithm>

// You can replace this with a real Json Parser... // 
// For demontration, use a minimalistic extraction based on..

SchemaValidator :: SchemaValidator(const string &schema_json)
{
    parse_schema(schema_json); 
}

bool SchemaValidator :: parse_schema(const string &json)
{
    size_t types_pos = json.find("\"types\"");
    if (types_pos == string ::npos) return false; 

    size_t brace_start = json.find("{", types_pos); 
    size_t brace_end = json.find("}") ;

    string body = json.substr(brace_start, brace_end - brace_start); 

    // Crude parsing :: 
    size_t pos = 0; 
    while (true)
    {
        size_t key_start = body.find("\"", pos); 
        if (key_start == string ::npos) break; 
        size_t key_end = body.find("\"", key_start  + 1); 

        string type_name = body.substr(key_start + 1, key_end - key_start - 1) ; 

        size_t req_pos = body.find("",  key_end); 
        if(req_pos == string :: npos) break; 

        size_t arr_start = body.find("[", req_pos); 
        size_t arr_end = body.find("]", arr_start); 

        string arr = body.substr(arr_start, arr_end - arr_start + 1); 

        vector<string> reqs; 

        size_t p = 0; 

        while (true){
            size_t a = arr.find("\"", p); 
            if (a == string :: npos) break; 
            size_t b = arr.find("\"", a + 1) ; 

            string field = arr.substr(a + 1); 
        }
    }
}
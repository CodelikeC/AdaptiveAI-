#include <iostream> 
using namespace std; 
#include <random>
#include "adaptive_ai.h"

AdaptiveAI :: AdaptiveAI() : isOverheating_(false), isRepairing_(false){}

AdaptiveAI :: ~AdaptiveAI(){}

void AdaptiveAI :: analyzeData(const string &rawData, State state, function<void(const string&)> hitlcallback)
{
    hitlCallback_ = hitlcallback; 
    switch(state)
    {
        case State :: NORMAL_MOVEMENT:
        analysisResult_ = "Normal Movement : Data processed succesfully" + rawData;
        break; 

        case State :: INACTIVE :
        analysisResult_ = "Inactive State : No data to proceed"; 
        break; 

        case State :: CRASH : 
        analysisResult_ = "backup mode: Analyzing data normal";
        requestHITL("System crashed due to unexpected error"); 
        break; 

        case State :: UNSTABLE_MOVEMENT : 
        analysisResult_ = "Unstable Movement: Data output got some problems" + rawData;
        requestHITL("Data inconsistency detected. Approve Correction?");
        break;
        
        case State :: BACKUP: 
        analysisResult_ = "Backup mode : Analyzig data" + rawData;
        break; 
        
        case State :: TOTAL_FAILURE : 
        analysisResult_ = "Total Failure : No input data, and analysis frozen" ;
        requestHITL("Total failure detected. Initiate shutdown"); 
        break; 

        case State :: CORRUPTED_DATA : 
        analysisResult_ = "Corrupted Data: Self - defense";
        requestHITL("Corrupted data detected. Approve self - destructor"); 
        break; 

        case State :: SERVER_LOST_CONTROL:
        analysisResult_ = "Server lost control: Attempting to regain control"; 
        requestHITL("Server just lost the control");
        break; 

        case State :: DB_OVERHEATING: 
        analysisResult_ = "Database Overheating : Intiating" ; 
        isOverheating_ = true; 
        break; 

        case State :: SELF_REPAIR: 
        analysisResult_ = "Self Repair mode: Expanding server"; 
        selfRepair(); 
        break; 

        default: 
        analysisResult_ = "Unknown state"; 
        break; 
    }
}

string AdaptiveAI :: getAnalysisResult() const 
{
    return analysisResult_; 
}

void AdaptiveAI:: requestHITL (const string &issue)
{
    if (hitlCallback_)
    {
        hitlCallback_(issue); 
    }
}

void AdaptiveAI :: selfRepair()
{
    isRepairing_ = true; 
    cout <<"Self - reparing : Rellocatin resources..." << endl; 
    isRepairing_ = false; 
}

void AdaptiveAI :: expandServer(const string &externalSystem)
{
    cout <<"Expanding server : Connecting to" << externalSystem << endl; 
}
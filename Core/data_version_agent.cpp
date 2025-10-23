#include "data_version_agent.h"
#include <fstream> 

#include <iomanip>
#include <openssl/sha.h>

#include "../utils/logger.h"

using namespace std;
using namespace adaptive_ai; 
using namespace AdaptiveAI;
namespace fs = filesystem;

// --- Utility hash --- // 
string DataVersionAgent :: computeFileHash(const string &path)
{
    ifstream file (path, ios :: binary); 
    if (!file.is_open())
    return "ERR_NOFILE"; 

    SHA256_CTX ctx; 
    SHA256_Init(&ctx); 

    char buf[8192];
    while (file.good()){
        file.read(buf, sizeof(buf)); 
        SHA256_Update(&ctx, buf, file.gcount());
    }
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &ctx);

    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        ss << hex << setw(2) << setfill('0') << (int)hash[i];

    return ss.str();

    // --- Constructor --- // 
    
}

DataVersionAgent::DataVersionAgent(const std::string &base)
    : basePath(base), metaPath(base + "meta/"), versioningPath(base + "versioning/") {}

// --- INIT --- // 
void DataVersionAgent :: intialize()
{
    Logger::info("DataVersionAgent : Initializing...");
    if (!fs::exists(metaPath)) {
        fs::create_directories(metaPath);
    }

    if (fs :: exists(metaPath + "data_hashes.json")){
        ifstream f (metaPath + "data_hashes.json") ;
        string line; 
        while (getline(f, line)){
            auto pos = line.find(":"); 
            if (pos != string :: npos)
            {
                string key = line.substr(0, pos); 
                string val = line.substr(pos + 1); 
                lastSnapshot[key] = {key, val, 0, time(nullptr)};
            }
        }
    }
    else {
        Logger :: warn("DataVersionAgent : No previous snapshot found.");
    }
}

//  ---- Detect drift between hashes ---- //

bool DataVersionAgent :: detectDrift(const unordered_map<string, DataHashRecord> &newsnapshot)
{
    int changed  = 0;
    int total = 0; 
    for (const auto &kv : newsnapshot){
        ++total; 
        auto it = lastSnapshot.find(kv.first);
        if (it == lastSnapshot.end() or it -> second.hash != kv.second.hash){
            ++changed; 
        }
        double ratio = (total > 0) ? (double) changed / total : 0.0; 
        Logger :: info("DataVersionAgent : Data drift ratio = " + to_string(ratio)) ; 
        return ratio >= driftThreshold ; 
    }
}

// --- Save MEta hashers ---- // 
void DataVersionAgent :: saveMetaHash(const unordered_map<string, DataHashRecord> &snapshot )
{
    ofstream out(metaPath + "data_hashes.json", ios :: trunc);
    out <<"{\n";
    for (auto it = snapshot.begin(); it != snapshot.end(); ++it){
        out <<"\"" << it -> first << "\" : \"" << it -> second.hash << endl;
        if (next(it) != snapshot.end()){
            out << ",";
            out << endl; 
        }
    }

    out <<"} \n"; 
    out.close(); 

    Logger :: info("DataVersionAgent : Meta hashes saved."); 
}

// --- Create snapshot --- // 
void DataVersionAgent :: createSnapshot()
{
    auto now = chrono :: system_clock :: to_time_t(chrono :: system_clock :: now()); 
    string ts = to_string(now); 

    string snapFolder = versioningPath + "snapshot_"+ ts + "/";
    fs :: create_directories(snapFolder); 
    
    string cmd = "python adaptiveAI/utils/copy_dir.py" ;
    int code = system(cmd.c_str()); 

    // if else 
    if (code == 0){
        Logger :: info("DataVersionAgent : Snapshot created at ");
    }
    else {
        Logger :: error("DataVersionAgent : Snapshot creation failed") ; 
    }
}

// ---- Monitor cycle ---- // 
void DataVersionAgent :: forceSnapshot()
{

}

void DataVersionAgent :: monitorCycle ()
{
    Logger :: info("DataVersionAgent : Starting monitor cycle"); 

    unordered_map<string, DataHashRecord> newSnapshot; 

    for (auto &p : fs :: recursive_directory_iterator(basePath + "")){
        if (p.is_regular_file()){
            string h = computeFileHash(p.path().string()); 
            newSnapshot[p.path().string()] = {p.path().string(), h, p.file_size(), fs :: last_write_time(p).time_since_epoch().count()};
        }
    }

    if (detectDrift(newSnapshot)){
        Logger :: warn("DataVersionAgent : Data drift detected"); 
        if (autoSnapshot)
        {
            createSnapshot();
            saveMetaHash(newSnapshot); 
            lastSnapshot = newSnapshot; 
        }
        else
        {
            Logger :: warn("DataVersionAgent : Auto snapshot is disabled."); 
        }
    }
    else 
    {
        Logger :: info("DataVersionAgent : No significant data drift detected"); 
    }
}

// -- Force snapshot Manually -- // 
void DataVersionAgent :: forceSnapshot()
{
    Logger :: info("DataVersionAgent : Forced snapshot triggered"); 
    createSnapshot(); 
}
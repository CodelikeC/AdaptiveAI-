#include <iostream> 
#include <string> 
#include <algorithm> 
#include <vector> 

using namespace std; 

#include "plugin_interface.h"
#include "../modules/protection/intrusion_detector.h"

namespace adaptive_ai
{
    class IntrusionDetectorPlugin: public Plugin
    {
        public: 
        string name () const override
        {
            return "Intrusion Detector"; 
        }

        void intialize() override
        {
            // KHoi tao neu can ..// 
        }

        void execute() override 
        {
            map<string,string> input 
            = {
                /*.....*/
            }; 
            detector.detect_intrusion(input); 
        }

        void shutdown() override
        {
            // don dep
        }

        private:
        IntrusionDetector detector; 
    }; 

    extern "C" Plugin *create_plugin()
    {
        return new IntrusionDetectorPlugin(); 
    }
}
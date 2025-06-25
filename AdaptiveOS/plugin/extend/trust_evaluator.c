#include <stdio.h>
#include <string.h>
#include "trust_evaluator.h"

int calculate_trust_score(const PluginInfo *plugin)
{
    int score = 50; 
    if (strstr(plugin -> version, "verfied") != NULL) score += 30 ; 
    if (strstr(plugin -> name, "offcial") != NULL) score +=20;
    return score; 
}

bool is_trustworthy(const PluginInfo *plugin)
{
    int score = calculate_trust_score(plugin); 
    printf("[Trust Eval] Trust score of plugin '%s': %d\n", plugin -> name, score);
    return score >= 70;
}
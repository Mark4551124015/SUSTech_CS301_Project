#ifndef __CALC_ALGO_HPP__
#define __CALC_ALGO_HPP__

#include <string>

#ifdef __cplusplus
extern "C" {
#endif

using std::string;

string getExRes(string expression);
string getEqRes(string equation);
string getBinRes(string binaryExperssion);

#ifdef __cplusplus
}
#endif

#endif
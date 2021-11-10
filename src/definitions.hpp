/*
 *------------------------------------------------------
 * (c) 2021, mohab metwally All Rights Reserved
 *
 * the content of this file may not be disclosed, copied, or duplicated in
 * any form, in whole, or part, without the prior wirtten permission of
 *  mohab metwally
 *------------------------------------------------------
 *
 * @author: mohab metwally
 * @date: 2021/11/8
 */

#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <chrono>

using namespace std;
using namespace std::filesystem;

typedef std::chrono::duration<int, std::ratio<60,1>> Minutes;
#define ONE_MINUTE std::chrono::duration<int, std::ratio<60,1>>(1)

const int SLOWMA=55;
const int FASTMA=21;

// bar signals string for i/o
const std::string BUY_STR("BUY");
const std::string SELL_STR("SELL");
const std::string WAIT_STR("");



const std::string CSVROW0 = "TIME, OPEN, HIGH, LOW, CLOSE, FASTMA, SLOWMA, SIGNAL";

const std::filesystem::path STOCK_DATA_PATH;

const float tick_size=0.05;
const int threshold = tick_size; //strict change;
static string TM_FMT_IN = "%H:%M:%S"; //%2H:%2M:%2S
static char DELIMT_IN = ',';  // input file delimiter ','
static char DELIMT_OUT = ',';  // input file delimiter ','
static string TM_FMT_OUT = "%H:%M";

/*
   this is the time point used to indicate Bar start
*/
typedef std::chrono::system_clock::time_point Time;


enum StrategyType
{
    MA
};

enum Signal {
    WAIT=0,
    BUY=1, //buy shares
    SELL=2 //sell shares
};

struct Price
{
    float m_price;
    Time m_time;
};

/*
 * print time to the stdout, was meant for debugging purpose
 */
void printTime(Time t)
{
    std::time_t tm = std::chrono::system_clock::to_time_t(t);
    std::cout << std::put_time(std::localtime(&tm), TM_FMT_OUT.c_str()) << std::endl;
}

/*
 * take back time to the start of the minute for the given time
 *
 * @param t: time_point
 * @returns Time: time at the start of the current minute
 */
Time beginOfMinute(Time &t)
{
    //shift cur_start to the start of the minute
    std::time_t cur_start_time = std::chrono::system_clock::to_time_t(t);
    std::tm *cur_start_tm = std::localtime(&cur_start_time);
    cur_start_tm->tm_sec=0;
    //

    t = std::chrono::system_clock::from_time_t(std::mktime(cur_start_tm));
    return t;
}

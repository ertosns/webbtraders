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

//#include <ostream>
//#include <chrono>
#include "definitions.hpp"

using namespace std;

/*
 * \class Bar
 *
 * Bar is a stock bar with {open, close, high, low, fastma, slowma} over some duration
 */
class Bar
{
public:

    Bar() = delete; /* delete default constructor */
    /*
     * Bar constructor
     * @param t: bar start time point
     * @param open: bar open price
     * @param close: bar close price
     * @param high: bar high price
     * @param low: bar low price
     */
    Bar(Time t, float open, float close, float high, float low) :
        m_t(t), m_open(open), m_close(close), m_high(high), m_low(low)
    {
        //
    }

    void SetFastma(float fastma)
    {
        m_fastma=fastma;
    }
    void SetSlowma(float slowma)
    {
        m_slowma=slowma;
    }
    void SetSignal(Signal sig)
    {
        m_sig = sig;
    }
    Time GetTime() const
    {
        return m_t;
    }
    float GetOpen() const
    {
        return m_open;
    }
    float GetClose() const
    {
        return m_close;
    }
    float GetHigh() const
    {
        return m_high;
    }
    float GetLow() const
    {
        return m_low;
    }
    float GetFastma() const
    {
        return m_fastma;
    }
    float GetSlowma() const
    {
        return m_slowma;
    }
    Signal GetSignal() const
    {
        return m_sig;
    }

    friend ostream& operator<<(ostream&, Bar &);

private:
    const Time m_t; // bar start time_point
    const float m_open; //open price
    const float m_close; //close price
    const float m_high; //high price
    const float m_low; //low price
    /*
     * the following variables are assigned through the processor
     */
    float m_fastma=0; //fast simple moving averages over the past 21 closes
    float m_slowma=0; //slow simple moving averages over the past 55 closes
    Signal m_sig=Signal::WAIT; //take no action
};

ostream& operator<<(ostream &os, Bar &bar)
{
    /*
     * print bar in the following format
     * %H:%M,%2f,%2f,%2f,%2f,%4f,[SIG]
     *
     */
    Time time = bar.GetTime();
    std::time_t tm = std::chrono::system_clock::to_time_t(time);
    os << put_time(localtime(&tm), TM_FMT_OUT.c_str());
    os << DELIMT_OUT
       << std::setprecision(2) << std::fixed
       << bar.GetOpen()   << DELIMT_OUT
       << std::setprecision(2)  << std::fixed
       << bar.GetHigh()   << DELIMT_OUT
       << std::setprecision(2)  << std::fixed
       << bar.GetLow()    << DELIMT_OUT
       << std::setprecision(2)  << std::fixed
       << bar.GetClose()  << DELIMT_OUT
       << std::setprecision(4)  << std::fixed
       << bar.GetFastma() << DELIMT_OUT
       << std::setprecision(4)  << std::fixed
       << bar.GetSlowma() << DELIMT_OUT;

    //trigger events
    Signal sig = bar.GetSignal();
    if (sig==Signal::SELL)
    {
        os << SELL_STR;
    }
    else if (sig==Signal::BUY)
    {
        os << BUY_STR;
    }
    else
    {
        os << WAIT_STR;
    }
    os << endl;
    return os;
}

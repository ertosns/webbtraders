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

#include <vector>
#include <cmath>

#include "definitions.hpp"
#include "stockParser.hpp"

class Stock
{

public:
    Stock() = delete;
    /*
     * construct Stock from StockParser, bar separation duration
     *
     * @param parser: StockParser of prices
     * @param minutes: bar separation duration
     */
    Stock(StockParser &parser, Minutes minutes=ONE_MINUTE) : m_parser(parser)
        {
            populateBars(minutes);
        }
    ~Stock()
        {
            //
        }
    vector<shared_ptr<Bar>> GetBars()
        {
            return m_bars;
        }
    const size_t Size() const{
        return m_bars.size();
    }

    void Exec(Signal);

private:
    StockParser m_parser;
    vector<shared_ptr<Bar>> m_bars;
    /*
     * populate bars with with given duration
     *
     * @param dur: bar separation duration
     */
    void populateBars(Minutes dur)
        {
            StockParser::Iterator it=m_parser.begin();

            Time cur_start = it->m_time;
            beginOfMinute(cur_start);
            Time cur_end = cur_start+dur;
            //

            Price open_price = *it;
            float open = open_price.m_price;
            float close = open;
            float low = open;
            float high = open;

            for(; it!=m_parser.end(); it++)
            {
                if (it->m_time>=cur_end)
                {
                    // construct bar
                    auto bar = make_shared<Bar>(cur_start,
                                                open,
                                                close,
                                                high,
                                                low);
                    m_bars.push_back(bar);
                    // set parameters of next Bar

                    cur_start=it->m_time;
                    beginOfMinute(cur_start);
                    cur_end=cur_start+dur;

                    open = it->m_price;
                    low = open;
                    high = open;
                }
                close = it->m_price;
                high = std::fmax(high, close);
                low = std::fmin(low, close);
            }
            // only if there is more to add for the final bar
            if (cur_end !=cur_start)
            {
                auto bar = make_shared<Bar>(cur_start, open, close, high, low);
                m_bars.push_back(bar);
            }
        }
};

/*
 * \class Share
 *
 * Share is the receiver in the command pattern
 * it's a concrete Stock with shares's count, and stock name,
 * and trading actions
 */
class Share : public Stock
{
public:
    Share() = delete; /* delete default constructor */
    /*
     * construct Share from StockParser, bar separation duration
     *
     * @param parser: StockParser of prices
     * @param minutes: bar separation duration
     */
    Share(StockParser &parser, Minutes minutes=ONE_MINUTE, string name="webbtraders", int count=1)
        : Stock(parser, minutes), m_name(name), m_count(count)
        {
            //
        }
    ~Share()
        {
            //destruct Share
        }
    /* exec is a function triggered by buy/sell event
     * execute buy/sell command
     */
    void Exec(Signal)
        {
            //TODO (impl) get rich!
        }

private:
    int m_count; //count of shares of that stock
    string m_name;
    /*
     * buy shares
     */
    void Buy()
        {
            //TODO (impl) buy low
        }
    /*
     * sell shares
     */
    void Sell()
        {
            //TODO (impl) sell high
        }
};

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

#include <memory>
#include <vector>
#include <ostream>
#include <numeric>
#include <algorithm>

#include "definitions.hpp"
#include "bar.hpp"
#include "strategy.hpp"

using namespace std;


/* \class SharesProcessor
 *
 * SharesProcessor is the invoker in the command pattern for
 * processing shares by the  given strategy,
 * it's also a publisher for shares.
*/
class SharesProcessor
{

public:
    SharesProcessor(vector<shared_ptr<Share>> shares)
        {
            //
            for (auto share : shares)
            {
                CalcSMA(share);
                m_shares.push_back(share);
            }
            //
        }
    void operator()(StrategyType type)
        {
            switch(type)
            {
            case StrategyType::MA:
            {
                MaStrategy strategy(m_shares);
                strategy.Exec();
            }
            };

        }
    /*
     * subscribe share to be traded
     */
    void SubscribeShare(shared_ptr<Share> share)
        {
            CalcSMA(share);
            m_shares.push_back(share);
        }
    /*
     * unsubscribe a share  from the processor
     */
    bool UnsubscribeShare(shared_ptr<Share> share)
        {
            for(auto it=m_shares.begin(); it<m_shares.end(); it++)
            {
                if (*it==share)
                {
                    m_shares.erase(it);
                    return true;
                }
            }
            return false;
        }
    std::vector<shared_ptr<Share>> GetShares() { return m_shares; }
    friend ostream& operator<<(ostream&, SharesProcessor&);
private:
    /*
     * calculate the simple moving averages over the close prices
     */
    void CalcSMA(shared_ptr<Share> share)
        {
            auto bars = share->GetBars();
            auto acc_closes = [](float sum,  shared_ptr<Bar> &b)
                                  {
                                      return sum + b->GetClose();
                                  };
            auto start = bars.rbegin();
            //calculate fast ma.
            float price_fma, price_sma;
            for(auto it=bars.rbegin(); it!=bars.rend(); it++)
            {

                if (it-start+FASTMA  < bars.rend()-bars.rbegin())
                {

                    price_fma = accumulate(it, it+FASTMA, 0.0, acc_closes)/float(FASTMA);
                    (*it)->SetFastma(price_fma);
                }


                if (it-start+SLOWMA < bars.rend()-bars.rbegin())
                {
                    price_sma = accumulate(it, it+SLOWMA, 0.0, acc_closes)/float(SLOWMA);
                    (*it)->SetSlowma(price_sma);
                }

            }
        }

    std::vector<shared_ptr<Share>> m_shares;
};

ostream& operator<<(ostream& os, SharesProcessor& proc)
{
    os << CSVROW0 << endl;
    for (auto share_ptr : proc.GetShares())
    {
        for (auto bar : share_ptr->GetBars())
        {
            os << *bar;
        }
    }
    return os;
}

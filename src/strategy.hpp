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


#include <memory>
#include <vector>

#include "definitions.hpp"
#include "share.hpp"

using namespace std;


/* \class Strategy
 *
 * Strategy is a abstract command interface in the command pattern
 * for trading strategies
 * Strategy applies on vector of Shares

*/

class Strategy
{
public:
    Strategy() {
    }


    Strategy(vector<shared_ptr<Share>> shares)
        {
            for (auto share : shares)
            {
                m_shares.push_back(share);
            }
        }

    ~Strategy()
        {

        }
    void operator()(vector<shared_ptr<Share>> share) {
        SetShare(share);
    }

    /*
     * execute the strategy
     */
    void Exec();
protected:
    vector<shared_ptr<Share>> GetShare() { return m_shares; }
    void SetShare(vector<shared_ptr<Share>> &shares)
        {
            m_shares.resize(0);
            for(auto share : shares)
            {
                m_shares.push_back(share);
            }
        }
private:
    vector<shared_ptr<Share>> m_shares; //share receiver
};


/* \class MaStrategy
 * MaStrategy is a concrete command for sma
 * buy: if the simple fast moving averages crosses above simple slow moving averages
 * sell: if the simple fast moving averages crosses below simple slow moving averages
 * MaStrategy applies on vector of Shares at once
*/
class MaStrategy : public Strategy
{
public:
    MaStrategy()
        {

        }

    MaStrategy(vector<shared_ptr<Share>> shares)
        : Strategy(shares)
        {

        }
    ~MaStrategy()
        {

        }
    virtual void Exec()
        {
            //strategy
            //at the beginning  fastma, slowma are the same.
            //
            auto shares = GetShare();
            for (auto share : shares)
            {
                ExecShare(share);
            }
        }
    /*
     * execute strategy on a single share of a stock
     *
     * @param share: shared_ptr of a share
     */
    virtual void ExecShare(shared_ptr<Share> share)
        {
            Signal prev_sig=Signal::WAIT;
            Signal cur_sig=Signal::WAIT;

            vector<shared_ptr<Bar>> bars = share->GetBars();
            for (auto bar : bars)
            {
                if ((bar->GetFastma() > bar->GetSlowma()))
                {
                    cur_sig =  Signal::BUY;
                }
                else if (bar->GetFastma() < bar->GetSlowma())
                {
                    cur_sig =  Signal::SELL;
                }
                else
                {
                    cur_sig=Signal::WAIT;
                    bar->SetSignal(cur_sig);
                }
                // trigger an event
                if (cur_sig!=prev_sig) {
                    // set bar signal
                    bar->SetSignal(cur_sig);
                    // trigger an event
                    share->Exec(cur_sig);
                }
                prev_sig=cur_sig;
            }
        }
};

#include <iostream>
#include <filesystem>
#include <chrono>
#include "definitions.hpp"
#include "stockParser.hpp"
#include "shareProcessor.hpp"
#include "share.hpp"

void trade()
{
    std::filesystem::path in("../data/market_data.csv");
    StockParser stock(in);
    //instantiate the receiver Share
    auto share =  make_shared<Share>(stock, ONE_MINUTE);
    vector<shared_ptr<Share>> shares;
    shares.push_back(share);
    //instantiate the invoker
    SharesProcessor processor(shares);
    //apply some command
    processor(StrategyType::MA);

    //fstream f("data/signals.csv", std::ios::out);
    //f << processor;
    std::cout << processor;
}
int main(int argc, char **args)
{
    //auto t1 = std::chrono::steady_clock::now();
    trade();
    //auto t2 = std::chrono::steady_clock::now();
    //std::cout << float(std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()) / 1000 << " secs" << std::endl;
    //0.13 seconds << 3 seconds
    return 0;
};

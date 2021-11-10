#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <iomanip>
#include <exception>
#include <algorithm>    // std::count
#include "definitions.hpp"
#include "bar.hpp"

using namespace std;
using namespace filesystem;

class StockParser
{
  /*
   * parse stock prices output file
   * the output file contains timestamps and prices using the following format:
   * HH:MM:SS.mmm,price
   */
public:
    struct Iterator
    {
        //iterator protocol
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using vaue_type = Price;
        using pointer = Price*;
        using reference = Price&;

        Iterator(pointer ptr) : m_ptr(ptr) {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }
        Iterator& operator++() { m_ptr++; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };
    private:
        pointer m_ptr;
    };
    StockParser() = delete;
    StockParser(string path)
        : m_file(path, std::ios::in)
        {
            //(1) open the file
            //f_file.open(path, std::ios::in);
            //m_size =   std::count(std::istreambuf_iterator<char>(m_file),
            //                    std::istreambuf_iterator<char>(), '\n');

            //(2) populate prices
            populate_prices();
        }
    StockParser(const StockParser &parser)
        {
            for (int i=0; i<parser.Size(); i++)
            {
                m_prices.push_back(parser.m_prices[i]);
            }
        }
    ~StockParser()
        {
            //
            m_file.flush();
            m_file.close();
        }
    Iterator begin() { return Iterator(&m_prices[0]); }
    Iterator end() { return Iterator(&m_prices[Size()]); }
    size_t Size() const
        {
            //return m_size;
            return m_prices.size();
        }


    /* parse validated price line
     *
     * @pram priceLine: is string of the format  HH:MM:SS.mmm,price
     *
     * @return Price
     */
    void populate_prices()
        {
            string priceline;
            //int counter =0;
            while (getline(m_file, priceline))
            {
                Price price;
                ParsePrice(price, priceline);
                //m_prices[counter++]=price;
                m_prices.push_back(price);
            }
        }
    void ParsePrice(Price &price, string priceLine)
        {
            std::istringstream ss(priceLine);
            std::string stampStr, priceStr;
            std::getline(ss, stampStr, DELIMT_IN);
            std::getline(ss, priceStr, DELIMT_IN);

            std::stringstream ts(stampStr);

            std::tm tm = {};
            ts >> std::get_time(&tm, TM_FMT_IN.c_str());
            auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
            price.m_time = tp;
            price.m_price = std::stof(priceStr);
        }
    fstream m_file;
    //size_t m_size;
    vector<Price> m_prices;

};

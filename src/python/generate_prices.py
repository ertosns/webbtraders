# TODO: I feel like the requirements/comments are correct but the code contains some bugs

'''
Script to generate random market data and export it to a file specified via the script's arguments.

The script was written for Python 3.

Requirements:
- The output file contains timestamps and prices using the following format:
   HH:MM:SS.mmm,price
- The price in the output file should be rounded to the tick size (0.05).
- The minimum possible price is equal to the tick size.
- A price entry should only be added to the output file when the price changed.
    Note: When the first price is 0.193 and the next is 0.192 it doesn't count
    as a price change, because when rounding to the tick size the price didn't
    change (both round to 0.2). So in this case only one entry should be included
    in the output file.
- There can't be more than one entry for the same timestamp.
- The timestamps should be in chronological order.
- You may assume that there are no bugs in the random number functions (rand, rand_int and rand_double).
    We have chosen to include these functions because different Python versions generate different random numbers.
- The first 4 lines of the output file should be:
    09:00:00.000,100
    09:00:00.355,100.2
    09:00:01.188,100.1
    09:00:01.258,100.15
- The last 3 lines of the output file should be:
    17:29:58.502,97.45
    17:29:58.587,97.5
    17:29:59.206,97.65
- The file should have 53,749 entries.
'''

import argparse
from datetime import datetime
from datetime import timedelta


TIME_FORMAT = '%H:%M:%S.%f'
TICK_SIZE = 0.05

SEED = 1


def rand():
    '''
    Note: You can assume there to be no bugs in this function.
    Pseudorandom number generator using the
    linear congruential generator (LCG) algorithm.
    '''
    global SEED
    SEED = (1103515245 * SEED + 12345) % (2 ** 31)
    return SEED


def rand_int(min, max):
    '''
    Note: You can assume there to be no bugs in this function.
    Generate a pseudorandom integer where:
        min <= result < max
    '''
    return (rand() % abs(max - min)) + min


def rand_double(min, max, decimals=3):
    '''
    Note: You can assume there to be no bugs in this function.
    Generate a pseudorandom double between min and max.
    '''
    accuracy = 10.0 ** decimals
    return rand_int(min * accuracy, max * accuracy) / accuracy


def round_to_tick_size(price, tick_size):
    '''
    Round the price to the nearest multiple of the tick size.
    Doesn't return more than 2 decimals.
    Example:
        round_to_tick_size(1.37, 0.05) -> 1.35
        round_to_tick_size(1.38, 0.05) -> 1.4
        round_to_tick_size(1.38, 0.5) -> 1.5
        round_to_tick_size(1.3856, 0.001) -> 1.39
    '''
    if tick_size==0:
        raise ZeroDivisionError('tick size cant be zero')
    return round(tick_size*round(price/tick_size), 2)

def price_changed(price0, price1):
    '''
    Return true if the rounded to nearest multiple,
    of the tick size prices aren't changed, false otherwise
    '''
    return round_to_tick_size(price0) == round_to_tick_size(price1)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--file_path',
                        help='The path to the output file to generate',
                        required=True)
    args = parser.parse_args()

    # Initialization
    rows = []
    price = 100
    current_time = datetime.strptime('09:00:00.000', TIME_FORMAT)
    end_time = datetime.strptime('17:30:00.000', TIME_FORMAT)

    # Generation of prices and timestamps
    last_price = None
    while current_time <= end_time:

        # Making sure that there's a price change for this timestamp. Otherwise, skip
        if price != last_price:
            formatted_time = str(current_time.strftime(TIME_FORMAT))[:-3]
            rows.append('{0},{1}'.format(formatted_time, price))

        last_price = price
        # Generate the new price and timestamp
        price += rand_double(-0.2, 0.2)
        price = round_to_tick_size(price, TICK_SIZE)
        price = max(TICK_SIZE, price)
        current_time += timedelta(milliseconds=rand_int(1, 1000))

    # Write the generated values to a file
    with open(args.file_path, 'w+') as file_handler:
        for row in rows:
            file_handler.write(row + '\n')

if __name__ == '__main__':
    main()

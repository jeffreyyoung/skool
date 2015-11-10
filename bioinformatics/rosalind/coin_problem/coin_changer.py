def greedyGetChange(money, coins):
    coins.sort(reverse=True)
    change = []
    for coin in coins:
        while money > 0 and (money - coin) >= 0:
            money -= coin
            #print 'subtracting ' + str(coins[-i])
            change.append(coin)
    return len(change)

def getChange(money, coins):
    change = []
    for i in range( 1, len(coins) + 1):
        change.append( greedyGetChange(money, coins[0:i]) )
    return min(x for x in change if x > 0)


if __name__ == "__main__":
    import sys
    with open(sys.argv[1]) as fh:
        money = int( next(fh).strip() )
        coins = [int(x) for x in next(fh).strip().split(',')]

    print getChange(money, coins)

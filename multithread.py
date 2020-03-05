from threading import Thread
import timeit

def getDivisors(n):
    start = 1
    counter = 0
    while start < n: 
        if (n % start==0): 
            counter += 1
        start = start + 1
    return counter

def findHighestDivisorFrom(start, end, curMax, curMaxCounter, tNum, totalTs):
    segments = end / totalTs
    if tNum == 1:
        start = 1
    else:
        start = segments * (tNum -1)
    end = segments * tNum
    for i in range(int(start), int(end)):
        numDiv = getDivisors(i)
        if numDiv > curMaxCounter:
            # print(i, " has ", numDiv, " diviors, which is higher than ", curMax, " with ", curMaxCounter, " divisors!\n")
            curMaxCounter = numDiv
            curMax = i

start = 1
end = 10000
curMax = 0
curMaxCounter = 0

start = timeit.default_timer()
single = Thread(target=findHighestDivisorFrom, args=(start, end, curMax, curMaxCounter, 1, 1))
single.start()
single.join()
stop = timeit.default_timer()
print('Single Threading Time: ', stop - start, "\n")  

start = timeit.default_timer()
double1 = Thread(target=findHighestDivisorFrom, args=(start, end, curMax, curMaxCounter, 1, 2))
double2 = Thread(target=findHighestDivisorFrom, args=(start, end, curMax, curMaxCounter, 2, 2))
double1.start()
double2.start()
double1.join()
double2.join()
stop = timeit.default_timer()
print('Double Threading Time: ', stop - start, "\n")  

# Triple Threading
start = timeit.default_timer()
trios = [Thread(target=findHighestDivisorFrom, args=(start, end, curMax, curMaxCounter, 1, 3)),
         Thread(target=findHighestDivisorFrom, args=(start, end, curMax, curMaxCounter, 2, 3)),
         Thread(target=findHighestDivisorFrom, args=(start, end, curMax, curMaxCounter, 3, 3))]
for i in range(len(trios)):
    trios[i].start()
for i in range(len(trios)):
    trios[i].join()
stop = timeit.default_timer()
print('Triple Threading Time: ', stop - start, "\n")  
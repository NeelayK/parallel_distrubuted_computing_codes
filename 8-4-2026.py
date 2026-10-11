import time
from multiprocessing import Pool

def slow_function(x):
    time.sleep(1)
    return x * x

if __name__ == "__main__":

    start = time.time()
    results = [slow_function(x) for x in range(4)]
    print("Sequential Time:", time.time() - start)

    start = time.time()
    with Pool(4) as p:
        results = p.map(slow_function, range(4))
    print("Parallel Time:", time.time() - start)
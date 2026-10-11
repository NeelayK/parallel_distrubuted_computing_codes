import time 
import multiprocessing as mp

def worker_add(data):
    return[x+10 for x in data]

def worker_square(data):
    return[x**2 for x in data]

def worker_cube(data):
    return[x**3 for x in data]

def worker_half(data):
    return[x/2 for x in data]

def execute_func(data, func):
      return func(data)

def mimd(data):
    data = list(range(1,21))
    chunk_size = len(data) // 4
    chunks = [data[0:chunk_size], data[chunk_size:2*chunk_size], data[2*chunk_size:3*chunk_size], data[3*chunk_size:]]

    with mp.Pool(processes=4) as pool:
        results = pool.starmap(execute_func, [(chunks[0], worker_add), (chunks[1], worker_square), (chunks[2], worker_cube), (chunks[3], worker_half)])
    return results

if __name__ == "__main__":
    start = time.time()
    result = mimd(list(range(1,21)))
    end = time.time()
    print(f"MIMD Executed Time: {end - start:.6f} seconds")
    print("MIMD Result", result)
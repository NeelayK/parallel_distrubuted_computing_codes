import time 
import numpy as np

data = np.arange(1,50_000_000)

def sisd():
    result=[]
    for x in data:
        result.append(int(x*2))
    return result

if __name__ == "__main__":
    start = time.time()
    result =  sisd()
    end = time.time()
    print(f"SISD Executed Time: {end - start:.6f} seconds")
    print("SISD Result (first 10)", result[:10])
    
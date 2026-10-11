import time 
import numpy as np

data = np.arange(1,50_000_000)

def misd():
   result1 = data*2
   result2 = data+3
   result3 = data*2
   return result1[:10], result2[:10], result3[:10]

if __name__ == "__main__":
    start = time.time()
    result =  misd()
    end = time.time()
    print(f"MISD Executed Time: {end - start:.6f} seconds")
    print("MISD Result (first 10)", result[:10])
    
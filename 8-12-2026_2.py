import time 
import numpy as np

data = np.arange(1,50_000_000)

def simd():
    return np.array(data) * 2

if __name__ == "__main__":
    start = time.time()
    result =  simd()
    end = time.time()
    print(f"SIMD Executed Time: {end - start:.6f} seconds")
    print("SIMD Result (first 10)", result[:10])
    
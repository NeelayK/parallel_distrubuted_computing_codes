import concurrent.futures
import numpy as np
import threading 
import time

def simd_add(core_id, thread_id, a,b):
    result = np.add(a,b)
    print(f"Core {core_id}, Thread {thread_id}")
    return result

def worker(core_id,thread_id):
    start_time = time.perf_counter()
    a = np.arange(8) + (core_id * 0.31 + thread_id * 0.428187648786173)
    b = np.ones(8) * (core_id + thread_id * 0.487681378)
    result = simd_add(core_id, thread_id, a, b)
    end_time = time.perf_counter()
    execution_time = end_time - start_time
    print(f"Core {core_id}, Thread {thread_id}: Execution Time: {execution_time}")
    return result, execution_time

if __name__ == "__main__":
    num_cores = 2
    num_threads_per_core = 2
    total_threads = num_cores * num_threads_per_core

    with concurrent.futures.ThreadPoolExecutor(max_workers=total_threads) as executor:
        futures = []
        for core_id in range(num_cores):
            for thread_id in range(num_threads_per_core):
                futures.append(executor.submit(worker, core_id, thread_id))
        all_results = [future.result() for future in concurrent.futures.as_completed(futures)]
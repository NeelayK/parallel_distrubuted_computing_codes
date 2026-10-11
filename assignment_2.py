import matplotlib.pyplot as plt

threads = [1, 2, 4, 8, 16]

serial = [2.560767, 2.453092, 2.452974, 2.412885, 2.432263]
par    = [2.750695, 2.299824, 1.505031, 1.094831, 1.119370]
gpu    = [3.047756, 1.995922, 1.276670, 1.409843, 1.813043]

T_serial = sum(serial) / len(serial)

par_speedup = [T_serial / t for t in par]
gpu_speedup = [T_serial / t for t in gpu]
par_eff = [s / p for s, p in zip(par_speedup, threads)]
gpu_eff = [s / p for s, p in zip(gpu_speedup, threads)]

print(f"Baseline serial time = {T_serial:.4f} s\n")
print("Threads | Par time | Par S | Par E | GPU time | GPU S | GPU E")
for i in range(len(threads)):
    print(f"{threads[i]:7d} | {par[i]:8.4f} | {par_speedup[i]:5.2f} | {par_eff[i]:5.2f} | "
          f"{gpu[i]:8.4f} | {gpu_speedup[i]:5.2f} | {gpu_eff[i]:5.2f}")

fig, ax = plt.subplots(1, 3, figsize=(16, 4.5))

ax[0].plot(threads, par, 'o-', label="OpenMP")
ax[0].plot(threads, gpu, 'g-', label="Fake GPU")
ax[0].axhline(T_serial, color='gray', linestyle='--', label="Serial")
ax[0].set_title("Execution Time"); ax[0].set_ylabel("Seconds")

ax[1].plot(threads, par_speedup, 'r-', label="OpenMP")
ax[1].plot(threads, gpu_speedup, 'b-', label="Fake GPU")
ax[1].plot(threads, threads, 'k--', label="Ideal")
ax[1].set_title("Speedup"); ax[1].set_ylabel("T_serial / T_parallel")

ax[2].plot(threads, par_eff, 'b-', label="OpenMP")
ax[2].plot(threads, gpu_eff, 'p-', label="Fake GPU")
ax[2].set_title("Efficiency"); ax[2].set_ylabel("Speedup / Threads")

for a in ax:
    a.set_xlabel("Threads")
    a.set_xticks(threads)
    a.grid(True)
    a.legend()

plt.savefig("performance.png", dpi=150)
plt.show()
import matplotlib.pyplot as plt

threads = [1, 2, 4, 8, 16, 32, 64]
time = [0.019631, 0.010207, 0.005078, 0.005319, 0.003830, 0.004524, 0.004976]
speedup = [0.979735, 1.884362, 3.787697, 3.615903, 5.021240, 4.251753, 3.864878]
efficiency = [97.973506, 94.218100, 94.692422, 45.198791, 31.382748, 13.286727, 6.038872]

fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(18, 5))

ax1.plot(threads, time, marker='o', color='tab:red', linewidth=2, markersize=6)
ax1.set_title('Execution Time vs. Threads', fontsize=12, fontweight='bold')
ax1.set_xlabel('Number of Threads', fontsize=10)
ax1.set_ylabel('Time (seconds)', fontsize=10)
ax1.set_xscale('log', base=2)
ax1.grid(True, which="both", ls="--", alpha=0.5)

ax2.plot(threads, speedup, marker='s', color='tab:blue', linewidth=2, markersize=6, label='Actual Speedup')

ax2.plot(threads, threads, linestyle='--', color='gray', label='Ideal Speedup')
ax2.set_title('Speedup vs. Threads', fontsize=12, fontweight='bold')
ax2.set_xlabel('Number of Threads', fontsize=10)
ax2.set_ylabel('Speedup (S)', fontsize=10)
ax2.set_xscale('log', base=2)
ax2.legend()
ax2.grid(True, which="both", ls="--", alpha=0.5)

ax3.plot(threads, efficiency, marker='^', color='tab:green', linewidth=2, markersize=6)
ax3.set_title('Parallel Efficiency vs. Threads', fontsize=12, fontweight='bold')
ax3.set_xlabel('Number of Threads', fontsize=10)
ax3.set_ylabel('Efficiency (%)', fontsize=10)
ax3.set_xscale('log', base=2)
ax3.set_ylim(0, 105)
ax3.grid(True, which="both", ls="--", alpha=0.5)

plt.tight_layout()

plt.savefig('openmp_performance_analysis.png', dpi=300)
print("Plot saved successfully as 'openmp_performance_analysis.png'.")

plt.show()
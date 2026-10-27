import subprocess
import matplotlib.pyplot as plt

try:
    subprocess.run(["gcc", "-fopenmp", "assignment_4.c", "-o", "assignment_4"], check=True)
    print("Compilation successful.")
except subprocess.CalledProcessError as e:
    print(f"Compilation failed: {e}")

try:
    result = subprocess.run(["./assignment_4"], capture_output=True, text=True, check=True)
    print("\n--- C Program Output ---")
    print(result.stdout)
except Exception as e:
    print(f"Execution failed: {e}")

threads = [2, 4, 8]
speedups = [0.43, 0.28, 0.16]
efficiencies = [s / t for s, t in zip(speedups, threads)]

plt.figure(figsize=(12, 5))

plt.subplot(1, 2, 1)
plt.plot(threads, speedups , marker='o', color='royalblue', linewidth=2, label='Actual Speedup')
plt.axhline(y=1.0, color='crimson', linestyle='--', label='Break-even (Speedup = 1.0)')
plt.title('Speedup vs. Thread Count (N = 30)')
plt.xlabel('Number of Threads')
plt.ylabel('Speedup')
plt.xticks(threads)
plt.grid(True, linestyle=':', alpha=0.6)
plt.legend()

plt.subplot(1, 2, 2)
plt.plot(threads, efficiencies, marker='s', color='forestgreen', linewidth=2, label='Efficiency')
plt.title('Efficiency vs. Thread Count (N = 30)')
plt.xlabel('Number of Threads')
plt.ylabel('Efficiency')
plt.xticks(threads)
plt.grid(True, linestyle=':', alpha=0.6)
plt.legend()

plt.tight_layout()
plt.savefig('performance_analysis.png', dpi=300)
print("\nPerformance analysis plot saved as 'performance_analysis.png'.")
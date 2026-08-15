import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("part1_results.csv")
df.columns = df.columns.str.strip()

print("Columns found in CSV:", df.columns.tolist())
print(df.head())

# 1. Traffic Intensity vs Queueing Delay
plt.figure(figsize=(7, 4.5))
plt.plot(df['rho'], df['avg_q_delay'], marker='o', color='b')
plt.title('Traffic Intensity (ρ) vs. Average Queueing Delay')
plt.xlabel('Traffic Intensity (ρ)')
plt.ylabel('Avg Queueing Delay (s)')
plt.grid(True)
plt.tight_layout()
plt.savefig('rho_vs_q_delay.png')
plt.close()

# 2. Traffic Intensity vs End-to-End Delay
plt.figure(figsize=(7, 4.5))
plt.plot(df['rho'], df['avg_e2e_delay'], marker='s', color='g')
plt.title('Traffic Intensity (ρ) vs. Average End-to-End Delay')
plt.xlabel('Traffic Intensity (ρ)')
plt.ylabel('Avg End-to-End Delay (s)')
plt.grid(True)
plt.tight_layout()
plt.savefig('rho_vs_e2e_delay.png')
plt.close()

# 3. Traffic Intensity vs Packet-Drop Probability
plt.figure(figsize=(7, 4.5))
plt.plot(df['rho'], df['drop_prob'], marker='^', color='r')
plt.title('Traffic Intensity (ρ) vs. Packet-Drop Probability')
plt.xlabel('Traffic Intensity (ρ)')
plt.ylabel('Packet-Drop Probability')
plt.grid(True)
plt.tight_layout()
plt.savefig('rho_vs_drop_prob.png')
plt.close()

print("Graphs plotted successfully!")

#python3 plot_graphs.py
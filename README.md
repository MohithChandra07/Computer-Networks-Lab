# Computer Networks Laboratory (CN LAB) 🌐

Welcome to the **Computer Networks Laboratory** repository for Semester 5. This repository hosts all weekly lab assignments, discrete-event network simulation codes, traffic datasets, graph visualization scripts, and analytical lab reports.

---

## 📂 Repository Structure

```text
CN LAB/
├── README.md                  # Main repository documentation
├── .gitignore                 # File exclusions (binary executables, macOS metadata)
├── Lab1/                      # Lab 1: M/M/1 & M/M/1/K Queueing Simulation
│   ├── 1.c                    # Discrete-event queue simulator in C
│   ├── plot_graphs.py         # Python visualization script (Pandas + Matplotlib)
│   ├── queue_sim_results.csv  # Simulation result dataset
│   ├── part1_results.csv      # Experiment Part 1 metrics
│   ├── part2_results.csv      # Experiment Part 2 metrics
│   └── Lab1-QueueSim.pdf      # Detailed Lab Report / Problem Statement
└── Lab2/                      # Lab 2: Queueing Dynamics & Wireshark Analysis
    ├── queue_sim.c            # Updated multi-parameter queue simulator in C
    ├── plot_graphs.py         # Automated graph generator script
    ├── part1_results.csv      # Traffic intensity sweep metrics
    ├── part2_results.csv      # Repeated trial stability metrics
    ├── rho_vs_q_delay.png     # Traffic Intensity vs Queueing Delay Plot
    ├── rho_vs_e2e_delay.png   # Traffic Intensity vs End-to-End Delay Plot
    ├── rho_vs_drop_prob.png   # Traffic Intensity vs Packet Drop Probability Plot
    └── Lab2-QueueSim-Wireshark.pdf # Lab Report & Wireshark capture analysis
```

---

## 🔬 Lab Overview & Problem Statements

### 📍 Lab 1: Discrete Event Queue Simulator ($M/M/1/K$ Model)

#### 1. Problem Statement
Simulate a single-router network path consisting of two communication links ($Link_1$ and $Link_2$) and an intermediate router buffer. Evaluate packet delay components and packet drop dynamics under varying traffic intensities ($\rho \in [0.1, 1.2]$).

#### 2. Network Parameters & Setup
* **Packet Size ($L$):** $1000 \text{ bytes} = 8000 \text{ bits}$
* **Link 1 Bandwidth ($R_1$):** $10 \text{ Mbps}$
* **Link 2 Bandwidth ($R_2$):** $10 \text{ Mbps}$
* **Propagation Delays ($d_{p1}, d_{p2}$):** $1.0 \text{ ms}$ each
* **Router Processing Delay ($d_{\text{proc}}$):** $0.1 \text{ ms}$
* **Buffer Capacity ($K$):** $50 \text{ packets}$
* **Arrival Process:** Poisson process with rate $\lambda = \frac{\rho \cdot R_2}{L}$
* **Packets Simulated per Run:** $100,000$

#### 3. Key Formulas Used
* **Transmission Delay:** $d_{\text{trans}} = \frac{L}{R}$
* **End-to-End Delay:** $d_{\text{e2e}} = (d_{\text{trans1}} + d_{p1}) + d_{\text{proc}} + d_{\text{queueing}} + (d_{\text{trans2}} + d_{p2})$
* **Packet Drop Probability:** $P_{\text{drop}} = \frac{\text{Dropped Packets}}{\text{Total Generated Packets}}$

---

### 📍 Lab 2: Queueing Dynamics & Wireshark Packet Analysis

#### 1. Problem Statement
Analyze queue stability, packet drop behavior, and delay degradation as traffic intensity $\rho$ approaches and exceeds saturation ($\rho \ge 1.0$). Perform packet capture inspection using Wireshark.

#### 2. Key Observations
1. **Low Traffic ($\rho < 0.7$):**
   * Queueing delay is negligible ($\approx 0.25 - 1.49 \text{ ms}$).
   * Zero packet drops ($P_{\text{drop}} = 0.0$).
2. **High Traffic ($\rho \to 1.0$):**
   * Queue length rapidly reaches maximum capacity ($K = 50$).
   * Average queueing delay rises sharply ($\approx 25.4 \text{ ms}$).
   * Drop probability starts accumulating ($P_{\text{drop}} \approx 1.97\%$).
3. **Over-saturated Traffic ($\rho > 1.0$):**
   * Severe buffer overflows occur. At $\rho = 1.2$, $P_{\text{drop}} \approx 16.59\%$.

#### 3. Performance Visualizations
* **Traffic Intensity vs. Queueing Delay:** `rho_vs_q_delay.png`
* **Traffic Intensity vs. End-to-End Delay:** `rho_vs_e2e_delay.png`
* **Traffic Intensity vs. Packet Drop Probability:** `rho_vs_drop_prob.png`

---

## 🛠️ Tools & Technologies Used

* **Programming Languages:** C (C99 standard for simulation engine), Python 3 (Data analysis & plotting)
* **Libraries:** `Pandas`, `Matplotlib`, `math.h`, `stdlib.h`
* **Network Tools:** Wireshark
* **Compiler:** GCC (`gcc -O2`)
* **Version Control:** Git & GitHub

---

## 🚀 How to Run the Simulations & Plot Graphs

### Step 1: Compile and Run C Simulation
Navigate to the specific lab directory (e.g., `Lab2`):
```bash
cd Lab2
gcc -O2 queue_sim.c -o queue_sim -lm
./queue_sim
```
*Outputs generated: `part1_results.csv`, `part2_results.csv`*

### Step 2: Generate Graphs
Ensure dependencies are installed:
```bash
python3 -m pip install pandas matplotlib
```
Run the plotting script:
```bash
python3 plot_graphs.py
```
*Outputs generated: PNG visualization plots in the current directory.*

---

## 📝 Weekly Workflow Template (For New Labs)

For each new weekly assignment:
1. Create a new directory: `mkdir LabX`
2. Add problem statement, simulation code (`.c`), and dataset (`.csv`).
3. Generate relevant plots using `plot_graphs.py`.
4. Update this `README.md` with the new experiment findings and conclusion.
5. Commit and push:
   ```bash
   cd "/Users/mohithchandra/Documents/LABS SEM 5/CN LAB"
   git add .
   git commit -m "Add Lab X assignment code and report"
   git push origin main
   
   ```

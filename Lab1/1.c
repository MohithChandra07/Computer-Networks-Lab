#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void run_simulation(
    double L_bits,
    double R1,
    double R2,
    double dp1,
    double dp2,
    double d_proc,
    int queue_capacity,
    double rho,
    int num_packets,
    unsigned int seed,
    FILE *csv_file
) {
    srand(seed);

    double lambda = (rho * R2) / L_bits;
    double trans_delay_link1 = L_bits / R1;
    double trans_delay_link2 = L_bits / R2;

    int generated = 0;
    int delivered = 0;
    int dropped = 0;

    double current_time = 0.0;
    double router_free_time = 0.0;

    double *queue_finish_times = (double *)malloc(sizeof(double) * queue_capacity);
    int queue_size = 0;
    int max_queue_occ = 0;

    double total_queueing_delay = 0.0;
    double total_e2e_delay = 0.0;

    for (int i = 0; i < num_packets; i++) {
        double u = (double)rand() / RAND_MAX;
        while (u == 0 || u == 1) u = (double)rand() / RAND_MAX;
        double inter_arrival = -log(u) / lambda;

        current_time += inter_arrival;
        generated++;

        double router_arrival = current_time + trans_delay_link1 + dp1;

        int new_queue_size = 0;
        for (int j = 0; j < queue_size; j++) {
            if (queue_finish_times[j] > router_arrival) {
                queue_finish_times[new_queue_size++] = queue_finish_times[j];
            }
        }
        queue_size = new_queue_size;

        if (queue_size >= queue_capacity) {
            dropped++;
        } else {
            queue_size++;
            if (queue_size > max_queue_occ) {
                max_queue_occ = queue_size;
            }

            double start_service = (router_arrival > router_free_time) ? router_arrival : router_free_time;
            double queueing_delay = start_service - router_arrival;
            
            double router_departure = start_service + d_proc + trans_delay_link2;
            router_free_time = router_departure;

            queue_finish_times[queue_size - 1] = router_departure;

            double e2e_delay = (trans_delay_link1 + dp1) + d_proc + queueing_delay + trans_delay_link2 + dp2;

            total_queueing_delay += queueing_delay;
            total_e2e_delay += e2e_delay;
            delivered++;
        }
    }

    free(queue_finish_times);

    double drop_prob = (double)dropped / generated;
    double avg_queue_delay = (delivered > 0) ? (total_queueing_delay / delivered) : 0.0;
    double avg_e2e_delay = (delivered > 0) ? (total_e2e_delay / delivered) : 0.0;

    fprintf(csv_file, "%.2f,%.4f,%d,%d,%d,%.6f,%.6f,%.6f,%d\n",
            rho, lambda, generated, delivered, dropped, drop_prob, avg_queue_delay, avg_e2e_delay, max_queue_occ);

    printf("%-6.2f | %-10.2f | %-9d | %-9d | %-8d | %-10.6f | %-14.6f | %-13.6f | %-8d\n",
           rho, lambda, generated, delivered, dropped, drop_prob, avg_queue_delay, avg_e2e_delay, max_queue_occ);
}

int main() {
    double packet_length_bytes = 1000.0;
    double R1 = 10000000.0;
    double R2 = 10000000.0;
    double dp1 = 0.001;
    double dp2 = 0.001;
    double d_proc = 0.0001;
    int queue_capacity = 50;
    int num_packets = 100000;
    unsigned int seed = 42;

    double rhos[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.95, 1.0, 1.1, 1.2};
    int num_rhos = sizeof(rhos) / sizeof(rhos[0]);

    FILE *csv_file = fopen("queue_sim_results.csv", "w");
    if (!csv_file) {
        printf("Error opening output file!\n");
        return 1;
    }

    fprintf(csv_file, "Traffic_Intensity,Lambda,Generated,Delivered,Dropped,Drop_Probability,Avg_Queueing_Delay,Avg_E2E_Delay,Max_Queue_Occupancy\n");

    printf("\n===========================================================================================================================\n");
    printf("%-6s | %-10s | %-9s | %-9s | %-8s | %-10s | %-14s | %-13s | %-8s\n",
           "rho", "Lambda", "Generated", "Delivered", "Dropped", "Drop_Prob", "Avg_Queue_Dly", "Avg_E2E_Dly", "Max_Occ");
    printf("===========================================================================================================================\n");

    for (int i = 0; i < num_rhos; i++) {
        run_simulation(
            packet_length_bytes * 8.0, R1, R2, dp1, dp2, d_proc,
            queue_capacity, rhos[i], num_packets, seed, csv_file
        );
    }

    printf("===========================================================================================================================\n");
    printf("Simulation complete. Results saved to 'queue_sim_results.csv'.\n\n");

    fclose(csv_file);
    return 0;
}
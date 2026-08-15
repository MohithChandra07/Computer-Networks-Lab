#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SOURCES 10
#define QUEUE_CAPACITY 50 // Bounded buffer capacity (packets)
#define SIM_TIME 1000.0   // Total simulation time in seconds

// Packet structure
typedef struct {
  double arrival_time;
  int source_id;
} Packet;

typedef struct {
  Packet buffer[QUEUE_CAPACITY];
  int front;
  int rear;
  int count;
} Queue;

void init_queue(Queue *q) {
  q->front = 0;
  q->rear = -1;
  q->count = 0;
}

int is_full(Queue *q) { return q->count == QUEUE_CAPACITY; }

int is_empty(Queue *q) { return q->count == 0; }

void enqueue(Queue *q, Packet p) {
  if (!is_full(q)) {
    q->rear = (q->rear + 1) % QUEUE_CAPACITY;
    q->buffer[q->rear] = p;
    q->count++;
  }
}

Packet dequeue(Queue *q) {
  Packet p = {0, -1};
  if (!is_empty(q)) {
    p = q->buffer[q->front];
    q->front = (q->front + 1) % QUEUE_CAPACITY;
    q->count--;
  }
  return p;
}

// Generate exponential random variate
double get_exponential(double rate) {
  double u = ((double)rand() + 1.0) / ((double)RAND_MAX + 2.0);
  return -log(1.0 - u) / rate;
}

// Simulation Function
void run_simulation(int N, double *lambda_sources, double packet_len_bits,
                    double bandwidth_bps, FILE *csv_file) {
  double agg_lambda = 0.0;
  for (int i = 0; i < N; i++) {
    agg_lambda += lambda_sources[i];
  }

  double service_rate = bandwidth_bps / packet_len_bits;
  double rho = (agg_lambda * packet_len_bits) / bandwidth_bps;

  Queue q;
  init_queue(&q);

  double current_time = 0.0;
  double next_arrival[MAX_SOURCES];
  for (int i = 0; i < N; i++) {
    next_arrival[i] = get_exponential(lambda_sources[i]);
  }

  double next_departure = DBL_MAX;
  int server_busy = 0;
  Packet current_serving_pkt;

  // Statistics
  long total_generated = 0;
  long total_delivered = 0;
  long total_dropped = 0;
  double total_queueing_delay = 0.0;
  double total_end_to_end_delay = 0.0;
  int max_queue_occupancy = 0;

  while (current_time < SIM_TIME) {
    // Find next event
    int next_src = 0;
    double min_arrival = next_arrival[0];
    for (int i = 1; i < N; i++) {
      if (next_arrival[i] < min_arrival) {
        min_arrival = next_arrival[i];
        next_src = i;
      }
    }

    if (min_arrival < next_departure) {
      // ARRIVAL EVENT
      current_time = min_arrival;
      total_generated++;

      Packet new_pkt = {current_time, next_src};
      next_arrival[next_src] =
          current_time + get_exponential(lambda_sources[next_src]);

      if (!server_busy) {
        server_busy = 1;
        current_serving_pkt = new_pkt;
        double service_time = get_exponential(service_rate);
        next_departure = current_time + service_time;
        total_queueing_delay += 0.0;
      } else {
        if (!is_full(&q)) {
          enqueue(&q, new_pkt);
          if (q.count > max_queue_occupancy) {
            max_queue_occupancy = q.count;
          }
        } else {
          total_dropped++;
        }
      }
    } else {
      // DEPARTURE EVENT
      current_time = next_departure;
      total_delivered++;

      double e2e_delay = current_time - current_serving_pkt.arrival_time;
      total_end_to_end_delay += e2e_delay;

      if (!is_empty(&q)) {
        current_serving_pkt = dequeue(&q);
        double q_delay = current_time - current_serving_pkt.arrival_time;
        total_queueing_delay += q_delay;

        double service_time = get_exponential(service_rate);
        next_departure = current_time + service_time;
      } else {
        server_busy = 0;
        next_departure = DBL_MAX;
      }
    }
  }

  double drop_prob = (double)total_dropped / total_generated;
  double avg_q_delay =
      (total_delivered > 0) ? (total_queueing_delay / total_delivered) : 0.0;
  double avg_e2e_delay =
      (total_delivered > 0) ? (total_end_to_end_delay / total_delivered) : 0.0;

  fprintf(csv_file, "%.2f, %.2f, %ld, %ld, %ld, %.6f, %.6f, %.6f, %d\n", rho,
          agg_lambda, total_generated, total_delivered, total_dropped,
          drop_prob, avg_q_delay, avg_e2e_delay, max_queue_occupancy);
}

int main() {
  srand(42);
  double L = 8000.0;    // Packet length: 8000 bits (1 KB)
  double R = 8000000.0; // Bandwidth: 8 Mbps -> Service rate = 1000 pkts/s

  FILE *f1 = fopen("part1_results.csv", "w");
  fprintf(f1, "rho,agg_lambda,generated,delivered,dropped,drop_prob,avg_q_"
              "delay,avg_e2e_delay,max_queue\n");

  // Part 1: Varying rho
  double rhos[] = {0.2, 0.4, 0.6, 0.8, 0.9, 1.0, 1.2};
  for (int i = 0; i < 7; i++) {
    double agg_lambda = (rhos[i] * R) / L;
    double lambda_i = agg_lambda / 4.0;
    double sources[4] = {lambda_i, lambda_i, lambda_i, lambda_i};
    run_simulation(4, sources, L, R, f1);
  }
  fclose(f1);

  // Part 2: Asymmetric rates with agg_lambda = 1000
  FILE *f2 = fopen("part2_results.csv", "w");
  fprintf(f2,
          "rho, agg_lambda, generated, delivered, dropped, drop_prob, avg_q_"
          "delay, avg_e2e_delay, max_queue\n");

  double set1[4] = {100, 100, 100, 700};
  double set2[4] = {150, 150, 250, 450};
  double set3[4] = {250, 250, 250, 250};
  double set4[4] = {400, 300, 200, 100};

  run_simulation(4, set1, L, R, f2);
  run_simulation(4, set2, L, R, f2);
  run_simulation(4, set3, L, R, f2);
  run_simulation(4, set4, L, R, f2);
  fclose(f2);

  printf("Simulation completed. CSV files generated.\n");
  return 0;
}

// gcc -O2 queue_sim.c -o queue_sim -lm
// ./queue_sim
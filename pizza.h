#ifndef PIZZA_H
#define PIZZA_H

#define _POSIX_C_SOURCE 199309L

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

//dinoume times stis metavlites pou tha xrisimopoihthoun sto main programma
#define Ntel 2
#define Ncook 2
#define Noven 10
#define Ndeliverer 10

#define Torderlow 1
#define Torderhigh 5

#define Norderlow 1
#define Norderhigh 5

#define Pm 0.35
#define Pp 0.25
#define Ps 0.40

#define Tpaymentlow 1
#define Tpaymenthigh 3

#define Pfail 0.05

#define Cm 10
#define Cp 11
#define Cs 12

#define Tprep 1
#define Tbake 10
#define Tpack 1

#define Tdellow 5
#define Tdelhigh 15


typedef struct {
    int order_id;
    int num_pizzas;
    int num_margaritas;
    int num_pepperonis;
    int num_specials;
    double order_time;
    double delivery_start;
    double delivery_end;
} order_t;

//dhlwnei metablites
void *orderHandler(void *arg);
void printRevenue();
void init();
void erase();

//dhlwnei oti ta mutex kai cond vriskontai se allo arxeio alla uparxoun
extern pthread_mutex_t print_mutex;
extern pthread_mutex_t global_time_mutex;
extern pthread_mutex_t call_center_mutex;
extern pthread_cond_t call_center_cond;
extern pthread_mutex_t cook_mutex;
extern pthread_cond_t cook_cond;
extern pthread_mutex_t oven_mutex;
extern pthread_cond_t oven_cond;
extern pthread_mutex_t delivery_mutex;
extern pthread_cond_t delivery_cond;
extern pthread_mutex_t revenue_mutex;
extern pthread_mutex_t maxServiceTime_mutex;
extern pthread_mutex_t waiting_time_mutex;
extern pthread_mutex_t cooling_time_mutex;
extern pthread_mutex_t delivery_time_mutex;
extern pthread_mutex_t calling_time_mutex;
extern pthread_mutex_t delivery_mutex;
extern pthread_cond_t delivery_cond;

extern double total_revenue;
extern int order_count;
extern int successful_orders;

extern int totalMargaritas;
extern int totalPeperonis;
extern int totalSpecials;

extern double maxServiceTime;
extern int maxServiceTimeOrderId;
extern double total_waiting_time;

extern double maxCoolingTime;
extern int maxCoolingTimeOrderID;
extern double total_cooling_time;

extern double total_calling_waiting_tim;
extern double max_calling_waiting_time;
extern int max_calling_waiting_time_order_id;

extern double total_delivery_ride_time;
extern double max_delivery_ride_time;
extern int max_delivery_ride_order_id;
extern int count_deliveries;




#endif // PIZZA_H
#include "p3220239-p3220133-pizza.h"

//dhlwnoume ta mutex kai cond 
pthread_mutex_t print_mutex;
pthread_mutex_t global_time_mutex;
pthread_mutex_t call_center_mutex;
pthread_cond_t call_center_cond;
pthread_mutex_t cook_mutex;
pthread_cond_t cook_cond;
pthread_mutex_t oven_mutex;
pthread_cond_t oven_cond;
pthread_mutex_t delivery_mutex;
pthread_cond_t delivery_cond;
pthread_mutex_t revenue_mutex;
pthread_mutex_t maxServiceTime_mutex;
pthread_mutex_t waiting_time_mutex;
pthread_mutex_t cooling_time_mutex;
pthread_mutex_t delivery_time_mutex;
pthread_mutex_t calling_time_mutex;
pthread_mutex_t delivery_mutex;
pthread_cond_t delivery_cond;
int free_delivery_boys = Ndeliverer;
int free_call_receivers = Ntel;
int free_cooks = Ncook;
int free_ovens = Noven;

double total_revenue = 0.0;
int order_count = 0;
int successful_deliveries = 0;

int totalMargaritas = 0;
int totalPeperonis = 0;
int totalSpecials = 0;

double maxServiceTime = 0.0;
int maxServiceTimeOrderID = 0;
double total_waiting_time = 0.0;


double maxCoolingTime = 0.0;
int maxCoolingTimeOrderID = 0;
double total_cooling_time = 0.0;

double total_calling_waiting_time = 0.0;
double max_calling_waiting_time = 0.0;
int max_calling_waiting_time_order_id = 0;

int successful_orders = 0;
double total_delivery_ride_time = 0.0;
double max_delivery_ride_time = 0.0;
int max_delivery_ride_order_id = 0;
int count_deliveries = 0;

struct timespec global_time;

//synartisi gia na girnaei to global time se deuterolepta
double get_global_time() {
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return (now.tv_sec - global_time.tv_sec) + (now.tv_nsec - global_time.tv_nsec) / 1e9;
}
//synartisi pou xrisimopoieitai gia thn paroxi asfaleias sta nhmata kai stin ektyposi minimaton
void safePrint(const char* format, ...) {
    va_list args;
    va_start(args, format);

    pthread_mutex_lock(&print_mutex);
    vprintf(format, args);
    fflush(stdout);
    pthread_mutex_unlock(&print_mutex);

    va_end(args);
}
//arxikopoihsh twn mutex kai cond pou dhlwsame proigoumenos 
void init() {
    clock_gettime(CLOCK_REALTIME, &global_time);
    pthread_mutex_init(&global_time_mutex, NULL);
    pthread_mutex_init(&print_mutex, NULL);
    pthread_mutex_init(&call_center_mutex, NULL);
    pthread_cond_init(&call_center_cond, NULL);
    pthread_mutex_init(&cook_mutex, NULL);
    pthread_cond_init(&cook_cond, NULL);
    pthread_mutex_init(&oven_mutex, NULL);
    pthread_cond_init(&oven_cond, NULL);
    pthread_mutex_init(&delivery_mutex, NULL);
    pthread_cond_init(&delivery_cond, NULL);
    pthread_mutex_init(&revenue_mutex, NULL);
    pthread_mutex_init(&maxServiceTime_mutex, NULL);
    pthread_mutex_init(&waiting_time_mutex, NULL);
    pthread_mutex_init(&cooling_time_mutex, NULL);
    pthread_mutex_init(&calling_time_mutex, NULL);
    pthread_mutex_init(&delivery_time_mutex, NULL);

}
//katastrefei ta mutex kai cond
void erase() {
    pthread_mutex_destroy(&call_center_mutex);
    pthread_cond_destroy(&call_center_cond);
    pthread_mutex_destroy(&cook_mutex);
    pthread_cond_destroy(&cook_cond);
    pthread_mutex_destroy(&oven_mutex);
    pthread_cond_destroy(&oven_cond);
    pthread_mutex_destroy(&delivery_mutex);
    pthread_cond_destroy(&delivery_cond);
    pthread_mutex_destroy(&revenue_mutex);
    pthread_mutex_destroy(&global_time_mutex);
    pthread_mutex_destroy(&print_mutex);
    pthread_mutex_destroy(&maxServiceTime_mutex);
    pthread_mutex_destroy(&waiting_time_mutex);
    pthread_mutex_destroy(&cooling_time_mutex);
    pthread_mutex_destroy(&calling_time_mutex);
    pthread_mutex_destroy(&delivery_time_mutex);

}
//h sunartisi h opoia vrisketai pisw apo thn epeksergasia dedomenwn twn nhmatwn kai to printarisma twn minimatwn
void *orderHandler(void *arg) {
    int customer_id = *(int *)arg;
    double start_time, connected_time, charge_time;
    double serviceTime = 0.0;
    //kanei lock ton poro afou tha ton xrisimopoihsei to nhma
    pthread_mutex_lock(&global_time_mutex);
    start_time = get_global_time();
    //upologizei thn wra pou den phre kaneis endiamesa stis 2 klhseis 
    int delay = customer_id == 1 ? 0 : (rand() % (Torderhigh - Torderlow + 1)) + Torderlow;
    //kathisterei thn leitourgeia kata delay 
    sleep(delay);  
    
    safePrint("Caller is calling [Time: %.2f minutes]\n", start_time + delay);
    double calltime = get_global_time();
    //kanei unlock ton poro afou teleiwse h leitourgia tou nhmatos
    pthread_mutex_unlock(&global_time_mutex);
    //kanei lock ton poro afou tha ton xrisimopoihsei to nhma
    pthread_mutex_lock(&call_center_mutex);
    while (free_call_receivers <= 0) {
        pthread_cond_wait(&call_center_cond, &call_center_mutex);
    }
    free_call_receivers--;
    //kanei unlock ton poro afou teleiwse h leitourgia tou nhmatos
    pthread_mutex_unlock(&call_center_mutex);
    //kanei lock ton poro afou tha ton xrisimopoihsei to nhma
    pthread_mutex_lock(&global_time_mutex);
    connected_time = get_global_time();
    //kanei unlock ton poro afou teleiwse h leitourgia tou nhmatos
    pthread_mutex_unlock(&global_time_mutex);

    double calling_waiting_time = connected_time - calltime;
    //kanei lock ton poro afou tha ton xrisimopoihsei to nhma
    pthread_mutex_lock(&calling_time_mutex);
    total_calling_waiting_time += calling_waiting_time;
    if (max_calling_waiting_time < calling_waiting_time) {
        max_calling_waiting_time = calling_waiting_time;
        max_calling_waiting_time_order_id = customer_id;
    }
    //kanei unlock ton poro afou teleiwse h leitourgia tou nhmatos
    pthread_mutex_unlock(&calling_time_mutex);
    safePrint("Client was connected with call receiver [Time: %.2f minutes]\n", connected_time);

    int payment_time = (rand() % (Tpaymenthigh - Tpaymentlow + 1)) + Tpaymentlow;
    sleep(payment_time);  
    charge_time = get_global_time();
    //kanei lock ton poro afou tha ton xrisimopoihsei to nhma
    pthread_mutex_lock(&revenue_mutex);
    order_count++;
    int order_number = order_count;  //mas dinei to epomeno noumero paragelias
    //kanei unlock ton poro afou teleiwse h leitourgia tou nhmatos
    pthread_mutex_unlock(&revenue_mutex);

    if ((double)rand() / RAND_MAX < Pfail) {
        safePrint("####Client with Order %d FAILED to be charged [Time: %.2f minutes]\n", order_number, charge_time);
        //kanei lock ton poro afou tha ton xrisimopoihsei to nhma
        pthread_mutex_lock(&call_center_mutex);
        free_call_receivers++;
        pthread_cond_signal(&call_center_cond);
        //kanei unlock ton poro afou teleiwse h leitourgia tou nhmatos
        pthread_mutex_unlock(&call_center_mutex);
        return NULL;  // Exit the thread function to stop processing this order
    }

    //synexizei mono an h paraggelia einai epitixis 
    //kanei lock ton poro afou tha ton xrisimopoihsei to nhma
    pthread_mutex_lock(&revenue_mutex);
    //metra tis epitixis paraggelies gia ta statistika
    successful_orders++; 
    //kanei unlock ton poro afou teleiwse h leitourgia tou nhmatos
    pthread_mutex_unlock(&revenue_mutex);

    int num_pizzas = (rand() % (Norderhigh - Norderlow + 1)) + Norderlow;
    int margaritas = 0, pepperonis = 0, specials = 0;
    for (int i = 0; i < num_pizzas; i++) {
        double pizza_type = (double)rand() / RAND_MAX;
        if (pizza_type < Pm) margaritas++;
        else if (pizza_type < Pm + Pp) pepperonis++;
        else specials++;
    }
    double order_revenue = margaritas * Cm + pepperonis * Cp + specials * Cs;
    //kanei lock ton poro afou tha ton xrisimopoihsei to nhma
    pthread_mutex_lock(&revenue_mutex);
    total_revenue += order_revenue;
    totalMargaritas += margaritas;
    totalPeperonis += pepperonis;
    totalSpecials += specials;
    //kanei unlock ton poro afou teleiwse h leitourgia tou nhmatos
    pthread_mutex_unlock(&revenue_mutex);

    safePrint("Order %d was charged SUCCESSFULLY [Margaritas:%d, Pepperonis:%d, Specials:%d][Time:%.2f minutes]\n", order_number, margaritas, pepperonis, specials, charge_time);
    //kanei lock ton poro afou tha ton xrisimopoihsei to nhma
    pthread_mutex_lock(&call_center_mutex);
    free_call_receivers++;
    pthread_cond_signal(&call_center_cond);
    //kanei unlock ton poro afou teleiwse h leitourgia tou nhmatos
    pthread_mutex_unlock(&call_center_mutex);
    //kanei lock ton poro afou tha ton xrisimopoihsei to nhma
    pthread_mutex_lock(&cook_mutex);
    //an den einai diathesimo perimenei na vrethei kapoios
    while (free_cooks <= 0) {
        pthread_cond_wait(&cook_cond, &cook_mutex);
    }
    free_cooks--;
    //kanei unlock ton poro afou teleiwse h leitourgia tou nhmatos
    pthread_mutex_unlock(&cook_mutex);
    double cook_time = get_global_time();
    safePrint("Order %d was taken by a cook [Time: %.2f minutes]\n", order_number, cook_time);
    //perimenei oso o xronos Tprep gia kathe pizza
    sleep(Tprep * num_pizzas);
    double prep_time = get_global_time();
    safePrint("Order %d was prepared [Time: %.2f minutes]\n", order_number, prep_time);

    

    //kanei lock ton poro afou tha ton xrisimopoihsei to nhma
    pthread_mutex_lock(&oven_mutex);
    while (free_ovens < num_pizzas) {
        pthread_cond_wait(&oven_cond, &oven_mutex);
    }
    free_ovens -= num_pizzas;
    //kanei unlock ton poro afou teleiwse h leitourgia tou nhmatos
    pthread_mutex_unlock(&oven_mutex);
    safePrint("The Order %d was placed in the Oven [Time: %.2f minutes]\n", order_number, prep_time);
    //perimenei oso to Tbake
    sleep(Tbake);
    double bake_time = get_global_time();
    safePrint("The Order %d is baked [Time: %.2f minutes]\n", order_number, bake_time);
    //kanei lock ton poro afou tha ton xrisimopoihsei to nhma
    pthread_mutex_lock(&cook_mutex);
    free_cooks++;
    pthread_cond_signal(&cook_cond);
    //kanei unlock ton poro afou teleiwse h leitourgia tou nhmatos
    pthread_mutex_unlock(&cook_mutex);

    //kanei lock ton poro afou tha ton xrisimopoihsei to nhma
    pthread_mutex_lock(&oven_mutex);
    free_ovens += num_pizzas;
    pthread_cond_broadcast(&oven_cond);
    //kanei unlock ton poro afou teleiwse h leitourgia tou nhmatos
    pthread_mutex_unlock(&oven_mutex);
    //kanei lock ton poro afou tha ton xrisimopoihsei to nhma
    pthread_mutex_lock(&delivery_mutex);
    //elegxei an uparxei diathesimos dianomeas
    while (free_delivery_boys <= 0) {
        pthread_cond_wait(&delivery_cond, &delivery_mutex);
    }
    free_delivery_boys--;
    //kanei unlock ton poro afou teleiwse h leitourgia tou nhmatos
    pthread_mutex_unlock(&delivery_mutex);

    sleep(Tpack * num_pizzas);
    double pack_time = get_global_time();
    safePrint("Order %d Packed [Time: %.2f minutes]\n", order_number, pack_time);
    
    int travel_time = (rand() % (Tdelhigh - Tdellow + 1)) + Tdellow;
    double delivery_ride_start_time = get_global_time();
    sleep(travel_time);  // Simulate delivery time

    double delivery_time = get_global_time();
    safePrint("Order %d delivered [Time: %.2f minutes]\n", order_number, delivery_time);
    double coolingTime = delivery_time - bake_time;
    serviceTime = delivery_time - start_time;
    double waitingTime = delivery_time - start_time;
    pthread_mutex_lock(&maxServiceTime_mutex);
    if (maxServiceTime < serviceTime) {
        maxServiceTime = serviceTime;
        maxServiceTimeOrderID = order_number;
    }
    //kanei unlock ton poro afou teleiwse h leitourgia tou nhmatos
    pthread_mutex_unlock(&maxServiceTime_mutex);
    //kanei lock ton poro afou tha ton xrisimopoihsei to nhma
    pthread_mutex_lock(&waiting_time_mutex);
    total_waiting_time += waitingTime;
    //kanei unlock ton poro afou teleiwse h leitourgia tou nhmatos
    pthread_mutex_unlock(&waiting_time_mutex);
    //kanei lock ton poro afou tha ton xrisimopoihsei to nhma
    pthread_mutex_lock(&cooling_time_mutex);
    total_cooling_time += coolingTime;
    successful_deliveries++;
    if (maxCoolingTime < coolingTime) {
        maxCoolingTime = coolingTime;
        maxCoolingTimeOrderID = order_number;
    }
    //kanei unlock ton poro afou teleiwse h leitourgia tou nhmatos
    pthread_mutex_unlock(&cooling_time_mutex);

    sleep(travel_time);
    
    
    double return_time = get_global_time();
    
    double delivery_ride_end_time = get_global_time();
    double delivery_ride_time = delivery_ride_end_time - delivery_ride_start_time;

    safePrint("Delivery boy returned from Order %d [Time: %.2f minutes]\n", order_number, return_time);
    
    pthread_mutex_lock(&delivery_mutex);
    free_delivery_boys++;
    pthread_cond_signal(&delivery_cond);
    pthread_mutex_unlock(&delivery_mutex);

    pthread_mutex_lock(&delivery_time_mutex);
    total_delivery_ride_time += delivery_ride_time;
    if (max_delivery_ride_time < delivery_ride_time) {
        max_delivery_ride_time = delivery_ride_time;
        max_delivery_ride_order_id = order_number;
    }
    //kanei unlock ton poro afou teleiwse h leitourgia tou nhmatos
    pthread_mutex_unlock(&delivery_time_mutex);

    
    return NULL;
}

void printRevenue() {
    printf("\n");
    printf("###############################################################################\n");
    printf("|                              Revenue Report                                \n");
    printf("###############################################################################\n");
    printf("| Total Calls           | %5d                                              \n", order_count);
    printf("| Successful Orders     | %5d                                              \n", successful_orders);
    printf("| Failed Orders         | %5d                                              \n", (order_count - successful_orders));
    printf("-----------------------------------------------------------------------------\n");
    printf("| Item                  | Quantity Sold          | Price       | Subtotal      \n");
    printf("-----------------------------------------------------------------------------\n");
    printf("|                                               \n");
    printf("| Margaritas            | %5d                  | 10€         | %7.2f€      \n", totalMargaritas, totalMargaritas * 10.0);
    printf("| Pepperonis            | %5d                  | 11€         | %7.2f€      \n", totalPeperonis, totalPeperonis * 11.0);
    printf("| Specials              | %5d                  | 12€         | %7.2f€      \n", totalSpecials, totalSpecials * 12.0);
    printf("==============================================================================\n");
    printf("| Total Revenue         | %7.2f€               \n", total_revenue);
    printf("|                                               \n");
    printf("\n");
    printf("------------------------------------------------------------------------------\n");
    printf("| Metric                | Value                 \n");
    printf("------------------------------------------------------------------------------\n");
    printf("| Max CallWait Time     | %7.2f minutes (ID: %d)\n", max_calling_waiting_time, max_calling_waiting_time_order_id);
    printf("| Max Service Time      | %7.2f minutes (ID: %d)\n", maxServiceTime, maxServiceTimeOrderID);
    printf("| Max Cooling Time      | %7.2f minutes (ID: %d)\n", maxCoolingTime, maxCoolingTimeOrderID);
    printf("| Max Delivery Ride     | %7.2f minutes (ID: %d)\n", max_delivery_ride_time, max_delivery_ride_order_id);
    if (successful_orders > 0) {
        double average_calling_waiting_time = total_calling_waiting_time / successful_orders;
        printf("| Avg Call Waiting Time | %7.2f minutes     \n",average_calling_waiting_time);
        double average_waiting_time = total_waiting_time / successful_orders;
        printf("| Avg Waiting Time      | %7.2f minutes         \n", average_waiting_time);
    } else {
        printf("| Avg Call Time         |    NULL                  \n");
        printf("| Avg Waiting Time      |    NULL                   \n");
    }
    if (successful_deliveries > 0) {
        double average_cooling_time = total_cooling_time / successful_deliveries;
        double average_delivery_time = total_delivery_ride_time / successful_deliveries;
        printf("| Avg Cooling Time      | %7.2f minutes         \n", average_cooling_time);
        printf("| Avg Delivery Ride     | %7.2f minutes         \n", average_delivery_time);

    } else {
        printf("| Avg Cooling Time      |    NULL                  \n");
        printf("| Avg Delivery Ride Time     | NULL\n");
    }
    printf("---------------------------------------------------------------------------- \n");
}


int main(int argc, char *argv[]) {
    
    if (argc != 3) {
        printf("Please input 2 parameters: 1: Number of Customers 2: Seed\n");
        return 1;
    }

    int Ncust = atoi(argv[1]);
    unsigned int seed = (unsigned int)atoi(argv[2]);
    srand(seed);

    init();

    pthread_t customers[Ncust];
    int customer_ids[Ncust];

    for (int i = 0; i < Ncust; i++) {
        customer_ids[i] = i + 1;
        pthread_create(&customers[i], NULL, orderHandler, &customer_ids[i]);
    }

    for (int i = 0; i < Ncust; i++) {
        pthread_join(customers[i], NULL);
    }

    printRevenue();
    erase();

    return 0;
}

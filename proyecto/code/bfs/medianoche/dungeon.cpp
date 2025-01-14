#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <pthread.h>
#include <unistd.h>

using namespace std;

// Struct to hold thread data
struct ThreadData {
    pthread_mutex_t mutex;
    pthread_cond_t cond_var;
    queue<int> curr_frontier;
    queue<int> next_frontier;
    vector<bool> visited;
    bool done;
    map<int, vector<int>> adj_list;
    vector<int> *distance;
};

// Thread function
void *thread_func(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    while (true) {
        // Wait for signal
        pthread_mutex_lock(&data->mutex);
        while (data->curr_frontier.empty() && !data->done) {
            pthread_cond_wait(&data->cond_var, &data->mutex);
        }

        // Check if we're done
        if (data->done) {
            pthread_mutex_unlock(&data->mutex);
            break;
        }

        // Process the current frontier
        while (!data->curr_frontier.empty()) {
            int node = data->curr_frontier.front();
            data->curr_frontier.pop();

            // Check if we've already visited this node
            if (data->visited[node]) {
                continue;
            }

            // Mark the node as visited and update its distance
            data->visited[node] = true;
            (*data->distance)[node] = (*data->distance)[node] == -1 ? (*data->distance)[node] = 0 : (*data->distance)[node];

            // Add the neighbors to the next frontier
            for (int neighbor : data->adj_list[node]) {
                if (!data->visited[neighbor]) {
                    data->next_frontier.push(neighbor);
                }
            }
        }

        // Swap the frontiers
        swap(data->curr_frontier, data->next_frontier);

        // Signal that we're done
        pthread_mutex_unlock(&data->mutex);
    }

    return NULL;
}

// Parallel BFS function with frontiers
void parallel_bfs(map<int, vector<int>> adj_list, int start_node, int goal_node, vector<int> &distance, int num_threads) {
    // Initialize thread data
    vector<ThreadData> thread_data(num_threads);
    vector<pthread_t> threads(num_threads);
    vector<bool> visited(adj_list.size(), false);
    queue<int> curr_frontier;
    queue<int> next_frontier;
    bool done = false;

    // Initialize the distance vector
    distance = vector<int>(adj_list.size(), -1);

    // Add the starting node to the current frontier
    curr_frontier.push(start_node);

    // Initialize the thread data
    for (int i = 0; i < num_threads; i++) {
      thread_data[i].curr_frontier = curr_frontier;
      thread_data[i].visited = visited;
      thread_data[i].done = done;
      thread_data[i].adj_list = adj_list;
      thread_data[i].distance = &distance;
      pthread_mutex_init(&thread_data[i].mutex, NULL);
      pthread_cond_init(&thread_data[i].cond_var, NULL);
      pthread_create(&threads[i], NULL, thread_func, (void *)&thread_data[i]);
    }
    
    // Wait for the goal node to be reached
    while (!visited[goal_node]) {
      // Signal the threads
      pthread_mutex_lock(&thread_data[0].mutex);
      pthread_cond_broadcast(&thread_data[0].cond_var);
      pthread_mutex_unlock(&thread_data[0].mutex);
          // Wait for a bit to allow the threads to work
    usleep(1000);
}

// Set the done flag and signal the threads to exit
done = true;
for (int i = 0; i < num_threads; i++) {
    pthread_mutex_lock(&thread_data[i].mutex);
    thread_data[i].done = true;
    pthread_cond_broadcast(&thread_data[i].cond_var);
    pthread_mutex_unlock(&thread_data[i].mutex);
}

// Join the threads
for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
}
}

int main() {
// Example adjacency list
map<int, vector<int>> adj_list = {
{0, {1, 2}},
{1, {0, 3, 4}},
{2, {0, 4}},
{3, {1}},
{4, {1, 2}}
};

 // Run parallel BFS with 2 threads
int start_node = 0;
int goal_node = 4;
vector<int> distance;
int num_threads = 2;
parallel_bfs(adj_list, start_node, goal_node, distance, num_threads);

// Print the distance to each node
for (int i = 0; i < distance.size(); i++) {
    cout << "Distance to node " << i << ": " << distance[i] << endl;
}

return 0;
}

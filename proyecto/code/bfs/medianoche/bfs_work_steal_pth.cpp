#include <iostream>
#include <queue>
#include <vector>
#include <pthread.h>

using namespace std;

// Define a struct to represent a range of nodes
struct NodeRange {
    int start;
    int end;
};

// Define a struct to hold thread-specific data
struct ThreadData {
    vector<vector<int>>* adj_list;
    vector<bool>* visited;
    vector<int>* distance;
    queue<int>* work_queue;
    pthread_mutex_t* queue_mutex;
    pthread_barrier_t* barrier;
    int num_threads;
    int thread_id;
};

// Define a function to perform BFS on a range of nodes
void bfs_range(NodeRange range, ThreadData* data) {
    queue<int>* q = data->work_queue;
    vector<bool>* visited = data->visited;
    vector<int>* distance = data->distance;
    vector<vector<int>>* adj_list = data->adj_list;

    while (true) {
        int curr_node = -1;

        // Try to get work from own queue
        pthread_mutex_lock(data->queue_mutex);
        if (!q->empty()) {
            curr_node = q->front();
            q->pop();
        }
        pthread_mutex_unlock(data->queue_mutex);

        // If own queue is empty, try to steal work from other threads
        if (curr_node == -1) {
            for (int i = 0; i < data->num_threads; i++) {
                // Compute the index of the queue to steal from
                int idx = (data->thread_id + i) % data->num_threads;

                pthread_mutex_lock(&data[idx].queue_mutex);
                if (!data[idx].work_queue->empty()) {
                    curr_node = data[idx].work_queue->front();
                    data[idx].work_queue->pop();
                }

		pthread_mutex_unlock(&data[idx].queue_mutex);

                if (curr_node != -1) {
                    break;
                }
            }
        }

        if (curr_node == -1) {
            // No more work to do, exit loop
            break;
        }

        // Process the current node
        cout << "visited node " << curr_node << " at thread " << data->thread_id << endl;

        // Visit neighbors
        for (int i = 0; i < (*adj_list)[curr_node].size(); i++) {
            int neighbor = (*adj_list)[curr_node][i];
            if (!(*visited)[neighbor]) {
                (*visited)[neighbor] = true;
                (*distance)[neighbor] = (*distance)[curr_node] + 1;
                q->push(neighbor);
            }
        }
    }

    // Wait for all threads to finish before returning
    pthread_barrier_wait(data->barrier);
}

int main() {
  int num_threads = 4;
  
  // Define the adjacency list for the graph
  vector<vector<int>> adj_list {
				{1, 2}, // Node 0
				{0, 2, 3}, // Node 1
				{0, 1, 4}, // Node 2
				{1}, // Node 3
				{2, 5}, // Node 4
				{4} // Node 5
  };
  
  int num_nodes = adj_list.size();
  vector<bool> visited(num_nodes, false);
  
  vector<int> distance(num_nodes, -1);
  queue<int> work_queue;
  
  // Initialize the work queue with the start node
  int start_node = 0;
  visited[start_node] = true;
  distance[start_node] = 0;
  work_queue.push(start_node);
  
  // Initialize thread data
  vector<ThreadData> thread_data(num_threads);
  pthread_mutex_t queue_mutex;
  pthread_mutex_init(&queue_mutex, NULL);
  pthread_barrier_t barrier;
  pthread_barrier_init(&barrier, NULL, num_threads);
  
  for (int i = 0; i < num_threads; i++) {
    thread_data[i].adj_list = &adj_list;
    thread_data[i].visited = &visited;
    thread_data[i].distance = &distance;
    thread_data[i].work_queue = &work_queue;
    thread_data[i].queue_mutex = &queue_mutex;
    thread_data[i].barrier = &barrier;
    thread_data[i].num_threads = num_threads;
    thread_data[i].thread_id = i;
  }
  
  // Create threads and assign them ranges of nodes to process
  pthread_t threads[num_threads];
  for (int i = 0; i < num_threads; i++) {
    NodeRange range {
		     i * num_nodes / num_threads,
		     (i + 1) * num_nodes / num_threads
    };
    pthread_create(&threads[i], NULL, (void*(*)(void*))bfs_range, &thread_data[i]);
  }
  
  // Wait for threads to finish
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }
  
  // Print the distances to each node
  for (int i = 0; i < num_nodes; i++) {
    cout << "Distance to node " << i << " is " << distance[i] << endl;
  }
  
  return 0;
  
}

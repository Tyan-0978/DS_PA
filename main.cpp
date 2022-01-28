// Programming homework from data structure

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

// nodes in min heap
class Node {
  public:
    vector<int> position;
    int key;

    // constructor
    Node(int i, int j, int k) {
      position = {i,j};
      key = k;
    }
};

// binary min heap implemented by a vector
class MinHeap {
  public:
    vector<Node> heap;
    int heap_size;

    // constructor
    MinHeap() {
      heap = {};
      heap_size = 0;
    };

    // heap operations
    // insert
    void ins(Node new_data) {
      heap.push_back(new_data);
      int new_data_index = heap_size;
      int new_data_parent = (new_data_index - 1) / 2;
      // compare and swap new data upwards
      while (new_data_index != 0 && heap[new_data_index].key < heap[new_data_parent].key) {
        swap(heap[new_data_index], heap[new_data_parent]);
	new_data_index = new_data_parent;
	new_data_parent = (new_data_index - 1) / 2;
      }
      heap_size++;
    }

    // extract min
    Node extract_min() {
      Node min = heap[0];
      swap(heap[0], heap[heap_size-1]);
      heap.pop_back();
      heap_size--;
      min_heapify(0);
      return min;
    }

    // decrease
    /*
    void decrease(int i, int j, int old_key, int new_key) {
      int target = find(i, j, old_key);
      heap[target].key = new_key;

      // compare and swap new data upwards
      int target_parent = (target - 1) / 2
      while (index != 0 && heap[target].key < heap[target_parent].key) {
        swap(heap[target], heap[target_parent]);
	target = target_parent;
	target_parent = (target - 1) / 2;
      }
    }
    */

  private:
    void min_heapify(int root) {
      int left_child = 2 * root + 1;
      int right_child = 2 * root + 2;
      int min = root;
      if (left_child < heap_size && heap[left_child].key < heap[min].key) {
        min = left_child;
      }
      if (right_child < heap_size && heap[right_child].key < heap[min].key) {
        min = right_child;
      }
      if (min != root) {
        swap(heap[root], heap[min]);
	min_heapify(min);
      }
    }
};

int main(int argc, char* argv[]) {
  if (argc != 3) {
    cout << "two arguments <input_file>, <output_file> are required" << endl;
    return 0;
  }


  // read input file
  ifstream input_file(argv[1]);
  int size;
  input_file >> size;

  // construct grid table
  vector<vector<int>> grid(size, vector<int>(size));
  int i = 0, j = 0;
  int num;
  while (input_file >> num) {
    grid[i][j] = num;
    j++;
    if (j == size) {
      j = 0;
      i++;
    }
  }

  // shortest path table, initialized with -1
  vector<vector<int>> shortest_path(size, vector<int>(size, -1));

  // grid status table, initialized with 0
  // 0: not discovered
  // 1: discovered but not traversed (in min heap)
  // 2: discovered and traversed (extracted from min heap)
  vector<vector<int>> grid_status(size, vector<int>(size, 0));

  // shortest path algorithm
  // initialize openlist
  MinHeap openlist;

  // start at [0,0]
  shortest_path[0][0] = grid[0][0];
  grid_status[0][0] = 2;

  // [0,1], [1,0]
  shortest_path[0][1] = shortest_path[0][0] + grid[0][1];
  grid_status[0][1] = 1;
  openlist.ins( Node(0, 1, shortest_path[0][1]) );

  shortest_path[1][0] = shortest_path[0][0] + grid[1][0];
  grid_status[1][0] = 1;
  openlist.ins( Node(1, 0, shortest_path[1][0]) );

  // find shortest path
  int x, y, k;
  while (grid_status[size-1][size-1] == 0) { // path does not reach exit
    // choose the shortest path
    Node min_grid = openlist.extract_min();
    x = min_grid.position[0];
    y = min_grid.position[1];
    k = min_grid.key;
    if (grid_status[x][y] == 2) { // duplicate node in min heap
      continue;
    }
    grid_status[x][y] = 2;

    // add new paths into heap
    // up
    if (x != 0 && grid_status[x-1][y] != 2) {
      if (grid_status[x-1][y] == 0) {
        shortest_path[x-1][y] = shortest_path[x][y] + grid[x-1][y];
        grid_status[x-1][y] = 1;
	openlist.ins( Node(x-1, y, shortest_path[x-1][y]) );
      } else if (grid_status[x-1][y] == 1) {
        if (shortest_path[x][y] + grid[x-1][y] < shortest_path[x-1][y]) {
          shortest_path[x-1][y] = shortest_path[x][y] + grid[x-1][y];
	  openlist.ins( Node(x-1, y, shortest_path[x-1][y]) );
	}
      }
    }
    // down
    if (x != size-1 && grid_status[x+1][y] != 2) {
      if (grid_status[x+1][y] == 0) {
        shortest_path[x+1][y] = shortest_path[x][y] + grid[x+1][y];
        grid_status[x+1][y] = 1;
	openlist.ins( Node(x+1, y, shortest_path[x+1][y]) );
      } else if (grid_status[x+1][y] == 1) {
        if (shortest_path[x][y] + grid[x+1][y] < shortest_path[x+1][y]) {
          shortest_path[x+1][y] = shortest_path[x][y] + grid[x+1][y];
	  openlist.ins( Node(x+1, y, shortest_path[x+1][y]) );
	}
      }
    }
    // left
    if (y != 0 && grid_status[x][y-1] != 2) {
      if (grid_status[x][y-1] == 0) {
        shortest_path[x][y-1] = shortest_path[x][y] + grid[x][y-1];
        grid_status[x][y-1] = 1;
	openlist.ins( Node(x, y-1, shortest_path[x][y-1]) );
      } else if (grid_status[x][y-1] == 1) {
        if (shortest_path[x][y] + grid[x][y-1] < shortest_path[x][y-1]) {
          shortest_path[x][y-1] = shortest_path[x][y] + grid[x][y-1];
	  openlist.ins( Node(x, y-1, shortest_path[x][y-1]) );
	}
      }
    }
    // right
    if (y != size-1 && grid_status[x][y+1] != 2) {
      if (grid_status[x][y+1] == 0) {
        shortest_path[x][y+1] = shortest_path[x][y] + grid[x][y+1];
        grid_status[x][y+1] = 1;
	openlist.ins( Node(x, y+1, shortest_path[x][y+1]) );
      } else if (grid_status[x][y+1] == 1) {
        if (shortest_path[x][y] + grid[x][y+1] < shortest_path[x][y+1]) {
          shortest_path[x][y+1] = shortest_path[x][y] + grid[x][y+1];
	  openlist.ins( Node(x, y+1, shortest_path[x][y+1]) );
	}
      }
    }
  }

  // track the shortest path
  vector<char> steps = {};
  x = size - 1;
  y = size - 1;
  while (x != 0 || y != 0) {
    // check four direction
    // up
    if (x != 0 && shortest_path[x][y] == shortest_path[x-1][y] + grid[x][y]) {
      x--;
      steps.insert(steps.begin(), 'D');
    }
    // down
    if (x != size-1 && shortest_path[x][y] == shortest_path[x+1][y] + grid[x][y]) {
      x++;
      steps.insert(steps.begin(), 'U');
    }
    // left
    if (y != 0 && shortest_path[x][y] == shortest_path[x][y-1] + grid[x][y]) {
      y--;
      steps.insert(steps.begin(), 'R');
    }
    // right
    if (y != size-1 && shortest_path[x][y] == shortest_path[x][y+1] + grid[x][y]) {
      y++;
      steps.insert(steps.begin(), 'L');
    }
    //cout << shortest_path[x][y] << endl;
  }

  // write output file
  ofstream output_file(argv[2]);
  output_file << shortest_path[size-1][size-1] << " "
              << steps.size() + 1 << endl;
  for (i = 0; i < steps.size(); i++) {
    output_file << steps[i];
  }
  output_file.close();

  return 0;
}






































// end

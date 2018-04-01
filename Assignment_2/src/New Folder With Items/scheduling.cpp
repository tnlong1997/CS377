#include <string>
#include <fstream>
#include <iostream>
#include <queue>
#include <list>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <cmath>
#include <climits>

#include "scheduling.h"
#define DELIM " \t\r\n\a"


using namespace std;

pqueue_arrival read_workload(string filename)
{
  pqueue_arrival workload;
  ifstream f(filename);
  string buffer;
  int temp;
  if (f) {
    Process temp_proc;
    while (getline(f, buffer)) {
      stringstream ss(buffer);
      ss >> temp;
      temp_proc.arrival = temp;
      ss >> temp;
      temp_proc.duration = temp;
      workload.push(temp_proc);
    }
  }
  f.close();
  return workload;
}

void show_workload(pqueue_arrival workload)
{
  pqueue_arrival xs = workload;
  cout << "Workload:" << endl;
  while (!xs.empty())
  {
    Process p = xs.top();
    cout << '\t' << p.arrival << ' ' << p.duration << endl;
    xs.pop();
  }
}

void show_processes(list<Process> processes)
{
  list<Process> xs = processes;
  cout << "Processes:" << endl;
  while (!xs.empty())
  {
    Process p = xs.front();
    cout << "\tarrival=" << p.arrival << ", duration=" << p.duration
         << ", first_run=" << p.first_run << ", completion=" << p.completion << endl;
    xs.pop_front();
  }
}

list<Process> fifo(pqueue_arrival workload)
{
  list<Process> complete;
  int temp = 0;
  pqueue_arrival temp_proc = workload;
  while (!temp_proc.empty()) {
    Process p = temp_proc.top();
    p.first_run = temp;
    p.completion = p.first_run + p.duration;
    temp += p.duration;
    complete.push_back(p);
    temp_proc.pop();
  }
  return complete;
}

list<Process> sjf(pqueue_arrival workload)
{
  list<Process> complete;
  pqueue_arrival temp_arr = workload;
  pqueue_duration temp_dur;
  int start, wait, tat;

  while (!temp_arr.empty()) {
    Process p = temp_arr.top();
    temp_dur.push(p);
    temp_arr.pop();
  }

  start = temp_dur.top().arrival;
  wait = 0;
  while (!temp_dur.empty()) {
    Process p = temp_dur.top();
    tat = p.duration + wait;
    p.first_run = start + wait;
    p.completion = tat + p.arrival;
    complete.push_back(p);
    wait += p.duration;
    temp_dur.pop();
  }
  return complete;
}

list<Process> stcf(pqueue_arrival workload)
{
  list<Process> complete;
  pqueue_arrival temp_arr = workload;
  int run[temp_arr.size()], wait[temp_arr.size()], first[temp_arr.size()];
  int index, index2, num, com_check, t, min, shortest, com_time;
  bool check;
  Process p;
  vector<Process> p_vector;

  index = 0;
  while (!temp_arr.empty()) {
    p = temp_arr.top();
    run[index] = p.duration;
    p.first_run = -1;
    first[index] = -1;
    p_vector.push_back(p);
    temp_arr.pop();
    index++;
  }

  num = index;
  com_check = 0;
  t = 0;
  min = INT_MAX;
  shortest = 0;
  check = false;

  while (com_check < num) {
    pqueue_arrival temp = workload;
    index2 = 0;
    while (index2 < num) {
      Process temp_p = temp.top();
      temp.pop();
      if ((temp_p.arrival <= t) && (run[index2] < min) && (run[index2] > 0)) {
        min = run[index2];
        shortest = index2;
        check = true;
      }
      index2++;
    }
    if (first[shortest] == -1) {
      first[shortest] = t;
    }
    if (!check) {
      t++;
      continue;
    }
    run[shortest] = run[shortest] - 1;

    min = run[shortest];
    if (min == 0) {
      min = INT_MAX;
    }

    if (run[shortest] == 0) {
      com_check++;
      com_time = t + 1;
      p = p_vector.at(shortest);
      p.completion = com_time;
      wait[shortest] = com_time - p.duration - p.arrival;

      if (wait[shortest] < 0) wait[shortest] = 0;
      p.first_run = first[shortest];
      complete.push_back(p);
    }
    t++;
  }
  return complete;
}

list<Process> rr(pqueue_arrival workload)
{
  list<Process> complete;
  vector<Process> p_vector;
  int size, index, t, quantum;
  size = workload.size();
  int c_d[size];
  pqueue_arrival temp_arr = workload;
  Process p;
  bool done;

  index = 0;
  while (!temp_arr.empty()) {
    p = temp_arr.top();
    temp_arr.pop();
    c_d[index] = p.duration;
    p.first_run = -1;
    p_vector.push_back(p);
    index++;
  }

  t = 0;
  quantum =  1;
  while (true) {
    done = true;
    for (int i = 0; i < size; i++) {
      if (c_d[i] > 0) {
        p = p_vector.at(i);
        if (p.first_run == -1) p.first_run = i;
        done = false;
        if (c_d[i] > quantum) {
          t += quantum;
          c_d[i] -= quantum;
        } else {
          t += c_d[i];
          p.completion = t;
          complete.push_back(p);
          c_d[i] = 0;
        }
      }
    }
  }
  return complete;
}

float avg_turnaround(list<Process> processes)
{
  return 0;
}

float avg_response(list<Process> processes)
{
  return 0;
}

void show_metrics(list<Process> processes)
{
  float avg_t = avg_turnaround(processes);
  float avg_r = avg_response(processes);
  show_processes(processes);
  cout << '\n';
  cout << "Average Turnaround Time: " << avg_turnaround(processes) << endl;
  cout << "Average Response Time:   " << avg_response(processes) << endl;
}

#ifndef QUEUE_H
#define QUEUE_H

// Original from Chris Martin

#define NULL 0

struct QueueItem {
  int val;
  QueueItem *next;
};

class Queue {
public:
  QueueItem *head;
  QueueItem *tail;

  Queue();
  ~Queue();

  bool empty();
  int size();
  int front();
  void push_back(int item);
  void pop_front();

private:
  int size_;
};

#endif

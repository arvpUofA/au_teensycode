// Original from Chris Martin

#include "queue.h"

// Ctor
Queue::Queue() {
  head = NULL;
  tail = NULL;
  size_ = 0;
}

// Dtor
Queue::~Queue() {
  while (!empty())
    pop_front();
}

int Queue::size() {
  return size_;
}

bool Queue::empty() {
  return head == NULL;
}

int Queue::front() {
  return head->val;
}

void Queue::push_back(int item) {
  QueueItem *tmp = new QueueItem; // New **struct**
  tmp->val = item;	// No ctor.  Init here.
  tmp->next = NULL;

  if (head == NULL) {	// Special case
    head = tmp;		// First object in queue
  }
  else {
    tail->next = tmp;	// Link in at tail
  }
  tail = tmp;	// New object is tail
  size_++;
}

void Queue::pop_front() {
  if (head == NULL) return;

  QueueItem *tmp = head;
  head = tmp->next;	// Unlink head
  delete tmp;
  
  if (head == NULL)
    tail = NULL;

  size_--;
}

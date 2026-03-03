# Task Management System (Linked List + Stack + Queue) — COMP2421 Project
 
The program loads tasks from a file (`tasks.txt`) into a **linked list**, allows the user to **perform tasks** (move them to a **stack**), **undo** the most recently performed task (restore it to the end of the linked list and track it in a **queue**), and generates a full summary report (`Report.txt`).

This project was implemented using **only Linked Lists, Stacks, and Queues** as required.

---

## Features

### ✅ Core Operations
- **Load Tasks File**: Read tasks from `tasks.txt` and store them in a linked list.
- **Add a New Task**: Insert a new task at the end of the list (unique ID enforced across list + performed stack).
- **Delete a Task**: Delete a task by ID from the linked list.
- **Search for a Task**:
  - by **Task ID**
  - by **Task Name**
- **View Unperformed Tasks**: Print all tasks currently in the linked list.
  - Tasks restored by undo are marked as `(undone)` in the list.

### ✅ Task Execution + Undo
- **Perform a Task**: Remove a task from the linked list and push it onto the performed stack.
- **Undo Last Performed Task**:
  - Pop from performed stack
  - Re-insert at the end of the linked list (marked as undone)
  - Enqueue a copy into an "undone history" queue (tracks all undo actions)

### ✅ Reporting
- **Generate Summary Report** (`Report.txt`) containing:
  - Unperformed tasks
  - Undone tasks (history of undo operations)
  - Performed tasks

---

## Data Structures Used

- **Linked List**: stores current unperformed tasks dynamically.
- **Stack**: stores performed tasks (LIFO) so the most recent task can be undone easily.
- **Queue**: stores undone tasks history (FIFO) to keep track of undo actions from first to last.

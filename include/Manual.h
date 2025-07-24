#ifndef manuel_h
#define manuel_h
#include <iostream>
#include <string>
inline std::string printManual() {
	return R"(Taskd CLI Commands List

1. taskd task add <title> [--desc <description>] [--priority <level>] [--due <date>]

    Create a new task with:

        Required: <title>
        Optional:
            --desc      Task description
            --priority  low | medium | high (default: medium)
            --due       Due date (e.g., 2025-07-16)

2. taskd task list [--all] [--status <status>] [--assigned <user>] [--priority <level>]

    List tasks with optional filters.

3. taskd task show <task_id>

    Show task details.

4. taskd task update <task_id> [--title <title>] [--desc <description>] [--status <status>] [--priority <level>] [--due <date>]

    Update fields of a task.

5. taskd task assign <task_id> <user>

    Assign a task.

6. taskd task close <task_id>

    Mark task as completed.

7. taskd task reopen <task_id>

    Reopen a closed task.

8. taskd task delete <task_id>

    Permanently delete a task.

Use: taskd help  — to see this list.
)";
}





#endif

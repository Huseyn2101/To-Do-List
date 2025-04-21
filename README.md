# Task Manager Application

The **Task Manager Application** is a comprehensive project designed to help users manage tasks efficiently. It includes features such as user authentication, task creation, filtering, and priority-based sorting. The application uses `NuGet` and external libraries like `nlohmann.json` for JSON handling.

---

## Key Features

- **User Authentication**: Secure login system with password hashing.
- **Task Management**:
  - Create, update, and remove tasks.
  - Filter tasks by priority or date range.
  - Sort tasks by priority and start time.
- **JSON-Based Persistence**: Save and load user data with tasks using a JSON file.
- **Error Handling**: Robust error handling for file operations and user input.
- **Multi-User Support**: Manage tasks for multiple users in a single application.

---

## Built With

- **C++17 or later**
- **NuGet Package Manager**
- **nlohmann.json** (for JSON parsing and serialization)

---

## Prerequisites

Before running the application, ensure you have the following installed:

- **Visual Studio 2019 or later**
- **NuGet Package Manager** (built into Visual Studio)
- **C++ Development Tools**
- `nlohmann.json` library (installed via NuGet)

---

## Installation Guide

Follow these instructions to set up the project in Visual Studio:

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/your-username/task-manager-app.git
   cd task-manager-app
   ```

2. **Open the Project in Visual Studio**:
   - Launch Visual Studio.
   - Open the `.sln` file in the repository.

3. **Install Dependencies**:
   - Open the `NuGet Package Manager`:
     - Go to **Tools > NuGet Package Manager > Manage NuGet Packages for Solution**.
   - Search for `nlohmann.json` and install version `3.11.3`.

4. **Build the Project**:
   - Set the build configuration to `Debug` or `Release`.
   - Click **Build > Build Solution** or press `Ctrl+Shift+B`.

---

## Usage Instructions

1. **Run the Application**:
   - Press `F5` or click **Start Debugging** in Visual Studio.

2. **Main Menu**:
   - **1. Register New User**: Create a new user account.
   - **2. Login**: Log in with your credentials to access tasks.
   - **3. Exit**: Exit the application.

3. **User Menu** (after login):
   - **1. Add Task**: Enter task details like name, description, start and end times, and priority.
   - **2. Remove Task**: Delete a task by its index.
   - **3. Update Task**: Modify an existing task.
   - **4. Display All Tasks**: View all tasks sorted by priority and start time.
   - **5. Filter Tasks by Priority**: Show tasks with a specific priority level.
   - **6. Filter Tasks by Date Range**: Show tasks within a specified date range.
   - **7. Logout**: Return to the main menu.

---

## Example Code Snippet

Below is an example of a simple task creation in this application:

```cpp
#include <nlohmann/json.hpp>
#include "UserManager.h"

int main() {
    UserManager userManager("users.json");

    User newUser("john_doe", "secure_password");
    userManager.addUser(newUser);

    Task newTask("Complete Project", "Finish the task manager project for submission",
                 Priority::HIGH, DateTime(2025, 4, 20, 10, 0, 0), DateTime(2025, 4, 20, 18, 0, 0));
    userManager.addTaskForUser("john_doe", newTask);

    newUser.displayTasks();

    return 0;
}
```

---

## Known Issues

- **Error on Missing Dependencies**: Ensure `nlohmann.json` is installed correctly via NuGet.
- **File Handling Exceptions**: If the `users.json` file is missing or corrupted, the application will generate a new file.

---

## Contributing

Contributions are welcome! Please follow these steps to contribute:

1. Fork the repository.
2. Create a new branch for your feature or bugfix:
   ```bash
   git checkout -b feature-name
   ```
3. Commit your changes:
   ```bash
   git commit -m "Add new feature"
   ```
4. Push the branch to your fork:
   ```bash
   git push origin feature-name
   ```
5. Open a pull request on the main repository.

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

## Contact

For inquiries or support, feel free to contact the project maintainer:

- **GitHub**: [Huseyn2101](https://github.com/Huseyn2101)
- **Email**: your-email@example.com

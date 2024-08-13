# Contact Management System

This is a C++ project (cli) for managing contacts, including personal and business contacts.

## Prerequisites

- C++ compiler supporting C++11 or later (e.g., GCC, Clang)
- CMake 3.10 or higher
- Git

## Building the Project

1. Clone the repository:
git clone https://github.com/yourusername/CONTACTMANAGEMENT_CPP.git
cd CONTACTMANAGEMENT_CPP

2. Create a build directory:
mkdir build
cd build

3. Generate the build files with CMake:
cmake ..

4. Build the project:
- On Windows with MinGW:
  ```
  mingw32-make
  ```
- On Linux or macOS:
  ```
  make
  ```

5. The executable will be created in the `build` directory.

### Using Visual Studio Code

1. Open the project folder in Visual Studio Code.

2. Press `Ctrl+Shift+P` (or `Cmd+Shift+P` on macOS) to open the Command Palette.

3. Type "Tasks: Run Build Task" and select it.

4. Choose the "make" task.

5. The project will be built, and the executable will be created in the `build` directory.

## Running the Program

After building, you can run the program:

- From the command line, navigate to the `build` directory and run:
./ContactManagement

(On Windows, use `ContactManagement.exe`)

- In Visual Studio Code, you can use the provided launch configuration to run and debug the program.

## Troubleshooting

- If CMake fails to find your compiler, ensure your compiler is installed correctly and its path is added to your system's PATH environment variable.

- If you encounter any "file not found" errors during compilation, double-check that all source files are in the correct directories as specified in the project structure.

- For Visual Studio Code users, ensure you have the "C/C++" and "CMake Tools" extensions installed for the best development experience.

## Additional Notes

- This project uses C++17 features. If you're using an older compiler, you may need to update it or modify the code to be compatible with your compiler version.

- The CMakeLists.txt file is configured for this project structure. If you add new source files or change the directory structure, remember to update CMakeLists.txt accordingly.

- For any further questions or issues, please open an issue on the project's GitHub repository.

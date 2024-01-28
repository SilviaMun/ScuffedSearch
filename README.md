# Scuffed Search

Welcome to Scuffed Search, the tool that turns your data breach nightmares into a walk in the digital park. Born out of necessity in the quirky world of Windows 11, where shared folders and VirtualBox don't always see eye to eye, Scuffed Search is here to save the day!

## Why Scuffed Search?

In the labyrinth of Windows 11, where setting up a shared folder in VirtualBox feels like trying to solve a Rubik's cube blindfolded, Scuffed Search emerges as a beacon of hope. Designed for those moments when you find yourself staring at a massive directory of data breaches on your Windows system, wondering, "How do I search through all this efficiently?" Scuffed Search is your answer.

## Features

- **Windows 11 Friendly**: Tailor-made for Windows environments, especially when VirtualBox gives you the cold shoulder.
- **Data Breach Specialist**: Whether it's a small leak or an ocean of data, Scuffed Search dives deep to find what you need.
- **VS Code Integration**: Runs directly in your Visual Studio Code terminal, making it a seamless part of your workflow.
- **Multithreaded Efficiency**: Harnesses the power of multithreading to make searches faster than a speeding bullet.
- **CSV Output**: Neatly prints the results in a CSV file, because organization is key.

## Getting Started

### Prerequisites

- A sense of adventure
- A Windows 11 system
- Visual Studio Code
- TDM-GCC Compiler
- C/C++ Compile Run extension for VS Code

### Installation

1. **Download the Project**
   - Visit the [Scuffed Search GitHub repository](https://github.com/SilviaMun/ScuffedSearch).
   - Click on the `Code` button and select `Download ZIP`.
   - Extract the ZIP file to your desired location.

2. **Open the Project in Visual Studio Code**
   - Navigate to the extracted directory.
   - Right-click and select "Open with Visual Studio Code".

3. **Install TDM-GCC Compiler**
   - Download TDM-GCC from [TDM-GCC's official site](https://jmeubank.github.io/tdm-gcc/).
   - Follow the installation instructions to set up the compiler on your system.

4. **Install C/C++ Compile Run Extension in VS Code**
   - In VS Code, go to the Extensions view by clicking on the square icon on the sidebar.
   - Search for "C/C++ Compile Run" and install the extension.

5. **Compile the Program**
   - Open the source file in VS Code.
   - Use the shortcut (`Ctrl+Alt+C` to compile and `Ctrl+Alt+R` to run) or right-click and select "Compile & Run" from the context menu.
   - A new folder will be created in your project directory containing the compiled program.

### Important Note

Due to the nature of file system operations performed by Scuffed Search, some antivirus software may flag the compiled executable. To ensure Scuffed Search runs smoothly, you may need to:

- **Sign the Executable**: If you have a code signing certificate, sign the compiled executable to avoid any antivirus warnings.
- **Disable Antivirus Temporarily**: If signing is not an option, you may need to temporarily disable your antivirus software while running Scuffed Search.

### Usage

1. Open the VS Code terminal.
2. Run the compiled program from the newly created folder.
3. Enter the directory path and the search string as prompted.
4. The CSV file with the search results will be created in the same folder as the compiled program.

## Contributing

Got an idea to make Scuffed Search even more scuffed (or less)? Fork the repo, make your changes, and submit a pull request. Let's make searching fun!

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.


---

*Scuffed Search: It's not just a search tool, it's an adventure.*

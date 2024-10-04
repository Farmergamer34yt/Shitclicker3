# Clicker Game

Welcome to the Clicker Game! This simple Windows application allows users to accumulate clicks and purchase auto-clickers that automatically generate clicks for you over time.

## Features

- **Click Button**: Increment your click count by pressing a button.
- **Auto-Clickers**: Purchase auto-clickers that automatically add clicks every second.
- **Dynamic UI**: The user interface resizes and updates dynamically as you click and purchase auto-clickers.
- **Cost Scaling**: The cost of auto-clickers increases with each purchase, providing a scaling challenge.

## Prerequisites

- Windows Operating System
- Visual Studio or any C compiler that supports Windows API

## Getting Started

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/yourusername/clicker-game.git
   cd clicker-game
Compile the Code: Open the project in Visual Studio or any compatible IDE, and build the project.

Run the Application: Execute the compiled binary to start playing the game.

## Controls
- Click Me Button: Click this button to increase your click count.
- Buy Auto-Clicker Button: Purchase an auto-clicker to automatically generate clicks.
## Code Overview
The code is written in C and utilizes the Windows API for creating the user interface and handling events. The main components of the code include:

- WinMain: The entry point of the application.
- WindowProcedure: Handles window messages and user interactions.
- UpdateClickText, UpdateAutoClickerText: Functions to update the display text for clicks and auto-clickers.
- StartAutoClickerTimer: Initializes the timer for auto-clickers.
- AutoClickerTick: Adds clicks based on the number of auto-clickers.
- ResizeControls: Dynamically resizes and repositions UI controls based on the window size.
- Contributions
- Feel free to fork the repository and submit pull requests for any enhancements or bug fixes.

## License
- his project is licensed under the MIT License. See the LICENSE file for details.

## Acknowledgments
Thanks to the open-source community for providing inspiration and resources for developing this project.
csharp
Copy code


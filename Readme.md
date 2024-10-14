# Startweek

## Project Overview
This project, developed for Avans, is designed to assist in the Startweek event through a game. The game features a route with integrated mini-games that are played along the way.

## Getting Started (For Developers)
This guide is intended for developers working on the project. Please note that the game requires a custom-made PCB to function properly. Follow the steps below to set up the development environment.

1. Make sure the folders are named as below or some setting may need changing later on.
   Keep in mind that some steps of the zephyr getting started may be a bit different because of the folder layout.
   
   * zephyrtoolchain
   * ├── zephyrproject
   * └── zephyr-sdk-0.16.5
3. Follow the Zephyr getting started before continuing: 
   * `https://docs.zephyrproject.org/latest/develop/getting_started/index.html` 
4. Clone the repository into the `zephyrproject` folder:
   * `git clone https://github.com/JaroWMR/Startweek.git`
5. Open Visual Studio Code (VSCode).
6. Press in the top left on file.
7. Open workspace from file.
8. Select Zephyr.code-workspace.
9. Download the recommended extensions for the project.
    (List of recommended extensions here)
10. Change the board in the `Zephyr.code-workspace` to the board that is used.
11. Use `ctrl + shift + b` and build the project.
12. Download Segger Ozone:
   * `https://www.segger.com/downloads/jlink/#Ozone`
11. Open Segger Ozone and press `File`, `Open` and select the `h7ConfigScriptOzone.jdebug`.
12. On the top select `Tools`, `J-link settings` and press the `...` to select the Jlink probe.
13. Then press `File` again and select `edit project file`.
14. In this configuration file change the following settings:
   * `Project.SetDevice ("STM32H743ZI");` Change board to correct board if not testing STM32H743ZI.
15. Press the bootpin button and hold it.
16. Press the hardwarereset button an release both buttons.
17. To start debugging press the `green drop down arrow` besides the `green power button` on the top left.
18. Select `Download and Reset Program`.
19. Debugging has been started tools to debug are in the top left.

## Additonal info
The branch named `EmptyEnvironment_DO_NOT_DELETE` offers the environment in its starting state.

## Dependencies
1. Segger Ozone `https://www.segger.com/downloads/jlink/#Ozone`
2. Jlink Software `https://www.segger.com/downloads/jlink/`


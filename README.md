# RoboCup 2019 SPL Directional Whistle Challenge Tester

This is the testing application for the Directional Whistle Challenge that will be held at RoboCup 2019 in the soccer SPL. For more details about the Directional Whistle Challenge, please refer to the [RoboCup 2019 SPL Technical Challenge Rules](https://spl.robocup.org/wp-content/uploads/downloads/Challenges2019.pdf).

## Compilation

This program depends on CMake, C++11 and Qt5 (the core, network and widgets components).

On Debian and derivatives, the necessary packages are `libqt5network5` and `libqt5widgets5`. To compile it, execute the following commands in the root directory of this repository.

```bash
mkdir Build
cd Build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

This results in an executable file called `DirectionalWhistleTester` in the `Build` directory.

For Windows and macOS, Qt must be installed differently. Otherwise, the compilation process is the same, provided that CMake is installed.

## Configuration

There are three configuration files in the `Config/` directory:
- `teams.cfg` contains the names of all teams and their team numbers.
- `whistleLocations.json` is a JSON array of the locations (meters) from which the whistle will be blown (the one-based array index corresponds to the location index in the GUI).
- `robotPoses.json` is a JSON array of the poses (meters/degrees) at which the robots (the one-based array index corresponds to the robot jersey number).

For testing, you will likely want to adjust the numbers in the `whistleLocations.json` and `robotPoses.json`.

## Usage

After starting the program, there is only the possibility to start a challenge pass by clicking the button labeled "Start Challenge...". This will open a dialog asking for the team (which will automatically determine the UDP port on which to listen for messages according to the team number) and the jersey numbers of the set of robots that the team handed in for the challenge. At least one robot must be selected to start the challenge.

Once the start dialog has been finished, a table will show up that summarizes the current state of the challenge pass. On the vertical axis, the different attempts (each corresponding to one whistle location) are listed. A challenge pass always proceeds from top to bottom. The "Location" column shows the index of the location from which the whistle will be blown corresponding to the array in the file `whistleLocations.json`. The purpose of this column is that the order of locations is randomized in each challenge pass. The columns "Remaining Time" and "Score" are filled as the challenge progesses with the time that was left when the whistle message arrived (in milliseconds) and the automatically calculated score for each attempt, respectively. At the same time, a log file is written which contains all relevant information to collect all scores afterwards.

The "Start Attempt" button should be pressed in the moment the whistle is blown. This starts a 5 second timer until which messages will be accepted. The attempt ends after either 5 seconds have passed or a whistle message has been received.

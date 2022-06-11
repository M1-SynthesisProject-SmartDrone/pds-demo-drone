# Demo Arkins

This is a simple project aiming to show a little demonstration with all features implemented about arkins

## How to install it

First, launch the `install_dependencies.sh` script in order to install every needed dependencies.

Next, copy the `config/config.cfg.TEMPLATE` file, renaming it to `config/config.cfg` and put the configurations wanted for this computer (the `config/config.cfg` file is not pushed to the repository).

The config have references to one folder (the images one, the folder "images" is in this repository), and to one file (the "map" to load, more on that later).
In order to be safe, it is advised to put absolute paths instead of relative ones.

Finally, you can call the `compile.sh` file in order to compile the project, creating the executable in the `bin` folder (the config.cfg created must always be aside the executable in order to work).

## The map file

> One map is already available : `data/map_test.json`

This is a single json file looking like this :

```json
{
    "drone": {
        "x": 0.0,
        "y": 0.0,
        "rotation": 0.0
    },
    "attractivePoints": [
        {
            "x": 200.0,
            "y": 200.0
        },
        {
            "x": 500.0,
            "y": 500.0
        }
    ],
    "repulsivePoints": [
        // ....
    ],
    "tangentialPoints": [
        // ....
    ],
    "uniformFields": [
        // TODO NOT IMPLEMENTED
    ]
}
```

> The order of the points in all fields is not important, EXCEPT FOR "attractivePoints" (must be set in the path order).

The simulation screen will adapt depending on the position of all points, don't worry about that !

## Commands

Along with the possibility to close the window (big feature !), you have access to multiple key bindings :
- P : permits to pause the simulation (in case it is too fast)
- R : permits to reset the simulation (this pauses the simulation too)
# SaveGuard for Borderlands 3

SaveGuard for Borderlands 3 (or simply SaveGuard) is a simple and flexible tool for automatically creating local backups of your Borderlands3 save files. I wrote this tool after many of my friends experienced crashes on their systems that resulted in permanently corrupted save files. The Steam Cloud Save feature was no help, as it had already backed up their corrupted saves! SaveGuard is an extra layer of semi-automated protection against save-file corruption.

## Getting Started

### Prerequisites

There are no pre-requisites for SaveGuard. All you need is a pre-existing save file for Borderlands 3 and a working Windows installation!

### Using SaveGuard

There's no need to run an installer to gain access to SaveGuard. Simply drop any release version of SaveGuard directly into your save folder and run it from there. Your save folder's path should look something like this:

`C:\Users\Name\Documents\My games\Borderlands 3\Saved\SaveGames\76561198093957\`

Once you've placed SaveGuard in the correct folder, you must place a shortcut to your Borderlands 3.exe in the folder with it. On my system using the Steam version of Borderlands 3, it is located at:

`D:\Games\SteamLibrary\steamapps\common\Borderlands 3\OakGame\Binaries\Win64`

Ensure that this shortcut is precisely named `Borderlands 3.lnk`.


## Developing SaveGuard

SaveGuard is developed using C++17. It relies on no libraries and has to external dependencies. If you would like to play around with SaveGuard's code, simply clone it onto your machine.

### Compiling

It's easy to compile SaveGuard. Make sure you have a working C++17-capable compiler and run make.bat. It will do all the work for you.

## Contributing

I am open to accepting contributions to SaveGuard. I do not currently have a formal system for working through this, so bear with me. 

## Authors

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* GearBox for not designing a stable game that doesn't write directly to the only copy of its savefiles. Lord knows this wouldn't be necessary if they did.

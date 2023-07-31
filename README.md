# IPK Project 1: IPK Calculator Client

## Hodnocení

17.5 / 20

> Koutensky - (-0,5) teoria - chyba. - (-0,5) navrh - chyba, resp. jedna veta. co je skoda, pretoze toto je jeden z mala projektov kde je nejaky rozumny navrh o ktorom by sa dalo pisat. - (-0,5) testovanie - koncepcne dobre, hodilo by sa ale popisat jednotlive situacie ktore boli testovane priamo v hlavnom texte. - kod - rozumny oop navrh, chvalim pouzitie abstraktnej baze.

## Původní README

This README is the main documentation for this project.

## Usage

```
ipkcpc -h <host> -p <port> -m <mode>
```

## Requirements

- `gcc`
- `make`

For testing

- `python3`

## Make targets

- `make` Builds the project and creates `ipkcpc` binary in project root
- `make run_tcp` Builds project and runs client in TCP mode with default (examples) arguments
- `make run_udp` Builds project and runs client in UDP mode with default (example) arguments
- `make test` Runs tests. _NOTE: Both TCP and UDP server has to be running_.
- `make zip` Creates final ZIP file for assignment submission
- `make clean` Cleans temporary files (e.g object files)

## Project structure

These are the notable files and folders.

- `/examples` Example inputs
- `ipkcpc.cc` Entry point
- `args.cc`, `args.hpp` Argument parsing module
- `client.cc`, `client.hpp` Abstract base class, factory for clients
- `tcp-client.cc`, `tcp-client.hpp` TCP client implementation
- `udp-client.cc`, `udp-client.hpp` UDP client implementation
- `test.py` Tests

## Code

The application is written in C++20. Client is abstract class and factory for specific (currently TCP and UDP) implementations. Arguments are parsed in their own module. Project aims to be simple and extensible.

## Testing

Testing was done with custom tests written in Python 3 with unittest library.
It tests tcp, udp comunication and argument parsing. Tests live in `test.py`, they have it's own help, which can be viewed with `python3 test.py -h`.
Some edge-cases hard to tests automatically were tested manually, this could be
improved in the future.

The application was tested as a whole. It was also confirmed that it compiles and runs on the reference VM. Tests require two servers running (one in TCP and other in UDP mode) and can be run against reference ipkcpd implementation or any other compatible implementation.

Testing proof is located in `test.log` file.

## Extra features

### Automatic _BYE_ sending

Client automatically sends _BYE_ message if user doesn't do it. This means, that if you supply file without _BYE_ message at the end, connection will
still properly terminate.

## License

License in located in **LICENSE** file.

## References

- Changelog format - https://keepachangelog.com/cs/1.0.0/
- General project instructions - https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master
- Project instructions https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Project%201
- Protocol specification https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Project%201/Protocol.md
- C++ reference https://en.cppreference.com/w/

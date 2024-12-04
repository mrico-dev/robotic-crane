# Set-up

## Dependencies (for debian)
```
sudo apt-get install cmake
sudo apt install libboost-all-de
sudo apt install libwebsocketpp-dev
sudo apt-get install libasio-dev
```

## Compile project
```
cmake -B build
cd build/
make
```

If the linker cannot find jsoncpp libraries, you may need to initialize them:
```
# From ./backend/
cd external/jsoncpp/
python3 amalgate.py
```
then run the first step again.

## Run the server
```
./crane-backend
```

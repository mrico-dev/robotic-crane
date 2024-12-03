
`sudo apt install libboost-all-dev`
`sudo apt install libwebsocketpp-dev`
`sudo apt-get install libasio-dev`
`cd external/jsoncpp/; python3 amalgate.py`

```
mkdir build && cd build
cmake ..
make
```

`./websocket_json_client`
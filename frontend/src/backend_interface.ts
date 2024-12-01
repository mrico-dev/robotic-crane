import * as VISUALIZER from './visualizer';

// Ideally, these should be stored in a configuration file
const WEBSOCKET_PORT: string = "8080";
const WEBSOCKET_URL: string = "ws://localhost";

var socket: WebSocket;

interface backend_message {
    liftHeight: number,
    liftArmAngleDegree: number,        
    armForearmAngleDegree: number, 
    forearmGripAngleDegree: number,
    gripperSpacing: number,
}

function messageHandler(event: any): void {
    const jsonData = event.data;
    console.log('Message from server:', jsonData);

    var data = JSON.parse(jsonData) as backend_message;
    set_crane_position(data);
}

function set_crane_position(position: backend_message): void {
    VISUALIZER.setLiftHeight(position.liftHeight);
    VISUALIZER.setLiftArmAngle(position.liftArmAngleDegree);
    VISUALIZER.setArmForearmAngle(position.armForearmAngleDegree);
    VISUALIZER.setForearmGripAngle(position.forearmGripAngleDegree);
    VISUALIZER.setGripperSpacing(position.gripperSpacing);
}

export function setup_button(): void {
    document.getElementById("message-send")?.addEventListener("click", () => {
        console.log("Sending message");
        socket.send(JSON.stringify("Hello this is frontend."));
    });
}

export function initialize_connection(): void {
    console.log("Setting up connection with backend...");
    socket = new WebSocket(WEBSOCKET_URL + ":" + WEBSOCKET_PORT);

    socket.addEventListener('open', () => {
        console.log("Connected.");
    });

    socket.addEventListener('message', messageHandler);

    socket.addEventListener('close', () => {
        console.log('Connection closed.');
    });

    socket.addEventListener('error', (error) => {
        console.error('/!\\ WebSocket error: ', error);
    });
}



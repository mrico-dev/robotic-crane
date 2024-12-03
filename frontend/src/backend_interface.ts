import * as VISUALIZER from './visualizer';

// Ideally, these should be stored in a configuration file
const WEBSOCKET_PORT: string = "8080";
const WEBSOCKET_URL: string = "ws://localhost";

var socket: WebSocket;

interface BackendMessage {
    liftHeight: number,
    liftArmAngleDegree: number,        
    armForearmAngleDegree: number, 
    forearmGripAngleDegree: number,
    gripperSpacing: number,
}

interface FrontendMessage {
    x: number,
    y: number,
    z: number,
}

function messageHandler(event: any): void {
    const jsonData = event.data;
    console.log('Message from server:', jsonData);

    try {
        var data = JSON.parse(jsonData) as BackendMessage;
        set_crane_position(data);
    } catch (error) {
        console.error("Could not parse data received from backend: ", error);
    }
}

function set_crane_position(position: BackendMessage): void {
    VISUALIZER.setLiftHeight(position.liftHeight / 1000);
    VISUALIZER.setLiftArmAngle(position.liftArmAngleDegree);
    VISUALIZER.setArmForearmAngle(position.armForearmAngleDegree);
    VISUALIZER.setForearmGripAngle(position.forearmGripAngleDegree);
    VISUALIZER.setGripperSpacing(position.gripperSpacing / 1000);
}

export function setup_button(): void {
    const xInput: HTMLInputElement = document.getElementById('x-coord') as HTMLInputElement;
    const yInput: HTMLInputElement = document.getElementById('y-coord') as HTMLInputElement;
    const zInput: HTMLInputElement = document.getElementById('z-coord') as HTMLInputElement;

    document.getElementById("message-send")?.addEventListener("click", () => {
        const x_mm: number = parseFloat(xInput.value) * 1000;
        const y_mm: number = parseFloat(yInput.value) * 1000;
        const z_mm: number = parseFloat(zInput.value) * 1000;
        const message: FrontendMessage = {x: x_mm, y: y_mm, z: z_mm};

        const json: string = JSON.stringify(message);
        console.log("Sending message: ", json);
        socket.send(json);
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



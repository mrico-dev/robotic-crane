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

interface CoordFrontendMessage {
    type: string,
    x: number,
    y: number,
    z: number,
}

interface CraneFrontendMessage {
    type: string,
    lift_elevation: number,
    swing_rotation: number,
    elbow_rotation: number,
    wrist_rotation: number,
    grip_extension: number,
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
    // Send crane
    const liftHeightInput: HTMLInputElement = document.getElementById('lift-height') as HTMLInputElement;
    const swingAngleInput: HTMLInputElement = document.getElementById('swing-angle') as HTMLInputElement;
    const elbowAngleInput: HTMLInputElement = document.getElementById('elbow-angle') as HTMLInputElement;
    const wristAngleInput: HTMLInputElement = document.getElementById('wrist-angle') as HTMLInputElement;
    const gripExtensionInput: HTMLInputElement = document.getElementById('grip-extension') as HTMLInputElement;

    document.getElementById("crane-send")?.addEventListener("click", () => {
        const liftHeight = parseFloat(liftHeightInput.value);
        const swingAngle = parseFloat(swingAngleInput.value);
        const elbowAngle = parseFloat(elbowAngleInput.value);
        const wristAngle = parseFloat(wristAngleInput.value);
        const gripExtension = parseFloat(gripExtensionInput.value);

        const message: CraneFrontendMessage = {type: "crane",
            lift_elevation: liftHeight,
            swing_rotation: swingAngle,
            elbow_rotation: elbowAngle,
            wrist_rotation: wristAngle,
            grip_extension: gripExtension};
        
        const json: string = JSON.stringify(message);
        console.log("Sending message: ", json);
        socket.send(json);
    });

    // Send coords
    const xInput: HTMLInputElement = document.getElementById('x-coord') as HTMLInputElement;
    const yInput: HTMLInputElement = document.getElementById('y-coord') as HTMLInputElement;
    const zInput: HTMLInputElement = document.getElementById('z-coord') as HTMLInputElement;

    document.getElementById("coord-send")?.addEventListener("click", () => {
        const x_mm: number = parseFloat(xInput.value) * 1000;
        const y_mm: number = parseFloat(yInput.value) * 1000;
        const z_mm: number = parseFloat(zInput.value) * 1000;
        const message: CoordFrontendMessage = {type: "coord", x: x_mm, y: y_mm, z: z_mm};

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



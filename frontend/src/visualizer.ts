import * as THREE from 'three';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls';
import * as BACKEND from './backend_interface';

// Constants (should be loaded from a config file idealy)
// values are in meters (m)
const BASE_HEIGHT: number = 1;
const BASE_WIDTH: number = 2;

const LIFT_HEIGHT: number = 10;
const LIFT_WIDTH: number = 1;

const ARM_LENGTH: number = 8;
const ARM_WIDTH: number = 1;

const ARM_FOREARM_SPACING_Y: number = 1;
const FOREARM_LENGTH: number = 2;
const FOREARM_WIDTH: number = .8;

const FOREARM_GRIP_SPACING_Y: number = .3;
const FOREARM_GRIP_SPACING_X: number = .5;
const INITIAL_GRIP_SPACING: number = .5; 
const GRIPPER_WIDTH: number = .05;
const GRIPPER_LENGTH: number = .5;
const GRIPPER_HEIGHT: number = .1;

// Canvas takes N% of the screen's width
const MAX_CANVAS_HEIGHT: number = 20;
const MIN_CANVAS_HEIGHT: number = 0;
const CANVAS_WIDTH_RATIO_PERCENT: number = 90;

const scene: THREE.Scene = new THREE.Scene();

// Set up scene
const camera: THREE.PerspectiveCamera = new THREE.PerspectiveCamera(
  75,
  window.innerWidth / window.innerHeight,
  1.1,
  1000
);
camera.position.set(10, 10, 20);
camera.lookAt(0, 0, 0);

const canvas = document.querySelector('#three-canvas') as HTMLCanvasElement;
if (!canvas) {
  throw new Error('Canvas element not found');
}

const renderer: THREE.WebGLRenderer = new THREE.WebGLRenderer({ canvas });
renderer.setSize(window.innerWidth / 100 * CANVAS_WIDTH_RATIO_PERCENT, window.innerHeight);

const ambientLight: THREE.AmbientLight = new THREE.AmbientLight(0xffffff, 0.5);
scene.add(ambientLight);

const directionalLight: THREE.DirectionalLight = new THREE.DirectionalLight(0xffffff, 1);
directionalLight.position.set(10, 10, 10);
scene.add(directionalLight);

const grid: THREE.GridHelper = new THREE.GridHelper(50, 50);
scene.add(grid);

const baseGeometry: THREE.CylinderGeometry = new THREE.CylinderGeometry(BASE_WIDTH, BASE_WIDTH, BASE_HEIGHT, 32);
const baseMaterial: THREE.MeshPhongMaterial = new THREE.MeshPhongMaterial({ color: 0x555555 });
const base: THREE.Mesh = new THREE.Mesh(baseGeometry, baseMaterial);
scene.add(base);

const liftGeometry: THREE.BoxGeometry = new THREE.BoxGeometry(LIFT_WIDTH, LIFT_HEIGHT, LIFT_WIDTH);
const liftMaterial: THREE.MeshPhongMaterial = new THREE.MeshPhongMaterial({ color: 0x777777 });
const lift: THREE.Mesh = new THREE.Mesh(liftGeometry, liftMaterial);
lift.position.y = LIFT_HEIGHT / 2;
base.add(lift);

const armGeometry: THREE.BoxGeometry = new THREE.BoxGeometry(ARM_LENGTH, ARM_WIDTH, ARM_WIDTH);
const armMaterial: THREE.MeshPhongMaterial = new THREE.MeshPhongMaterial({ color: 0xff0000 });
const arm: THREE.Mesh = new THREE.Mesh(armGeometry, armMaterial);
arm.position.x = ARM_LENGTH / 2 + ARM_WIDTH / 2;
lift.add(arm);

const armForearmPivot: THREE.Object3D = new THREE.Object3D();
armForearmPivot.position.set(ARM_LENGTH / 2, 0, 0);
arm.add(armForearmPivot);

const forearmGeometry: THREE.BoxGeometry = new THREE.BoxGeometry(FOREARM_LENGTH, FOREARM_WIDTH, FOREARM_WIDTH);
const forearmMaterial: THREE.MeshPhongMaterial = new THREE.MeshPhongMaterial({ color: 0x0000ff });
const forearm: THREE.Mesh = new THREE.Mesh(forearmGeometry, forearmMaterial);
forearm.position.x = FOREARM_LENGTH / 2;
forearm.position.y = -ARM_FOREARM_SPACING_Y;
armForearmPivot.add(forearm);

const forearmGripPivot: THREE.Object3D = new THREE.Object3D();
forearmGripPivot.position.set(FOREARM_LENGTH / 2, 0, 0);
forearm.add(forearmGripPivot);

const gripLeftGeometry: THREE.BoxGeometry = new THREE.BoxGeometry(GRIPPER_LENGTH, GRIPPER_HEIGHT, GRIPPER_WIDTH);
const gripLeftMaterial: THREE.MeshPhongMaterial = new THREE.MeshPhongMaterial({ color: 0x00ff00 });
const gripLeft: THREE.Mesh = new THREE.Mesh(gripLeftGeometry, gripLeftMaterial);
gripLeft.position.x = FOREARM_GRIP_SPACING_X;
gripLeft.position.y = -FOREARM_GRIP_SPACING_Y;
gripLeft.position.z = -INITIAL_GRIP_SPACING / 2;
forearmGripPivot.add(gripLeft);

const gripRightGeometry: THREE.BoxGeometry = new THREE.BoxGeometry(GRIPPER_LENGTH, GRIPPER_HEIGHT, GRIPPER_WIDTH);
const gripRightMaterial: THREE.MeshPhongMaterial = new THREE.MeshPhongMaterial({ color: 0x00ff00 });
const gripRight: THREE.Mesh = new THREE.Mesh(gripRightGeometry, gripRightMaterial);
gripRight.position.x = FOREARM_GRIP_SPACING_X;
gripRight.position.y = -FOREARM_GRIP_SPACING_Y;
gripRight.position.z = INITIAL_GRIP_SPACING / 2;
forearmGripPivot.add(gripRight);

// Showing rotation axis
const baseAxisGeometry: THREE.BufferGeometry = new THREE.BufferGeometry().setFromPoints([
  new THREE.Vector3(0, MIN_CANVAS_HEIGHT, 0),
  new THREE.Vector3(0, MAX_CANVAS_HEIGHT, 0),
]); 
const baseAxisLine: THREE.Line = new THREE.Line(baseAxisGeometry, new THREE.LineBasicMaterial({ color: 0xff0000 }));
scene.add(baseAxisLine);

var armForearmPivotWorldPos: THREE.Vector3 = new THREE.Vector3(); 
armForearmPivot.getWorldPosition(armForearmPivotWorldPos);
const armForearmAxisGeometry: THREE.BufferGeometry = new THREE.BufferGeometry().setFromPoints([
  new THREE.Vector3(armForearmPivotWorldPos.x, MIN_CANVAS_HEIGHT, armForearmPivotWorldPos.z),
  new THREE.Vector3(armForearmPivotWorldPos.x, MAX_CANVAS_HEIGHT, armForearmPivotWorldPos.z),
]); 
const armForearmAxisLine: THREE.Line = new THREE.Line(armForearmAxisGeometry, new THREE.LineBasicMaterial({ color: 0x0000ff }));
scene.add(armForearmAxisLine);

var forearmGripPivotWorldPos: THREE.Vector3 = new THREE.Vector3(); 
forearmGripPivot.getWorldPosition(forearmGripPivotWorldPos);
const forearmGripAxisGeometry: THREE.BufferGeometry = new THREE.BufferGeometry().setFromPoints([
  new THREE.Vector3(forearmGripPivotWorldPos.x, MIN_CANVAS_HEIGHT, forearmGripPivotWorldPos.z),
  new THREE.Vector3(forearmGripPivotWorldPos.x, MAX_CANVAS_HEIGHT, forearmGripPivotWorldPos.z),
]); 
const forearmGripAxisLine: THREE.Line = new THREE.Line(forearmGripAxisGeometry, new THREE.LineBasicMaterial({ color: 0x00ff00 }));
scene.add(forearmGripAxisLine);

function refreshAxis(): void {
  armForearmPivot.getWorldPosition(armForearmPivotWorldPos);
  armForearmAxisLine.geometry.attributes.position.setXYZ(0, armForearmPivotWorldPos.x, MIN_CANVAS_HEIGHT, armForearmPivotWorldPos.z);
  armForearmAxisLine.geometry.attributes.position.setXYZ(1, armForearmPivotWorldPos.x, MAX_CANVAS_HEIGHT, armForearmPivotWorldPos.z);
  armForearmAxisLine.geometry.attributes.position.needsUpdate = true;

  forearmGripPivot.getWorldPosition(forearmGripPivotWorldPos);
  forearmGripAxisLine.geometry.attributes.position.setXYZ(0, forearmGripPivotWorldPos.x, MIN_CANVAS_HEIGHT, forearmGripPivotWorldPos.z)
  forearmGripAxisLine.geometry.attributes.position.setXYZ(1, forearmGripPivotWorldPos.x, MAX_CANVAS_HEIGHT, forearmGripPivotWorldPos.z)
  forearmGripAxisLine.geometry.attributes.position.needsUpdate = true;
}

// Controls
const controls: OrbitControls = new OrbitControls(camera, renderer.domElement);
controls.enableDamping = true;
controls.dampingFactor = 0.1;

// Function for moving the robot
function degToRad(val: number): number {
  return val * Math.PI / 180;
}

function setLiftHeight(newLiftHeight: number): void {
  var newLiftHeightClipped = Math.min(Math.max(newLiftHeight, BASE_HEIGHT), liftGeometry.parameters.height);
  var liftHeightRelative: number = newLiftHeightClipped - LIFT_HEIGHT / 2; 
  arm.position.y = liftHeightRelative;
  base.updateMatrixWorld();
}

function setLiftArmAngle(newLiftArmAngleDeg : number): void {
  lift.rotation.y = degToRad(newLiftArmAngleDeg);
  base.updateMatrixWorld();
  refreshAxis();
}

function setArmForearmAngle(newArmForearmAngleDeg: number): void {
  armForearmPivot.rotation.y = degToRad(newArmForearmAngleDeg);
  base.updateMatrixWorld();
  refreshAxis();
}

function setForearmGripAngle(newForearmGripAngleDef: number): void {
  forearmGripPivot.rotation.y = degToRad(newForearmGripAngleDef);
  base.updateMatrixWorld();
  refreshAxis();
}

function setGripperSpacing(newGripperSpacing : number): void {
  gripLeft.position.z = -newGripperSpacing / 2;
  gripRight.position.z = newGripperSpacing / 2;
  base.updateMatrixWorld();
}

window.addEventListener('resize', (): void => {
  camera.aspect = window.innerWidth / window.innerHeight;
  camera.updateProjectionMatrix();
  renderer.setSize(window.innerWidth / 100 * CANVAS_WIDTH_RATIO_PERCENT, window.innerHeight);
});

// Input listeners
const liftHeightInput: HTMLInputElement = document.getElementById('lift-height') as HTMLInputElement;
const liftArmAngleInput: HTMLInputElement = document.getElementById('lift-arm-angle') as HTMLInputElement;
const armForearmAngleInput: HTMLInputElement = document.getElementById('arm-forearm-angle') as HTMLInputElement;
const forearmGripAngleInput: HTMLInputElement = document.getElementById('forearm-grip-angle') as HTMLInputElement;
const gripSpacingInput: HTMLInputElement = document.getElementById('gripper-spacing') as HTMLInputElement;

liftHeightInput.addEventListener('input', (event: Event) => {
  const newLiftHeight = parseFloat((event.target as HTMLInputElement).value);
  setLiftHeight(newLiftHeight);
});

liftArmAngleInput.addEventListener('input', (event: Event) => {
  const newLiftArmAngle = parseFloat((event.target as HTMLInputElement).value);
  setLiftArmAngle(newLiftArmAngle);
});

armForearmAngleInput.addEventListener('input', (event: Event) => {
  const newArmForearmAngle = parseFloat((event.target as HTMLInputElement).value);
  setArmForearmAngle(newArmForearmAngle);
});

forearmGripAngleInput.addEventListener('input', (event: Event) => {
  const newArmGripAngle = parseFloat((event.target as HTMLInputElement).value);
  setForearmGripAngle(newArmGripAngle);
});

gripSpacingInput.addEventListener('input', (event: Event) => {
  const newGripSpacing = parseFloat((event.target as HTMLInputElement).value);
  setGripperSpacing(newGripSpacing);
});

// Main function
function animate(): void {
  requestAnimationFrame(animate);
  controls.update();
  renderer.render(scene, camera);
}

animate();
BACKEND.initialize_connection();
BACKEND.setup_button();
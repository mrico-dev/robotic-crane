import * as THREE from 'three';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls';

// Constants (should be loaded from a config file idealy)
// values are in meters (m)
const BASE_HEIGHT: number = 1;
const BASE_WIDTH: number = 2;

const LIFT_HEIGHT: number = 10;
const LIFT_WIDTH: number = 1;

const ARM_LENGTH: number = 6;
const ARM_WIDTH: number = 1;

const ARM_FOREARM_SPACING_Y: number = 1;
const FOREARM_LENGTH: number = 4;
const FOREARM_WIDTH: number = .8;

const FOREARM_GRIP_SPACING_Y: number = .3;
const FOREARM_GRIP_SPACING_X: number = .5;
const INITIAL_GRIP_SPACING: number = .5; 
const GRIPPER_WIDTH: number = .05;
const GRIPPER_LENGTH: number = .5;
const GRIPPER_HEIGHT: number = .1;

const GOAL_POINT_SIZE = .1;

// Canvas takes N% of the screen's width
const MAX_CANVAS_HEIGHT: number = 20;
const MIN_CANVAS_HEIGHT: number = 0;
const CANVAS_WIDTH_RATIO_PERCENT: number = 90;

// shared variables
var renderer: THREE.WebGLRenderer;
var scene: THREE.Scene;
var camera: THREE.PerspectiveCamera;
var controls: OrbitControls;

var base: THREE.Mesh;
var lift: THREE.Mesh;
var arm: THREE.Mesh;
var armForearmPivot: THREE.Object3D;
var forearm: THREE.Mesh;
var forearmGripPivot: THREE.Object3D;
var gripLeft: THREE.Mesh;
var gripRight: THREE.Mesh;

var baseAxisLine: THREE.Line;
var armForearmAxisLine: THREE.Line;
var forearmGripAxisLine: THREE.Line;

var goalPoint: THREE.Points;

export function build_scene(): void {
  scene = new THREE.Scene();

  // Set up scene
  camera = new THREE.PerspectiveCamera(
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

  renderer = new THREE.WebGLRenderer({ canvas });
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
  base = new THREE.Mesh(baseGeometry, baseMaterial);
  base.position.y = BASE_HEIGHT / 2;
  scene.add(base);
  const zeroDegBaseIndicatorGeometry: THREE.BoxGeometry = new THREE.BoxGeometry(BASE_WIDTH / 1.5, BASE_WIDTH / 5, BASE_HEIGHT / 10); 
  const zeroDegBaseIndicator = new THREE.Mesh(zeroDegBaseIndicatorGeometry, new THREE.MeshPhongMaterial({ color: 0x990000 }));
  zeroDegBaseIndicator.position.x = BASE_WIDTH / 2;
  zeroDegBaseIndicator.position.y = BASE_HEIGHT / 2;
  base.add(zeroDegBaseIndicator);

  const liftGeometry: THREE.BoxGeometry = new THREE.BoxGeometry(LIFT_WIDTH, LIFT_HEIGHT, LIFT_WIDTH);
  const liftMaterial: THREE.MeshPhongMaterial = new THREE.MeshPhongMaterial({ color: 0x777777 });
  lift = new THREE.Mesh(liftGeometry, liftMaterial);
  lift.position.y = LIFT_HEIGHT / 2;
  base.add(lift);

  const armGeometry: THREE.BoxGeometry = new THREE.BoxGeometry(ARM_LENGTH, ARM_WIDTH, ARM_WIDTH);
  const armMaterial: THREE.MeshPhongMaterial = new THREE.MeshPhongMaterial({ color: 0xff0000 });
  arm = new THREE.Mesh(armGeometry, armMaterial);
  arm.position.x = ARM_LENGTH / 2;
  lift.add(arm);

  armForearmPivot = new THREE.Object3D();
  armForearmPivot.position.set(ARM_LENGTH / 2, 0, 0);
  arm.add(armForearmPivot);

  const forearmGeometry: THREE.BoxGeometry = new THREE.BoxGeometry(FOREARM_LENGTH, FOREARM_WIDTH / 2, FOREARM_WIDTH);
  const forearmMaterial: THREE.MeshPhongMaterial = new THREE.MeshPhongMaterial({ color: 0x0000ff });
  forearm = new THREE.Mesh(forearmGeometry, forearmMaterial);
  forearm.position.x = FOREARM_LENGTH / 2;
  forearm.position.y = -ARM_FOREARM_SPACING_Y;
  armForearmPivot.add(forearm);

  forearmGripPivot = new THREE.Object3D();
  forearmGripPivot.position.set(FOREARM_LENGTH / 2, 0, 0);
  forearm.add(forearmGripPivot);

  const gripLeftGeometry: THREE.BoxGeometry = new THREE.BoxGeometry(GRIPPER_LENGTH, GRIPPER_HEIGHT, GRIPPER_WIDTH);
  const gripLeftMaterial: THREE.MeshPhongMaterial = new THREE.MeshPhongMaterial({ color: 0x00ff00 });
  gripLeft = new THREE.Mesh(gripLeftGeometry, gripLeftMaterial);
  gripLeft.position.x = FOREARM_GRIP_SPACING_X;
  gripLeft.position.y = -FOREARM_GRIP_SPACING_Y;
  gripLeft.position.z = -INITIAL_GRIP_SPACING / 2;
  forearmGripPivot.add(gripLeft);

  const gripRightGeometry: THREE.BoxGeometry = new THREE.BoxGeometry(GRIPPER_LENGTH, GRIPPER_HEIGHT, GRIPPER_WIDTH);
  const gripRightMaterial: THREE.MeshPhongMaterial = new THREE.MeshPhongMaterial({ color: 0x00ff00 });
  gripRight = new THREE.Mesh(gripRightGeometry, gripRightMaterial);
  gripRight.position.x = FOREARM_GRIP_SPACING_X;
  gripRight.position.y = -FOREARM_GRIP_SPACING_Y;
  gripRight.position.z = INITIAL_GRIP_SPACING / 2;
  forearmGripPivot.add(gripRight);

  // Showing rotation axis
  const baseAxisGeometry: THREE.BufferGeometry = new THREE.BufferGeometry().setFromPoints([
    new THREE.Vector3(0, MIN_CANVAS_HEIGHT, 0),
    new THREE.Vector3(0, MAX_CANVAS_HEIGHT, 0),
  ]); 
  baseAxisLine = new THREE.Line(baseAxisGeometry, new THREE.LineBasicMaterial({ color: 0xff0000 }));
  scene.add(baseAxisLine);

  var armForearmPivotWorldPos: THREE.Vector3 = new THREE.Vector3(); 
  armForearmPivot.getWorldPosition(armForearmPivotWorldPos);
  const armForearmAxisGeometry: THREE.BufferGeometry = new THREE.BufferGeometry().setFromPoints([
    new THREE.Vector3(armForearmPivotWorldPos.x, MIN_CANVAS_HEIGHT, armForearmPivotWorldPos.z),
    new THREE.Vector3(armForearmPivotWorldPos.x, MAX_CANVAS_HEIGHT, armForearmPivotWorldPos.z),
  ]); 
  armForearmAxisLine = new THREE.Line(armForearmAxisGeometry, new THREE.LineBasicMaterial({ color: 0x0000ff }));
  scene.add(armForearmAxisLine);

  var forearmGripPivotWorldPos: THREE.Vector3 = new THREE.Vector3(); 
  forearmGripPivot.getWorldPosition(forearmGripPivotWorldPos);
  const forearmGripAxisGeometry: THREE.BufferGeometry = new THREE.BufferGeometry().setFromPoints([
    new THREE.Vector3(forearmGripPivotWorldPos.x, MIN_CANVAS_HEIGHT, forearmGripPivotWorldPos.z),
    new THREE.Vector3(forearmGripPivotWorldPos.x, MAX_CANVAS_HEIGHT, forearmGripPivotWorldPos.z),
  ]); 
  forearmGripAxisLine = new THREE.Line(forearmGripAxisGeometry, new THREE.LineBasicMaterial({ color: 0x00ff00 }));
  scene.add(forearmGripAxisLine);

  const goalPointGeometry = new THREE.BufferGeometry();
  const goalPointInitialPos = new Float32Array([0, 0, 0]);
  goalPointGeometry.setAttribute('position', new THREE.BufferAttribute(goalPointInitialPos, 3));
  goalPoint = new THREE.Points(goalPointGeometry, new THREE.PointsMaterial({
    color: 0xff0000,
    size: GOAL_POINT_SIZE,
  }))
  scene.add(goalPoint);

  // Controls
  controls = new OrbitControls(camera, renderer.domElement);
  controls.enableDamping = true;
  controls.dampingFactor = 0.1;
}

function refreshAxis(): void {
  var baseWorldPos: THREE.Vector3 = new THREE.Vector3(); 
  base.getWorldPosition(baseWorldPos);
  baseAxisLine.geometry.attributes.position.setXYZ(0, baseWorldPos.x, MIN_CANVAS_HEIGHT, baseWorldPos.z);
  baseAxisLine.geometry.attributes.position.setXYZ(1, baseWorldPos.x, MAX_CANVAS_HEIGHT, baseWorldPos.z);
  baseAxisLine.geometry.attributes.position.needsUpdate = true;

  var armForearmPivotWorldPos: THREE.Vector3 = new THREE.Vector3(); 
  armForearmPivot.getWorldPosition(armForearmPivotWorldPos);
  armForearmAxisLine.geometry.attributes.position.setXYZ(0, armForearmPivotWorldPos.x, MIN_CANVAS_HEIGHT, armForearmPivotWorldPos.z);
  armForearmAxisLine.geometry.attributes.position.setXYZ(1, armForearmPivotWorldPos.x, MAX_CANVAS_HEIGHT, armForearmPivotWorldPos.z);
  armForearmAxisLine.geometry.attributes.position.needsUpdate = true;

  var forearmGripPivotWorldPos: THREE.Vector3 = new THREE.Vector3(); 
  forearmGripPivot.getWorldPosition(forearmGripPivotWorldPos);
  forearmGripAxisLine.geometry.attributes.position.setXYZ(0, forearmGripPivotWorldPos.x, MIN_CANVAS_HEIGHT, forearmGripPivotWorldPos.z)
  forearmGripAxisLine.geometry.attributes.position.setXYZ(1, forearmGripPivotWorldPos.x, MAX_CANVAS_HEIGHT, forearmGripPivotWorldPos.z)
  forearmGripAxisLine.geometry.attributes.position.needsUpdate = true;
}

// Functions for moving the robot
function degToRad(val: number): number {
  return val * Math.PI / 180;
}

export function setLiftHeight(newLiftHeight: number): void {
  var newLiftHeightClipped = Math.min(Math.max(newLiftHeight, ARM_WIDTH / 2), LIFT_HEIGHT);
  var liftHeightRelative: number = newLiftHeightClipped - LIFT_HEIGHT / 2 - ARM_WIDTH / 2; 
  arm.position.y = liftHeightRelative;
  base.updateMatrixWorld();
}

export function setLiftArmAngle(newLiftArmAngleDeg : number): void {
  lift.rotation.y = degToRad(newLiftArmAngleDeg);
  base.updateMatrixWorld();
  refreshAxis();
}

export function setArmForearmAngle(newArmForearmAngleDeg: number): void {
  armForearmPivot.rotation.y = degToRad(newArmForearmAngleDeg);
  base.updateMatrixWorld();
  refreshAxis();
}

export function setForearmGripAngle(newForearmGripAngleDef: number): void {
  forearmGripPivot.rotation.y = degToRad(newForearmGripAngleDef);
  base.updateMatrixWorld();
  refreshAxis();
}

export function setGripperSpacing(newGripperSpacing : number): void {
  gripLeft.position.z = -newGripperSpacing / 2;
  gripRight.position.z = newGripperSpacing / 2;
  base.updateMatrixWorld();
}

export function setGoalPoint(x: number, y: number, z: number): void {
  goalPoint.position.x = x;
  goalPoint.position.y = y;
  goalPoint.position.z = -z;
  goalPoint.updateMatrixWorld();
}

export function setCranePostion(x: number, y: number, z: number, rotation: number): void {
  base.position.x = x;
  base.position.y = y + BASE_HEIGHT / 2;
  base.position.z = z;
  base.rotation.y = degToRad(rotation);
  refreshAxis();
  base.updateMatrixWorld();
}

window.addEventListener('resize', (): void => {
  camera.aspect = window.innerWidth / window.innerHeight;
  camera.updateProjectionMatrix();
  renderer.setSize(window.innerWidth / 100 * CANVAS_WIDTH_RATIO_PERCENT, window.innerHeight);
});

// Main function
export function animate(): void {
  requestAnimationFrame(animate);
  controls.update();
  renderer.render(scene, camera);
}


